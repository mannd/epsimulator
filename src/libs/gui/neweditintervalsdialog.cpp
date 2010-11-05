/***************************************************************************
 *   Copyright (C) 2010 by EP Studios, Inc.                                *
 *   mannd@epstudiossoftware.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "neweditintervalsdialog.h"

#include <QComboBox>
#include <QDataWidgetMapper>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>

#include <QtDebug>

using EpGui::NewEditIntervalsDialog;
using EpGui::EditIntervalTypeDialog;

NewEditIntervalsDialog::NewEditIntervalsDialog(QWidget* parent)
    : QDialog(parent) {
    setupUi(this);
    setWindowTitle(tr("Intervals"));
    showCopyButton(false);
    model_ = new QSqlRelationalTableModel(this);
    model_->setTable("Intervals");
    model_->setRelation(Interval_Mark1,
                        QSqlRelation("Marks", "MarkID", "Name"));
    model_->setRelation(Interval_Mark2,
                        QSqlRelation("Marks", "MarkID", "Name"));
    model_->select();

    listView->setModel(model_);
    listView->setModelColumn(Interval_Name);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(removeItem()));
    connect(newButton, SIGNAL(clicked()), this, SLOT(insert()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(edit()));
    connect(listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit()));

    listView->setFocus();
}

void NewEditIntervalsDialog::insert() {
    editItem(NewItem);
}

void NewEditIntervalsDialog::edit() {
    editItem(EditItem);
}

void NewEditIntervalsDialog::removeItem() {
    QModelIndex index = listView->currentIndex();
    if (!index.isValid())
        return;
    int result = QMessageBox::warning(this, tr("Delete Item?"),
                         tr("The selected item will be permanently deleted."
                            " Do you wish to continue?"),
                            QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        QSqlDatabase::database().transaction();
        model_->removeRow(index.row());
        model_->submitAll();
        QSqlDatabase::database().commit();
    }
    listView->setFocus();
}

void NewEditIntervalsDialog::selectionIsEmptyWarning() {
    QMessageBox::information(this, tr("No Item"),
                             tr("You must select an item first"));
}

void NewEditIntervalsDialog::editItem(EditorType type) {
    QModelIndex index = listView->currentIndex();
    if (type == EditItem && !index.isValid()) {
        selectionIsEmptyWarning();
        return;
    }
    int row = index.row();
    EditIntervalTypeDialog d(type, model_, row, this);
    d.exec();
}

void NewEditIntervalsDialog::showCopyButton(bool show) {
    copyButton->setVisible(show);
}

EditIntervalTypeDialog::EditIntervalTypeDialog(
       NewEditIntervalsDialog::EditorType type,
       QSqlRelationalTableModel* model,
       int row,
       QWidget* parent)
   : QDialog(parent) {
   QString editType;
   if (type == NewEditIntervalsDialog::NewItem)
       editType = tr("New");
   else
       editType = tr("Edit");
   setWindowTitle(tr("%1 Interval Type").arg(editType));
   // box is too narrow unless below set
   setMinimumSize(300,300);

   mapper_ = new QDataWidgetMapper(this);
   mapper_->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
   mapper_->setModel(model);
   mapper_->setItemDelegate(new QSqlRelationalDelegate(this));

   QLabel* nameLabel = new QLabel(tr("Name:"));
   nameLineEdit_ = new QLineEdit;
   nameLabel->setBuddy(nameLineEdit_);
   mapper_->addMapping(nameLineEdit_, NewEditIntervalsDialog::Interval_Name);

   QLabel* mark1Label = new QLabel(tr("Mark 1"));
   mark1ComboBox_ = new QComboBox;
   mark1Label->setBuddy(mark1ComboBox_);
   QLabel* mark2Label = new QLabel(tr("Mark 2"));
   mark2ComboBox_ = new QComboBox;
   mark2Label->setBuddy(mark2ComboBox_);
   QSqlTableModel* mark1Model =
           model->relationModel(NewEditIntervalsDialog::Interval_Mark1);
   QSqlTableModel* mark2Model =
           model->relationModel(NewEditIntervalsDialog::Interval_Mark2);
   mark1ComboBox_->setModel(mark1Model);
   mark1ComboBox_->setModelColumn(mark1Model->fieldIndex("Name"));
   mark2ComboBox_->setModel(mark2Model);
   mark2ComboBox_->setModelColumn(mark2Model->fieldIndex("Name"));
   mapper_->addMapping(mark1ComboBox_, NewEditIntervalsDialog::Interval_Mark1);
   mapper_->addMapping(mark2ComboBox_, NewEditIntervalsDialog::Interval_Mark2);

   QLabel* widthLabel = new QLabel(tr("Width"));
   widthSpinBox_ = new QSpinBox;
   // default value listed as 5 in manual,
   // though I think actual Prucka shows 0.
   //widthSpinBox_->setValue(5);
   widthLabel->setBuddy(widthSpinBox_);
   mapper_->addMapping(widthSpinBox_, NewEditIntervalsDialog::Interval_Width);
   buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

   QHBoxLayout* layout1 = new QHBoxLayout;
   layout1->addWidget(nameLabel);
   layout1->addWidget(nameLineEdit_);
   QHBoxLayout* layout2 = new QHBoxLayout;
   layout2->addWidget(mark1Label);
   layout2->addWidget(mark1ComboBox_);
   QHBoxLayout* layout3 = new QHBoxLayout;
   layout3->addWidget(mark2Label);
   layout3->addWidget(mark2ComboBox_);
   QHBoxLayout* layout4 = new QHBoxLayout;
   layout4->addWidget(widthLabel);
   layout4->addWidget(widthSpinBox_);
   QVBoxLayout* vertLayout = new QVBoxLayout;
   vertLayout->addLayout(layout1);
   vertLayout->addLayout(layout2);
   vertLayout->addLayout(layout3);
   vertLayout->addLayout(layout4);
   vertLayout->addWidget(buttonBox_);
   setLayout(vertLayout);

   connect(nameLineEdit_, SIGNAL(textChanged(const QString&)),
           this, SLOT(enableOkButton(const QString&)));
   connect(buttonBox_, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox_, SIGNAL(rejected()), this, SLOT(reject()));
   enableOkButton(nameLineEdit_->text());

   mapper_->setCurrentIndex(row);
   if (type == NewEditIntervalsDialog::NewItem) {
        model->insertRow(row);
        nameLineEdit_->clear();
        widthSpinBox_->setValue(0);
        mark1ComboBox_->setCurrentIndex(0);
        mark2ComboBox_->setCurrentIndex(0);
        nameLineEdit_->setFocus();
   }

}

void EditIntervalTypeDialog::accept() {
    mapper_->submit();
    QDialog::accept();
}

void EditIntervalTypeDialog::enableOkButton(const QString& text) {
   buttonBox_->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
}

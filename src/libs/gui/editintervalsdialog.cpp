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

#include "editintervalsdialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QStringList>

using EpCore::Interval;
using EpCore::ItemList;
using EpGui::EditIntervalsDialog;
using EpGui::EditIntervalTypeDialog;

EditIntervalsDialog::EditIntervalsDialog(QWidget* parent)
        : AbstractEditItemsDialog(tr("Intervals"), parent) {
    showCopyButton(false);
    createListWidget();
}

void EditIntervalsDialog::createListWidget() {
    listWidget->clear();
    listWidget->setSortingEnabled(true);
    for (int i = 0; i < intervals_.size(); ++i) {
        new QListWidgetItem(intervals_[i].name(), listWidget);
    }
}

void EditIntervalsDialog::removeItem() {
    intervals_.remove(listWidget->currentItem()->text());
    createListWidget();
}


void EditIntervalsDialog::editItem() {
    makeEditIntervalTypeDialog(EditItem);
}

void EditIntervalsDialog::newItem() {
    makeEditIntervalTypeDialog(NewItem);
}

void EditIntervalsDialog::makeEditIntervalTypeDialog(EditorType type) {
    EditIntervalTypeDialog d(type, this);
    if (d.exec()) {
        EpCore::Interval interval = d.interval();
        if (intervals_.duplicate(interval) && type == NewItem) {
            QMessageBox::information(this, tr("Duplicate Interval"),
                                    tr("%1 is already present "
                                    "in the list").arg(interval.name()));
            return;
        }
        intervals_.append(d.interval());
        createListWidget();
    }
}

EditIntervalTypeDialog::EditIntervalTypeDialog(
        EditIntervalsDialog::EditorType type,
        QWidget* parent)
    : QDialog(parent) {
    QStringList markList;
    markList << tr("A1") << tr("A2") << tr("H1") << tr("H2")
            << tr("S1") << tr("S2") << tr("S3")
            << tr("S4") << tr("S5")
            << tr("V1") << tr("V2") << tr("V3")
            << tr("V4") << tr("V5");
    QString editType;
    if (type == EditIntervalsDialog::NewItem)
        editType = tr("New");
    else
        editType = tr("Edit");
    setWindowTitle(tr("%1 Interval Type").arg(editType));
    // box is too narrow unless below set
    setMinimumSize(300,300);
    QLabel* nameLabel = new QLabel(tr("Name:"));
    nameLineEdit_ = new QLineEdit;
    nameLabel->setBuddy(nameLineEdit_);

    QLabel* mark1Label = new QLabel(tr("Mark 1"));
    mark1ComboBox_ = new QComboBox;
    mark1ComboBox_->insertItems(0, markList);
    mark1Label->setBuddy(mark1ComboBox_);
    QLabel* mark2Label = new QLabel(tr("Mark 2"));
    mark2ComboBox_ = new QComboBox;
    mark2ComboBox_->insertItems(0, markList);
    mark2Label->setBuddy(mark2ComboBox_);

    QLabel* widthLabel = new QLabel(tr("Width"));
    widthSpinBox_ = new QSpinBox;
    // default value listed as 5 in manual,
    // though I think actual Prucka shows 0.
    widthSpinBox_->setValue(5);
    widthLabel->setBuddy(widthSpinBox_);

    buttonBox_ =
            new QDialogButtonBox(QDialogButtonBox::Ok
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
}

void EditIntervalTypeDialog::setInterval(const EpCore::Interval& interval) {
    nameLineEdit_->setText(interval.name());
    mark1ComboBox_->setEditText(interval.mark1());
    mark2ComboBox_->setEditText(interval.mark2());
    widthSpinBox_->setValue(interval.width());
    enableOkButton(nameLineEdit_->text());
}

EpCore::Interval EditIntervalTypeDialog::interval() const {
    EpCore::Interval interval;
    interval.setName(nameLineEdit_->text());
    interval.setMark1(mark1ComboBox_->currentText());
    interval.setMark2(mark2ComboBox_->currentText());
    interval.setWidth(widthSpinBox_->value());
    return interval;
}

void EditIntervalTypeDialog::enableOkButton(const QString& text) {
    buttonBox_->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
}

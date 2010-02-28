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

#include "editcolumnformatdialog.h"

#include "itemlist.h"

#include <QStringListModel>

using EpCore::ColumnFormat;
using EpCore::Interval;
using EpCore::ItemList;
using EpGui::EditColumnFormatDialog;

EditColumnFormatDialog::EditColumnFormatDialog(
        AbstractEditItemsDialog::EditorType type,
        QWidget *parent)
            :  QDialog(parent) {
    setupUi(this);
    QString title = type == AbstractEditItemsDialog::NewItem
                        ? tr("New") : tr("Edit");
    title = tr("%1 Column Format").arg(title);
    setWindowTitle(title);
    connect(selectPushButton, SIGNAL(clicked()), this, SLOT(select()));
    connect(unselectPushButton, SIGNAL(clicked()), this, SLOT(unselect()));
    connect(selectAllPushButton, SIGNAL(clicked()),
            this, SLOT(selectAll()));
    connect(unselectAllPushButton, SIGNAL(clicked()),
            this, SLOT(unselectAll()));
    connect(availableListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(enableSelectButtons()));
    connect(selectedListView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(enableSelectButtons()));
    connect(availableListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(select()));
    connect(selectedListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(unselect()));
    connect(nameLineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(enableOkButton(const QString&)));
    enableSelectButtons();
    enableOkButton(nameLineEdit->text());
}

void EditColumnFormatDialog::select() {
    move(availableListView, selectedListView,
         availableModel_, selectedModel_);
}

void EditColumnFormatDialog::unselect() {
    move(selectedListView, availableListView,
         selectedModel_, availableModel_);
}

void EditColumnFormatDialog::selectAll() {
    availableModel_->setStringList(QStringList());
    selectedModel_->setStringList(ColumnFormat::allIntervalNames());
}

void EditColumnFormatDialog::unselectAll() {
    selectedModel_->setStringList(QStringList());
    availableModel_->setStringList(ColumnFormat::allIntervalNames());
}

void EditColumnFormatDialog::move(QListView* sourceView,
                                  QListView* destView,
                                  QStringListModel* sourceModel,
                                  QStringListModel* destModel) {
    int row = sourceView->currentIndex().row();
    QModelIndex index = sourceModel->index(row);
    QVariant name = index.data();
    sourceModel->removeRows(row, 1);
    row = destView->currentIndex().row();
    destModel->insertRows(++row, 1);
    index = destModel->index(row);
    destView->setCurrentIndex(index);
    destModel->setData(index, name);
    enableSelectButtons();  // moving items can cause items to be selected

}

void EditColumnFormatDialog::setColumnFormat(const EpCore::ColumnFormat& cf) {
    nameLineEdit->setText(cf.name());
    availableModel_ = new QStringListModel(this);
    QStringList unselected =
            ColumnFormat::intervalNames(cf.unselectedIntervals());
    availableModel_->setStringList(unselected);
    availableListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    availableListView->setModel(availableModel_);
    QStringList selected =
            ColumnFormat::intervalNames(cf.selectedIntervals());
    selectedModel_ = new QStringListModel(this);
    selectedModel_->setStringList(selected);
    selectedListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    selectedListView->setModel(selectedModel_);
}

EpCore::ColumnFormat EditColumnFormatDialog::columnFormat() const {
    QList<Interval> intervals;
    QStringList list = selectedModel_->stringList();
    QStringListIterator iter(list);
    ItemList<Interval> allIntervals;
    while (iter.hasNext())
        intervals.append(allIntervals[iter.next()]);
    EpCore::ColumnFormat cf(nameLineEdit->text(), intervals);
    return cf;
}

void EditColumnFormatDialog::enableSelectButtons() {
    bool availableItemIsSelected =
            availableListView->currentIndex().row() > -1;
    bool selectedItemIsSelected =
            selectedListView->currentIndex().row() > -1;
    selectPushButton->setEnabled(availableItemIsSelected);
    unselectPushButton->setEnabled(selectedItemIsSelected);
}

void EditColumnFormatDialog::enableOkButton(const QString& text) {
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
}

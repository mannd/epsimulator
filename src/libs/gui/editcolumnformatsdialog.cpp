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

#include "editcolumnformatsdialog.h"

#include "editcolumnformatdialog.h"

#include <QMessageBox>
#include <QSqlTableModel>

using EpCore::ColumnFormat;
//using EpCore::ItemList;
using EpGui::EditColumnFormatDialog;
using EpGui::EditColumnFormatsDialog;

EditColumnFormatsDialog::EditColumnFormatsDialog(QWidget* parent)
    : QDialog(parent ) {
    setupUi(this);
    setWindowTitle((tr("Column Formats")));
    showCopyButton(true);

    model_ = new QSqlTableModel(this);
    model_->setTable("ColumnFormats");
    model_->select();

    listView->setModel(model_);
    listView->setModelColumn(ColumnFormat_Name);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(removeItem()));
    connect(newButton, SIGNAL(clicked()), this, SLOT(insert()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(edit()));
    connect(listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit()));

    listView->setFocus();

}

void EditColumnFormatsDialog::editItem(EditorType type) {
    QModelIndex index = listView->currentIndex();
    if (type == EditItem && !index.isValid()) {
        selectionIsEmptyWarning();
        return;
    }
    int row = index.row();
    EditColumnFormatDialog d(type, this);
    QString columnFormatName;
    if (type == EditItem) {
        columnFormatName = model_->data(index, Qt::DisplayRole).toString();
        d.setColumnFormat(columnFormatName);
    }
    else if (type == NewItem)
        d.setColumnFormat(QString());
    // if (d.exec()) {
    //     EpCore::ColumnFormat columnFormat = d.columnFormat();
    //     if (itemIsDuplicated(type, columnFormatName, columnFormat,
    //                          columnFormats_))
    //         return;
    //     if (type == NewItem)
    //         columnFormats_.append(d.columnFormat());
    //     else if (type == EditItem)
    //         columnFormats_[d.columnFormat().name()] = d.columnFormat();
    //     createListWidget();
    // }
    d.exec();
}

void EditColumnFormatsDialog::copyItem() {
    QModelIndex index = listView->currentIndex();
    QString name = model_->data(index, Qt::DisplayRole).toString();
    //    ColumnFormat cf = columnFormats_[name];
    name = tr("Copy of %1").arg(name);
    //    cf.setName(name);
    //columnFormats_.append(cf);
    //editCopiedItem(name);
}


void EditColumnFormatsDialog::insert() {
    editItem(NewItem);
}

void EditColumnFormatsDialog::edit() {
    editItem(EditItem);
}

void EditColumnFormatsDialog::removeItem() {
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

void EditColumnFormatsDialog::selectionIsEmptyWarning() {
    QMessageBox::information(this, tr("No Item"),
                             tr("You must select an item first"));
}

// void EditColumnFormatsDialog::editItem(EditorType type) {
//     // QModelIndex index = listView->currentIndex();
//     // if (type == EditItem && !index.isValid()) {
//     //     selectionIsEmptyWarning();
//     //     return;
//     // }
//     // int row = index.row();
//     // EditIntervalTypeDialog d(type, model_, row, this);
//     // d.exec();
// }

void EditColumnFormatsDialog::showCopyButton(bool show) {
    copyButton->setVisible(show);
}


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

#include <QSqlTableModel>

//using EpCore::ColumnFormat;
//using EpCore::ItemList;
using EpGui::AbstractEditItemsDialog;
using EpGui::EditColumnFormatDialog;
using EpGui::EditColumnFormatsDialog;

EditColumnFormatsDialog::EditColumnFormatsDialog(QWidget* parent)
    : AbstractEditItemsDialog(tr("Column Formats"), parent ) {
    showCopyButton(true);

    model_ = new QSqlTableModel(this);
    model_->setTable("ColumnFormats");
    model_->select();

    listView->setModel(model_);
    listView->setModelColumn(ColumnFormat_Name);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setFocus();
}

void EditColumnFormatsDialog::editItem(EditorType type) {
    QModelIndex index = listView->currentIndex();
    if (type == EditItem && !index.isValid()) {
        selectionIsEmptyWarning();
        return;
    }
    //int row = index.row();
    EditColumnFormatDialog d(type, this);
    QString columnFormatName;
    if (type == EditItem) {
        columnFormatName = model_->data(index, Qt::DisplayRole).toString();
        d.setColumnFormat(columnFormatName);
    }
    else if (type == NewItem)
        d.setColumnFormat(QString());
    d.exec();
}

//void EditColumnFormatsDialog::copyItem() {
//    QModelIndex index = listView->currentIndex();
//    QString name = model_->data(index, Qt::DisplayRole).toString();
//    //    ColumnFormat cf = columnFormats_[name];
//    name = tr("Copy of %1").arg(name);
//    //    cf.setName(name);
//    //columnFormats_.append(cf);
//    //editCopiedItem(name);
//}

void EditColumnFormatsDialog::removeItem() {
    AbstractEditItemsDialog::removeItem(model_);
}

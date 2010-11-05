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

using EpCore::ColumnFormat;
using EpCore::ItemList;
using EpGui::EditColumnFormatDialog;
using EpGui::EditColumnFormatsDialog;

EditColumnFormatsDialog::EditColumnFormatsDialog(QWidget* parent)
    : AbstractEditItemsDialog(tr("Column Formats"), parent ){
    showCopyButton(true);

    //createListWidget();
}

void EditColumnFormatsDialog::createListWidget() {
    //createListWidgetItems(columnFormats_);
}

void EditColumnFormatsDialog::removeItem() {
    //removeItemFromList(columnFormats_);
}

void EditColumnFormatsDialog::editItem(EditorType type) {
    // if (type == EditItem && selectionIsEmpty()) {
    //     selectionIsEmptyWarning();
    //     return;
    // }
    // EditColumnFormatDialog d(type, this);
    // QString columnFormatName;
    // if (type == EditItem) {
    //     columnFormatName = listWidget->currentItem()->text();
    //     d.setColumnFormat(columnFormats_[columnFormatName]);
    // }
    // else if (type == NewItem) {
    //     d.setColumnFormat(ColumnFormat());
    // }
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
}

void EditColumnFormatsDialog::copyItem(const QList<QListWidgetItem*>&
                                       selectedItems) {
    // QString name = selectedItems[0]->text();
    // ColumnFormat cf = columnFormats_[name];
    // name = tr("Copy of %1").arg(name);
    // cf.setName(name);
    // columnFormats_.append(cf);
    // editCopiedItem(name);
}


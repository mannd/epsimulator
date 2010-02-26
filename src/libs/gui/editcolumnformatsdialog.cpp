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
    createListWidget();
}

void EditColumnFormatsDialog::createListWidget() {
    listWidget->clear();
    listWidget->setSortingEnabled(true);
    for (int i = 0; i < columnFormats_.size(); ++i) {
        new QListWidgetItem(columnFormats_[i].name(), listWidget);
    }
}

void EditColumnFormatsDialog::removeItem() {
    columnFormats_.remove(listWidget->currentItem()->text());
}

void EditColumnFormatsDialog::editItem(EditorType type) {
    EditColumnFormatDialog d(type, this);
    if (type == EditItem) {
        if (!listWidget->selectedItems().size() > 0) {
            QMessageBox::information(this, tr("No Column Format Selected"),
                                     tr("You must select a column format first"));
            return;
        }
        QString name = listWidget->currentItem()->text();
        d.setColumnFormat(columnFormats_[name]);
    }
    if (d.exec()) {
        EpCore::ColumnFormat columnFormat = d.columnFormat();
        if (columnFormats_.duplicate(columnFormat) && type == NewItem) {
            QMessageBox::information(this, tr("Duplicate Column Format"),
                                    tr("%1 is already present "
                                    "in the list").arg(columnFormat.name()));
            return;
        }
        if (type == NewItem)
            columnFormats_.append(d.columnFormat());
        else    // type == EditItem
            columnFormats_[d.columnFormat().name()] = d.columnFormat();
        createListWidget();
    }
}

void EditColumnFormatsDialog::copyItem(QList<QListWidgetItem*> selectedItems) {
    QString name = tr("Copy of %1").arg(selectedItems[0]->text());
    ColumnFormat cf(name);
    columnFormats_.append(cf);
    createListWidget();
    listWidget->setCurrentItem(listWidget->findItems(name, Qt::MatchExactly)[0]);
    editItem(EditItem);

}


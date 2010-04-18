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

#include "editprotocolsdialog.h"

#include "editprotocoldialog.h"

using EpGui::EditProtocolDialog;
using EpGui::EditProtocolsDialog;

EditProtocolsDialog::EditProtocolsDialog(QWidget* parent)
    : AbstractEditItemsDialog(tr("Protocols"), parent) {
    showCopyButton(true);
    createListWidget();
}

void EditProtocolsDialog::createListWidget() {
    createListWidgetItems(protocols_);
}

void EditProtocolsDialog::removeItem() {
    removeItemFromList(protocols_);
}

/// TODO maybe can templatize duplicate code here and
/// in editcolumnformatsdialog.cpp.
void EditProtocolsDialog::editItem(EditorType type) {
    if (type == EditItem && selectionIsEmpty()) {
        selectionIsEmptyWarning();
        return;
    }
    EditProtocolDialog d(type, this);
    QString protocolName;
    if (type == EditItem) {
        protocolName = listWidget->currentItem()->text();
        d.setProtocol(protocols_[protocolName]);
    }
    if (d.exec()) {
        EpStudy::Protocol protocol = d.protocol();
        if (itemIsDuplicated(type, protocolName, protocol, protocols_))
            return;
        if (type == NewItem)
            protocols_.append(d.protocol());
        else if (type == EditItem)
            protocols_[protocolName] = d.protocol();
        createListWidget();
    }
}

void EditProtocolsDialog::copyItem(const QList<QListWidgetItem*>& selectedItems) {
    QString name = selectedItems[0]->text();
    EpStudy::Protocol protocol = protocols_[name];
    name = tr("Copy of %1").arg(name);
    protocol.setName(name);
    int n = 1;
    QString originalName = name;
    while (protocols_.duplicate(protocol)) {
        name = originalName + QString::number(n++);
        protocol.setName(name);
    }
    protocols_.append(protocol);
    editCopiedItem(name);
}



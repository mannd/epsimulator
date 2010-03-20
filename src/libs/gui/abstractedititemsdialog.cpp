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

#include "abstractedititemsdialog.h"

#include <QMessageBox>

using EpGui::AbstractEditItemsDialog;

AbstractEditItemsDialog::AbstractEditItemsDialog(const QString& title,
                                                 QWidget* parent) :
    QDialog(parent) {
    setupUi(this);
    setWindowTitle(title);

    connect(newButton, SIGNAL(clicked()), this, SLOT(insert()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(edit()));
    connect(copyButton, SIGNAL(clicked()), this, SLOT(copy()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(del()));
    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                               this, SLOT(edit()));
}

void AbstractEditItemsDialog::showCopyButton(bool show) {
    copyButton->setVisible(show);
}

void AbstractEditItemsDialog::insert() {
    editItem(NewItem);
}

void AbstractEditItemsDialog::edit() {
    editItem(EditItem);
}

void AbstractEditItemsDialog::copy() {
    if (selectionIsEmpty())
        return;
    else {
        copyItem(listWidget->selectedItems());
        createListWidget();
    }
}

void AbstractEditItemsDialog::editCopiedItem(const QString& name) {
    createListWidget();
    listWidget->setCurrentItem(listWidget->findItems(name, Qt::MatchExactly)[0]);
    editItem(EditItem);
}

void AbstractEditItemsDialog::del() {
    if (selectionIsEmpty())
        return;
    int result = QMessageBox::warning(this, tr("Delete Item?"),
                         tr("The selected item will be permanently deleted."
                             "Do you wish to continue?"),
                            QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        removeItem();
        createListWidget();
    }
}

bool AbstractEditItemsDialog::selectionIsEmpty() const {
    return listWidget->selectedItems().size() == 0;
}

void AbstractEditItemsDialog::duplicateItemWarning(const QString& name) {
    QMessageBox::information(this, tr("Duplicate Item"),
                            tr("%1 is already present "
                            "in the list").arg(name));
}

void AbstractEditItemsDialog::selectionIsEmptyWarning() {
    QMessageBox::information(this, tr("No Item Selected"),
                             tr("You must select an item first"));
}


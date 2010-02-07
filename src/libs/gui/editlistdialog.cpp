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

#include "editlistdialog.h"

#include <QStringList>
#include <QStringListModel>

using EpGui::EditListDialog;

EditListDialog::EditListDialog(const QStringList& items,
                               const QString& title,
                               const QString& label,
                               QWidget* parent)
                                   : QDialog(parent) {
    setupUi(this);
    setWindowTitle(title);
    listLabel->setText(label);
    model_ = new QStringListModel(this);
    model_->setStringList(items);

    listView->setModel(model_);
    listView->setEditTriggers(QAbstractItemView::AnyKeyPressed
                              | QAbstractItemView::DoubleClicked);

    connect(insertButton, SIGNAL(clicked(bool)),
            this, SLOT(newItem()));
    connect(deleteButton, SIGNAL(clicked(bool)),
            this, SLOT(deleteItem()));
    connect(editButton, SIGNAL(clicked(bool)),
            this, SLOT(editItem()));


}

void EditListDialog::editItem() {
    int row = listView->currentIndex().row();
    QModelIndex index = model_->index(row);
    listView->edit(index);
}

void EditListDialog::newItem() {
    int row = listView->currentIndex().row();
    model_->insertRows(++row, 1);

    QModelIndex index = model_->index(row);
    listView->setCurrentIndex(index);
    listView->edit(index);
}

void EditListDialog::deleteItem() {
    model_->removeRows(listView->currentIndex().row(), 1);
}

QStringList EditListDialog::items() {
    return model_->stringList();
}


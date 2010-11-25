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


#include <QSqlRelationalDelegate>
#include <QSqlTableModel>
#include <QStringList>

#include <QtDebug>

using EpGui::EditListDialog;

EditListDialog::EditListDialog(const QString& table,
                               const QString& title,
                               const QStringList& labels,
                               QWidget* parent)
                                   : QDialog(parent) {
    setupUi(this);
    model_ = new QSqlTableModel(this);
    model_->setTable(table);
    init(title, labels);
}

EditListDialog::EditListDialog(QSqlTableModel* model,
                               const QString& title,
                               const QStringList& labels,
                               QWidget* parent)
                                   : QDialog(parent), model_(model) {
    setupUi(this);
    tableView->setItemDelegate(new QSqlRelationalDelegate(tableView));
    init(title, labels);
}

void EditListDialog::init(const QString& title,
                          const QStringList& labels) {
    setWindowTitle(title);
    model_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_->select();

    tableView->setModel(model_);
    tableView->hideColumn(0);
    tableView->setShowGrid(false);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    for (int i = 0; i < labels.count(); ++i) {
        model_->setHeaderData(i+1, Qt::Horizontal, labels[i]);
    }
    connect(insertButton, SIGNAL(clicked(bool)),
            this, SLOT(newItem()));
    connect(deleteButton, SIGNAL(clicked(bool)),
            this, SLOT(deleteItem()));
    connect(editButton, SIGNAL(clicked(bool)),
            this, SLOT(editItem()));
    connect(allowEditsButton, SIGNAL(clicked(bool)),
            this, SLOT(allowEdits(bool)));
    connect(tableView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(enableButtons()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    allowEdits(false);
    enableButtons();
}

void EditListDialog::accept() {
    model_->submitAll();
    QDialog::accept();
}

void EditListDialog::allowEdits(bool allow) {
    insertButton->setVisible(allow);
    editButton->setVisible(allow);
    deleteButton->setVisible(allow);
    // line is the separator in the toolbar
    line->setVisible(allow);
    if (allow)
        tableView->setEditTriggers(QAbstractItemView::AnyKeyPressed
                                  | QAbstractItemView::DoubleClicked);
    else {
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QModelIndex index = tableView->currentIndex();
        tableView->closePersistentEditor(index);
    }
}

void EditListDialog::enableButtons() {
       bool itemIsSelected = (tableView->currentIndex().row() > -1);
       editButton->setEnabled(itemIsSelected);
       deleteButton->setEnabled(itemIsSelected);

}

void EditListDialog::editItem() {
    QModelIndex index = tableView->currentIndex();
    // prevents error message when edit button hit twice
    tableView->closePersistentEditor(index);
    tableView->edit(index);
}

void EditListDialog::newItem() {
    int row = tableView->currentIndex().row();
    model_->insertRows(++row, 1);
    QModelIndex index = model_->index(row, 1);
    tableView->setCurrentIndex(index);
    tableView->edit(index);
}

void EditListDialog::deleteItem() {
    model_->removeRows(tableView->currentIndex().row(), 1);
}

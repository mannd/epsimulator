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

#include "editsimpletabledialog.h"

#include <QHeaderView>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QTableView>
#include <QListView>
#include <QLayout>
#include <QPushButton>

using EpGui::EditSimpleTableDialog;

EditSimpleTableDialog::EditSimpleTableDialog(QWidget *parent) :
    QDialog(parent) {

//    QSqlQuery query;
//    query.exec("DROP TABLE test");

//    query.exec("CREATE TABLE test ("
//	       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//	       "name VARCHAR(40) NOT NULL)");
//    query.exec("INSERT INTO  test (name) "
//	       "VALUES ('HRA')");
//    query.exec("INSERT INTO  test (name) "
//	       "VALUES ('RVA')");
    


    setWindowTitle("Edit List");
    model_ = new QSqlTableModel(this);
    model_->setTable("test");
    //model_->setHeaderData(1, Qt::Horizontal, tr("Name"));
    //model_->removeColumn(0);
    model_->select();

    QListView* view = new QListView(this);
    view->setModel(model_);
    view->setModelColumn(1);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectItems);
    // view->setColumnHidden(0, true);
    // view->resizeColumnsToContents();
    view->setEditTriggers(QAbstractItemView::DoubleClicked |
                          QAbstractItemView::AnyKeyPressed);

    // QHeaderView* header = view->horizontalHeader();
    // header->setStretchLastSection(true);
    // header->setDefaultAlignment(Qt::AlignLeft);

    // QHeaderView* verticalHeader = view->verticalHeader();
    // verticalHeader->hide();

    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addWidget(view);
    QPushButton* p = new QPushButton("Commit");
    mainLayout->addWidget(p);
    setLayout(mainLayout);

    connect(p, SIGNAL(clicked()), this, SLOT(commit()));
}

void EditSimpleTableDialog::commit() {
    model_->submitAll();
}

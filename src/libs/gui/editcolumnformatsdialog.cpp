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

#include "columnformat.h"
#include "editcolumnformatdialog.h"
#include "interval.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

#include <QtDebug>

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
    if (type == EditItem && selectionIsEmpty()) {
        selectionIsEmptyWarning();
        return;
    }
    QModelIndex index = listView->currentIndex();
    QSqlRecord record = model_->record(index.row());
    int id = record.value(ColumnFormat_Id).toInt();
    QSqlQuery query(QString("SELECT Intervals.Name FROM ColumnFormats "
                    "JOIN ColumnFormatInterval USING (ColumnFormatID) "
                    "JOIN Intervals USING (IntervalID) "
                    "WHERE ColumnFormatInterval.ColumnFormatID = %1 "
                              "ORDER BY SortOrder").arg(id));
    QList<EpCore::Interval> intervals;
    while (query.next()) {
        QString value = query.value(0).toString();
        intervals.append(value);
    }
    EditColumnFormatDialog d(type, this);
    if (type == EditItem) {
        QString columnFormatName = record.value(ColumnFormat_Name).toString();
        EpCore::ColumnFormat columnFormat(columnFormatName, intervals);
        d.setColumnFormat(columnFormat);
    }
    else if (type == NewItem) {
        EpCore::ColumnFormat columnFormat;
        d.setColumnFormat(columnFormat);
    }
    if (d.exec() == QDialog::Accepted) {
        EpCore::ColumnFormat columnFormat = d.columnFormat();
        if (type == NewItem) {
            int row = index.row();
            model_->insertRows(row, 1);
            model_->setData(model_->index(row, ColumnFormat_Name), columnFormat.name());

            model_->submitAll();
            model_->select();
            QSqlTableModel updateModel;
            updateModel.setTable("ColumnFormats");
            updateModel.setFilter(QString("Name = '%1'").arg(columnFormat.name()));
            updateModel.select();
            int newId = 0;
            QString newName;
            if (updateModel.rowCount() >= 1) {
                QSqlRecord record = updateModel.record(0);
                newId = record.value(0).toInt();
                newName = record.value(1).toString();
            }
            QList<EpCore::Interval> intervals = columnFormat.intervals();
            QListIterator<EpCore::Interval> iter(intervals);
            int i = 0;
            QSqlDatabase::database().transaction();
            while (iter.hasNext()) {
                QSqlQuery lookup(QString("SELECT IntervalID FROM Intervals "
                                         "WHERE Name = '%1'").arg(iter.next().name()));
                int lookupId;
                while (lookup.next()) {
                    lookupId = lookup.value(0).toInt();
                    QSqlQuery insertQuery(QString("INSERT INTO ColumnFormatInterval "
                                                  "(ColumnFormatID, IntervalID, SortOrder) "
                                                  "VALUES (%1, %2, %3)")
                                          .arg(newId).arg(lookupId).arg(i++));
                }
            }
            QSqlDatabase::database().commit();
        }
    }
}

void EditColumnFormatsDialog::copyItem() {
   QModelIndex index = listView->currentIndex();
   QSqlRecord record = model_->record(index.row());
   QString name = record.value(ColumnFormat_Name).toString();
   int id = record.value(ColumnFormat_Id).toInt();
   name = tr("Copy of %1").arg(name);
   int lastRow = model_->rowCount();
   model_->insertRow(lastRow);
   index = model_->index(lastRow, ColumnFormat_Name);
   listView->setCurrentIndex(index);
   model_->setData(index, name);
   model_->submitAll();
   QSqlRecord newRecord = model_->record(index.row());
   int newId = newRecord.value(0).toInt();
   qDebug() << id;
   qDebug() << newId;
   QSqlQuery query(QString("SELECT * FROM ColumnFormatInterval "
                           "WHERE ColumnFormatInterval.ColumnFormatID = %1 ").arg(id));
   QSqlQuery insertQuery;
   QSqlDatabase::database().transaction();
   while (query.next()) {
       insertQuery.exec(QString("INSERT INTO ColumnFormatInterval "
                                 "(ColumnFormatID, IntervalID, SortOrder) "
                                "VALUES (%1, %2, %3)").arg(newId).arg(query.value(1).toInt())
                        .arg(query.value(2).toInt()));
       
   }
   QSqlDatabase::database().commit();
}

void EditColumnFormatsDialog::removeItem() {
    QSqlDatabase::database().transaction();
    QModelIndex index = listView->currentIndex();
    QSqlRecord record = model_->record(index.row());
    int id = record.value(ColumnFormat_Id).toInt();
    QSqlQuery query;
    query.exec(QString("DELETE FROM ColumnFormatInterval WHERE ColumnFormatID = %1").arg(id));
    QSqlDatabase::database().commit();
    AbstractEditItemsDialog::removeItem(model_);
}

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

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelation>
#include <QSqlRelationalTableModel>

#include <QtDebug>

using EpGui::EditProtocolDialog;
using EpGui::EditProtocolsDialog;

EditProtocolsDialog::EditProtocolsDialog(QWidget* parent)
    : AbstractEditItemsDialog(tr("Protocols"), parent) {
    showCopyButton(true);

    model_ = new QSqlRelationalTableModel(this);
    model_->setTable("Protocols");
    model_->setRelation(Protocol_SenseChannelLabelID,
                        QSqlRelation("ChannelLabels",
                                     "ChannelLabelID",
                                     "Name"));
    model_->setRelation(Protocol_ColumnFormatID,
                        QSqlRelation("ColumnFormats",
                                     "ColumnFormatID",
                                     "Name"));
    model_->setRelation(Protocol_WindowSettingID,
                        QSqlRelation("WindowSettings",
                                     "WindowSettingID",
                                     "Name"));
    model_->setRelation(Protocol_MacroCategoryID,
                        QSqlRelation("MacroCategories",
                                     "MacroCategoryID",
                                     "Name"));
    model_->setRelation(Protocol_UpdateReviewWindowID,
                        QSqlRelation("OnOff",
                                     "OnOffID",
                                     "Name"));
    model_->setRelation(Protocol_FocalPointID,
                        QSqlRelation("FocalPoints",
                                     "FocalPointID",
                                     "Name"));
    model_->setRelation(Protocol_DisplayPageID,
                        QSqlRelation("DisplayPages",
                                     "DisplayPageID",
                                     "Name"));

    model_->select();

    listView->setModel(model_);
    listView->setModelColumn(Protocol_Name);
}

void EditProtocolsDialog::removeItem() {
    AbstractEditItemsDialog::removeItem(model_);
}

void EditProtocolsDialog::editItem(EditorType type) {
    if (type == EditItem && selectionIsEmpty()) {
        selectionIsEmptyWarning();
        return;
    }
    QModelIndex index = listView->currentIndex();
    int row = index.row();
    EditProtocolDialog d(type, model_, row, this);
    d.exec();
}

void EditProtocolsDialog::copyItem() {
    // Don't use the model here, it doesn't give you
    // the foreign keys you need to do this!
    QModelIndex index = listView->currentIndex();
    if (!index.isValid())
        return;
    QSqlRecord record = model_->record(index.row());
    int id = record.value(Protocol_Id).toInt();
    QSqlQuery query(QString("SELECT * FROM Protocols WHERE ProtocolID "
                            "= %1").arg(id));
    while (query.next()) {
        QString name = query.value(Protocol_Name).toString();
        name = tr("Copy of %1").arg(name);
        int senseChannelLabelID = query.value(Protocol_SenseChannelLabelID).toInt();
        int columnFormatID = query.value(Protocol_ColumnFormatID).toInt();
        int windowSettingID = query.value(Protocol_WindowSettingID).toInt();
        int macroCategoryID = query.value(Protocol_MacroCategoryID).toInt();
        int updateReviewWindowID = query.value(Protocol_UpdateReviewWindowID).toInt();
        int focalPointID = query.value(Protocol_FocalPointID).toInt();
        int displayPageID = query.value(Protocol_DisplayPageID).toInt();
        QSqlQuery insertQuery(QString("INSERT INTO Protocols (Name, SenseChannelLabelID, "
                                      "ColumnFormatID, WindowSettingID, "
                                      "MacroCategoryID, UpdateReviewWindowID, "
                                      "FocalPointID, DisplayPageID) VALUES "
                                      "('%1', %2, %3, %4, %5, %6, %7, %8)")
                              .arg(name).arg(senseChannelLabelID).arg(columnFormatID)
                              .arg(windowSettingID).arg(macroCategoryID)
                              .arg(updateReviewWindowID).arg(focalPointID)
                              .arg(displayPageID));
        Q_ASSERT(insertQuery.isActive());
    }
    model_->select();
}

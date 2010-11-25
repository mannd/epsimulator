/***************************************************************************
 *   Copyright (C) %YEAR% by EP Studios, Inc.                                *
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

#include "editprotocoldialog.h"

#include "editprotocolsdialog.h"

#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>

using EpGui::EditProtocolDialog;

EditProtocolDialog::EditProtocolDialog(AbstractEditItemsDialog::EditorType type,
                                       QSqlRelationalTableModel* model,
                                       int row,
                                       QWidget *parent)
    : QDialog(parent){
    setupUi(this);
    QString title = type == AbstractEditItemsDialog::NewItem
        ? tr("New") : tr("Edit");
    title = tr("%1 Protocol").arg(title);
    setWindowTitle(title);

    mapper_ = new QDataWidgetMapper(this);
    mapper_->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper_->setModel(model);
    mapper_->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper_->addMapping(nameLineEdit, EditProtocolsDialog::Protocol_Name);
    QSqlTableModel* senseChannelLabelModel =
        model->relationModel(EditProtocolsDialog::Protocol_SenseChannelLabelID);
    senseChannelLabelComboBox->setModel(senseChannelLabelModel);
    senseChannelLabelComboBox->setModelColumn(senseChannelLabelModel->
                                              fieldIndex("Name"));
    mapper_->addMapping(senseChannelLabelComboBox, 
                        EditProtocolsDialog::Protocol_SenseChannelLabelID);
    QSqlTableModel* columnFormatModel =
        model->relationModel(EditProtocolsDialog::Protocol_ColumnFormatID);
    columnFormatComboBox->setModel(columnFormatModel);
    columnFormatComboBox->setModelColumn(columnFormatModel->fieldIndex("Name"));
    mapper_->addMapping(columnFormatComboBox,
                        EditProtocolsDialog::Protocol_ColumnFormatID);
    QSqlTableModel* windowSettingModel =
        model->relationModel(EditProtocolsDialog::Protocol_WindowSettingID);
    windowSettingComboBox->setModel(windowSettingModel);
    windowSettingComboBox->setModelColumn(windowSettingModel->fieldIndex("Name"));
    mapper_->addMapping(windowSettingComboBox,
                        EditProtocolsDialog::Protocol_WindowSettingID);
    QSqlTableModel* macroCategoryModel =
        model->relationModel(EditProtocolsDialog::Protocol_MacroCategoryID);
    macroCategoryComboBox->setModel(macroCategoryModel);
    macroCategoryComboBox->setModelColumn(macroCategoryModel->fieldIndex("Name"));
    mapper_->addMapping(macroCategoryComboBox,
                        EditProtocolsDialog::Protocol_MacroCategoryID);
    QSqlTableModel* updateReviewModel = 
        model->relationModel(EditProtocolsDialog::Protocol_UpdateReviewWindowID);
    updateReviewComboBox->setModel(updateReviewModel);
    updateReviewComboBox->setModelColumn(updateReviewModel->fieldIndex("Name"));
    mapper_->addMapping(updateReviewComboBox, 
                        EditProtocolsDialog::Protocol_UpdateReviewWindowID);
    QSqlTableModel* focalPointModel = 
        model->relationModel(EditProtocolsDialog::Protocol_FocalPointID);
    focalPointComboBox->setModel(focalPointModel);
    focalPointComboBox->setModelColumn(focalPointModel->fieldIndex("Name"));
    mapper_->addMapping(focalPointComboBox, 
                        EditProtocolsDialog::Protocol_FocalPointID);
    QSqlTableModel* displayPageModel = 
        model->relationModel(EditProtocolsDialog::Protocol_DisplayPageID);
    displayPageComboBox->setModel(displayPageModel);
    displayPageComboBox->setModelColumn(displayPageModel->fieldIndex("Name"));
    mapper_->addMapping(displayPageComboBox, 
                        EditProtocolsDialog::Protocol_DisplayPageID);
    mapper_->setCurrentIndex(row);

    connect(nameLineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(enableOkButton()));
    buttonBox->button(QDialogButtonBox::Ok)->setDefault(false);
    enableOkButton();
}

void EditProtocolDialog::accept() {
    mapper_->submit();
    QDialog::accept();
}

void EditProtocolDialog::enableOkButton() {
    buttonBox->button(QDialogButtonBox::Ok)->
        setEnabled(!nameLineEdit->text().isEmpty());
}

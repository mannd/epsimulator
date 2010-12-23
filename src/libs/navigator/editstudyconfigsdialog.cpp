/***************************************************************************
 *   Copyright (C) 2006 by EP Studios, Inc.                                *
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

#include "editstudyconfigsdialog.h"

// #include "options.h"
// #include "user.h"
#include "studyconfiguration.h"
#include "studyconfigurationdialog.h"

#include <QInputDialog>
#include <QList>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlRecord>
#include <QSqlTableModel>

// using EpCore::Options;
// using EpCore::User;
using EpNavigator::EditStudyConfigsDialog;
using EpStudy::StudyConfiguration;
// using EpStudy::StudyConfigurations;
using EpGui::AbstractEditItemsDialog;
using EpGui::RealTimeStudyConfigurationDialog;

EditStudyConfigsDialog::EditStudyConfigsDialog(QWidget *parent)
    : AbstractEditItemsDialog(tr("Study Configurations"), parent) {
    showCopyButton(true);

    model_ = new QSqlTableModel(this);
    model_->setTable("StudyConfigurations");
    model_->select();
    listView->setModel(model_);
    listView->setModelColumn(StudyConfiguration_Name);
}


void EditStudyConfigsDialog::copyItem() {
    QModelIndex index = listView->currentIndex();
    QSqlRecord record = model_->record(index.row());
    QString name = record.value(StudyConfiguration_Name).toString();
    int id = record.value(StudyConfiguration_Id).toInt();
    name = tr("Copy of %1").arg(name);
    int lastRow = model_->rowCount();
    model_->insertRow(lastRow);
    index = model_->index(lastRow, StudyConfiguration_Name);
    listView->setCurrentIndex(index);
    model_->setData(index, name);
    model_->submitAll();
}

void EditStudyConfigsDialog::removeItem() {
    AbstractEditItemsDialog::removeItem(model_);
};

void EditStudyConfigsDialog::editItem(EditorType type) {
    if (type == EditItem && selectionIsEmpty()) {
        selectionIsEmptyWarning();
        return;
    }
    QModelIndex index = listView->currentIndex();
    if (type == NewItem) {
            StudyConfiguration* config = new StudyConfiguration;
            RealTimeStudyConfigurationDialog d(config, true, this);
            d.exec();
    }
}







// void EditStudyConfigsDialog::newStudyConfig() {
//     bool ok = false;
//     QString name;
//     name =  QInputDialog::getText(this,
//                                   tr("Enter New Study Configuration Name"),
//                                   tr("Study configuration name:"),
//                                   QLineEdit::Normal,
//                                   name, &ok);
//     if (!ok || name.isEmpty())
//         return;
//     // search for duplicate study configuration name -- Not Allowed!
//     if (configList_.isPresent(name)) {
//         QMessageBox::warning(this,
//                              tr("Duplicate Study Configuration Name"),
//                              tr("Study configuration name %1 already exists. "
//                                 "Select Edit if you wish to "
//                                 "change an existing study configuration")
//                              .arg(name));

//         return;
//     }
//     StudyConfiguration config(name);
//     configList_.add(config);
//     RealTimeStudyConfigurationDialog d(&config, true, this); /// FIXME
//     d.exec();
//     // The study configuration dialog writes changes to disk, but
//     // doesn't affect configList_, so refresh from disk.
//     // Need to do this here because user could do a few "Save As"
//     // clicks and create more study configurations from the dialog.
//     /// FIXME
//     //configList_.refresh();
//     createConfigListWidget();
// }

// void EditStudyConfigsDialog::editStudyConfig() {
//     if (configListWidget->selectedItems().size() == 0) {
//         noSelectionError();
//         return;
//     }
//     QString configName = configListWidget->currentItem()->text();
//     StudyConfiguration* config = new StudyConfiguration(
//             *configList_.studyConfiguration(configName));
//     RealTimeStudyConfigurationDialog d(config, true, this);  ///FIXME
//     d.exec();
//     delete config;
//     // The study configuration dialog writes changes to disk, but
//     // doesn't affect configList_, so refresh from disk.
//     /// FIXME
//     //configList_.refresh();
//     createConfigListWidget();
// }

// void EditStudyConfigsDialog::copyStudyConfig() {
//     if (configListWidget->selectedItems().size() == 0) {
//         noSelectionError();
//         return;
//     }
//     QString originalName = configListWidget->currentItem()->text();
//     QString newName = tr("Copy of %1").arg(originalName);
//     bool ok = false;
//     newName =
//             QInputDialog::getText(this,
//                                   tr("Enter New Study Configuration Name"),
//                                   tr("Study configuration name:"),
//                                   QLineEdit::Normal,
//                                   newName, &ok);
//     if (ok && !newName.isEmpty()) {
//         // search for duplicate study configuration name -- Not Allowed!
//         if (configList_.isPresent(newName)) {
//             QMessageBox::warning(this,
//                                  tr("Duplicate Study Configuration Name"),
//                                  tr("Study configuration name %1 already exists. "
//                                     "Select <b>Edit</b> if you wish to "
//                                     "change an existing study configuration")
//                                  .arg(newName));

//             return;
//         }
//         StudyConfiguration config(*configList_.studyConfiguration(originalName));
//         config.setName(newName);
//         // commit to keeping copy here, even if Close button used.
//         configList_.add(config);
//         RealTimeStudyConfigurationDialog d(&config, true, this);///FIXME
//         d.exec();
//         // The study configuration dialog writes changes to disk, but
//         // doesn't affect configList_, so refresh from disk.
//         // Need to do this here because user could do a few "Save As"
//         // clicks and create more study configurations from the dialog.
//         /// FIXME
//         //configList_.refresh();
//         createConfigListWidget();
//     }
// }

// void EditStudyConfigsDialog::deleteStudyConfig() {
//     if (configListWidget->selectedItems().size() == 0) {
//         noSelectionError();
//         return;
//     }
//     int result = QMessageBox::warning(this, tr("Delete Study Configuration?"),
//                          tr("Really delete this study configuration? "
//                             "Deleting will be permanent."),
//                             QMessageBox::Yes | QMessageBox::No);
//     if (result == QMessageBox::Yes) {
//         configList_.remove(configListWidget->
//                            currentItem()->text());
//         createConfigListWidget();

//     }
// }

// void EditStudyConfigsDialog::noSelectionError() {
//     QMessageBox::warning(this, tr("No Study Configuration Selected"),
//                          tr("You must select a study configuration "
//                             "first to do this."));
// }


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

#include "studyconfiguration.h"

#include <QInputDialog>
#include <QList>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>

using EpNavigator::EditStudyConfigsDialog;
using EpStudy::StudyConfiguration;
using EpStudy::StudyConfigurations;

EditStudyConfigsDialog::EditStudyConfigsDialog(QWidget *parent)
    : QDialog(parent) {
    setupUi(this);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(configListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(editStudyConfig()));
    connect(newButton, SIGNAL(clicked()), this, SLOT(newStudyConfig()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editStudyConfig()));
    connect(copyButton, SIGNAL(clicked()), this, SLOT(copyStudyConfig()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteStudyConfig()));

    createConfigListWidget();
}

void EditStudyConfigsDialog::createConfigListWidget() {
    configListWidget->setSortingEnabled(true);
    configListWidget->clear();
    for (int i = 0; i < configList_.size(); ++i)
        new QListWidgetItem(configList_[i].name(), configListWidget);
}

void EditStudyConfigsDialog::newStudyConfig() {

}

void EditStudyConfigsDialog::editStudyConfig() {
    if (configListWidget->selectedItems().size() == 0) {
        noSelectionError();
        return;
    }
    // what kind of study configuration is used here????

}

void EditStudyConfigsDialog::copyStudyConfig() {
    if (configListWidget->selectedItems().size() == 0) {
        noSelectionError();
        return;
    }
    QString configName = configListWidget->currentItem()->text();
    bool ok = false;
    QString text =
            QInputDialog::getText(this,
                                  tr("Enter New Study Configuration Name"),
                                  tr("Study configuration name:"),
                                  QLineEdit::Normal,
                                  configName, &ok);
    if (ok && !text.isEmpty()) {
        // search for duplicate study configuration name -- Not Allowed!
        if (configList_.isPresent(text)) {
            QMessageBox::warning(this,
                                 tr("Duplicate Study Configuration Name"),
                                 tr("Study configuration name already exists. "
                                    "Select <b>Edit</b> if you wish to "
                                    "change an existing study configuration"));

            return;
        }
        StudyConfiguration config(*configList_.studyConfiguration(configName));
        config.setName(text);
        configList_.add(config);
        // edit it here
        createConfigListWidget();
    }
}

void EditStudyConfigsDialog::deleteStudyConfig() {
    if (configListWidget->selectedItems().size() == 0) {
        noSelectionError();
        return;
    }
    int result = QMessageBox::warning(this, tr("Delete Study Configuration?"),
                         tr("Really delete this study configuration? "
                            "Deleting will be permanent."),
                            QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        configList_.remove(configListWidget->
                           currentItem()->text());
        createConfigListWidget();

    }
}

void EditStudyConfigsDialog::noSelectionError() {
    QMessageBox::warning(this, tr("No Study Configuration Selected"),
                         tr("You must select a study configuration "
                            "first to do this."));
}


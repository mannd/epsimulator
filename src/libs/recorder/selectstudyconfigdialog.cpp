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

#include "selectstudyconfigdialog.h"

#include <QList>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSqlTableModel>

using EpGui::SelectStudyConfigDialog;
using EpStudy::StudyConfiguration;
using EpStudy::StudyConfigurations;  // delete after refactoring

SelectStudyConfigDialog::SelectStudyConfigDialog(QWidget *parent)
    : QDialog(parent) {
    setupUi(this);
    enableOkButton();

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(configListView, SIGNAL(itemClicked(QListWidgetItem*)),
        this, SLOT(enableOkButton()));
    connect(configListView, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(accept()));

    //configListView->setSortingEnabled(true);

    model_ = new QSqlTableModel(this);
    model_->setTable("StudyConfigurations");
    model_->select();
    configListView->setModel(model_);
    configListView->setModelColumn(StudyConfiguration_Name);

    /*for (int i = 0; i < configList_.size(); ++i)
        new QListWidgetItem(configList_[i].name(), configListWidget)*/;
}

SelectStudyConfigDialog::~SelectStudyConfigDialog() {}

void SelectStudyConfigDialog::enableOkButton() {
//    buttonBox->button(QDialogButtonBox::Ok)->
//        setEnabled(configListView->selectedItems().size() > 0);
}

// returns StudyConfiguration* selected, or 0 if none selected
StudyConfiguration* SelectStudyConfigDialog::studyConfiguration() {
    QModelIndex index = configListView->currentIndex();
    if (!index.isValid())
        return 0;
    StudyConfiguration* s = new StudyConfiguration;
    return s;
}


void SelectStudyConfigDialog::setStudyConfiguration(
        const StudyConfiguration& s) {
//    int n;
//    if ((n = configList_.index(s.name())) > -1) {
//        configListView->setCurrentRow(n);
//        enableOkButton();
//    }
}

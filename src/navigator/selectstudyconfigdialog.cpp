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

SelectStudyConfigDialog::SelectStudyConfigDialog(QWidget *parent)
    : QDialog(parent) {
    setupUi(this);
    connect(configListWidget, SIGNAL(itemActivated(QListWidgetItem*)), 
        this, SLOT(enableOkButton()));
    connect(configListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(accept()));
    new QListWidgetItem(tr("<default>"), configListWidget);
    /// TODO read list of study configurations from 
    /// file in System dir.
}

void SelectStudyConfigDialog::enableOkButton() {
    okButton->setEnabled(configListWidget->currentItem());
}

SelectStudyConfigDialog::~SelectStudyConfigDialog() {}

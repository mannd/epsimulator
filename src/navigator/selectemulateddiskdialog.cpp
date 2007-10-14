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
#include "selectemulateddiskdialog.h"

#include <q3listbox.h>
#include <qpushbutton.h>
#include <qstringlist.h>

SelectEmulatedDiskDialog::SelectEmulatedDiskDialog(QWidget *parent)
    : QDialog(parent), select_(false),
      new_(false), flip_(false) {
    enableButtons();
}

void SelectEmulatedDiskDialog::setLabelList(const QStringList& stringList) {
    labelListBox->insertStringList(stringList);
}

void SelectEmulatedDiskDialog::toggleFlipDisk(bool flip) {
    flipDiskPushButton->setEnabled(flip);
}

void SelectEmulatedDiskDialog::enableButtons() {
    selectPushButton->setEnabled(labelListBox->selectedItem());
    flipDiskPushButton->setEnabled(labelListBox->selectedItem());
}




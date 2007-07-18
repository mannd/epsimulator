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


#include "disklabeldialog.h"

#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qradiobutton.h>

DiskLabelDialog::DiskLabelDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
: DiskLabelDialogBase(parent,name, modal,fl)
{
}

QString DiskLabelDialog::label() const {
    return diskLabelLineEdit->text();
}

QString DiskLabelDialog::side() const {
    if (noneButton->isChecked()) 
        return QString::null;
    return sideAButton->isChecked() ? "A" : "B";
}

void DiskLabelDialog::setLabel(const QString& label) {
    diskLabelLineEdit->setText(label);
}

void DiskLabelDialog::setSide(const QString& side) {
    if (side.isEmpty())
        noneButton->setChecked(true);
    else if (side == "A")
        sideAButton->setChecked(true);
    else
        sideBButton->setChecked(true);
}

void DiskLabelDialog::enableSideButtons(bool enable) {
    diskSideButtonGroup->setEnabled(enable);
}

void DiskLabelDialog::enableNoneButton(bool enable) {
    noneButton->setEnabled(enable);
}

DiskLabelDialog::~DiskLabelDialog()
{
}

/*$SPECIALIZATION$*/



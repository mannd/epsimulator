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

#include <q3buttongroup.h>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>

DiskLabelDialog::DiskLabelDialog(QWidget* parent)
: QDialog(parent) {
    setupUi(this);
    // set Ok button to correct state at the onset
    on_diskLabelLineEdit_textChanged();

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QString DiskLabelDialog::label() const {
    return diskLabelLineEdit->text();
}

QString DiskLabelDialog::side() const {
    if (noneButton->isChecked()) 
        return QString();
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

void DiskLabelDialog::on_diskLabelLineEdit_textChanged() {
   buttonBox->button(QDialogButtonBox::Ok)->
        setEnabled(!diskLabelLineEdit->text().isEmpty());
}

void DiskLabelDialog::accept() {
    if (diskLabelLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Missing Information"),
                            tr("Disk label can't be blank.  Please "
                               "enter it or cancel." ));    
        diskLabelLineEdit->setFocus();  
    }
    else 
        QDialog::accept();
}

DiskLabelDialog::~DiskLabelDialog()
{
}



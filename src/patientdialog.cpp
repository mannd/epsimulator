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
#include "patientdialog.h"

#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qstring.h>

#include <math.h>

PatientDialog::PatientDialog(QWidget *parent, const char *name)
    :PatientDialogBase(parent, name) {
//    heightLineEdit->
}

double PatientDialog::inchesToCentimeters(double inches) {
    return inches * 2.54;
}

double PatientDialog::poundsToKilograms(double pounds) {
    return pounds * 0.45;
}

void PatientDialog::manualEditBsaCheckBox_toggled(bool checked) {
    bsaLineEdit->setEnabled(checked);
}

void PatientDialog::heightLineEdit_returnPressed() {
    setBsaText();
}

void PatientDialog::weightLineEdit_returnPressed() {
    setBsaText();
}

double PatientDialog::bsa() const {
   return sqrt((heightLineEdit->text().toDouble() * weightLineEdit->text().toDouble())/3600);
}

void PatientDialog::setBsaText() {
    if (!manualEditBsaCheckBox->isChecked())
        bsaLineEdit->setText(QString::number(bsa()));
}

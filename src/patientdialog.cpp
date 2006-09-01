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

void PatientDialog::setBsaText() {
    if (!manualEditBsaCheckBox->isChecked())
        bsaLineEdit->setText(QString::number(bsa()));
}

void PatientDialog::heightLineEdit_lostFocus() {
    setBsaText();
}

void PatientDialog::weightLineEdit_lostFocus() {
    setBsaText();
}

double PatientDialog::bsa() {
    double height = heightLineEdit->text().toDouble();
    if (heightUnitsComboBox->currentItem() == 0)    // inches
        height = inchesToCentimeters(height);
    double weight = weightLineEdit->text().toDouble();
    if (weightUnitsComboBox->currentItem() == 0)    // poundsetBsaText()
        weight = poundsToKilograms(weight);
    return sqrt(height * weight) / 3600;
}

void PatientDialog::weightUnitsComboBox_textChanged(const QString&) {
    setBsaText();
}

void PatientDialog::heightUnitsComboBox_textChanged(const QString&) {
    setBsaText();
}






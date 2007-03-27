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

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdatetime.h>
#include <qdatetimeedit.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qstring.h>

#include <math.h>

PatientDialog::PatientDialog(QWidget* parent, const char* name)
    : PatientDialogBase(parent, name) {
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

double PatientDialog::bsa() {
    double height = heightCmLineEdit->text().toDouble();
    double weight = weightKgLineEdit->text().toDouble();
    return sqrt(height * weight) / 3600;
}

void PatientDialog::heightInLineEdit_lostFocus() {
    heightCmLineEdit->setText(QString::number(inchesToCentimeters(heightInLineEdit->text().toDouble())));
    setBsaText();
}

void PatientDialog::weightLbsLineEdit_lostFocus() {
    weightKgLineEdit->setText(QString::number(poundsToKilograms(weightLbsLineEdit->text().toDouble())));
    setBsaText();
}

void PatientDialog::heightCmLineEdit_lostFocus() {
    setBsaText();
}

void PatientDialog::weightKgLineEdit_lostFocus() {
    setBsaText();
}

void PatientDialog::setFields(const Study* study) {
    Name name = study->name();
    lastNameLineEdit->setText(name.last);
    firstNameLineEdit->setText(name.first);
    middleNameLineEdit->setText(name.middle);
    mrnLineEdit->setText(study->mrn());
    sexComboBox->setCurrentItem(study->sex());
    studyDateTimeEdit->setDateTime(study->dateTime());
    studyNumberLineEdit->setText(study->number());
    dobDateEdit->setDate(study->dateOfBirth());
    sexComboBox->setCurrentItem(study->sex());
    heightInLineEdit->setText(QString::number(study->heightIn()));
    weightLbsLineEdit->setText(QString::number(study->weightLbs()));
    heightCmLineEdit->setText(QString::number(study->height()));
    weightKgLineEdit->setText(QString::number(study->weight()));
    manualEditBsaCheckBox->setChecked(study->bsaManualEdit());
    bsaLineEdit->setText(QString::number(study->bsa()));
    efSpinBox->setValue(study->ef());
    vagalToneSpinBox->setValue(study->vagalTone());
    sympatheticToneSpinBox->setValue(study->sympatheticTone());
    ischemiaCheckBox->setChecked(study->ischemia());
    // handle heart
}

void PatientDialog::getFields(Study* study) {
    Name name;
    name.last = lastNameLineEdit->text();
    name.first = firstNameLineEdit->text();
    name.middle = middleNameLineEdit->text();
    study->setName(name);
    study->setMrn(mrnLineEdit->text());
    study->setDateTime(studyDateTimeEdit->dateTime());
    study->setNumber(studyNumberLineEdit->text());
    study->setDateOfBirth(dobDateEdit->date());
    study->setSex(getSex());
    study->setHeight(heightCmLineEdit->text().toDouble());
    study->setWeight(weightKgLineEdit->text().toDouble()); 
    study->setHeightIn(heightInLineEdit->text().toDouble());
    study->setWeightLbs(weightLbsLineEdit->text().toDouble());
    study->setBsaManualEdit(manualEditBsaCheckBox->isChecked());
    study->setBsa(bsaLineEdit->text().toDouble());
    study->setEf(efSpinBox->value());
    study->setVagalTone(vagalToneSpinBox->value());
    study->setSympatheticTone(sympatheticToneSpinBox->value());
    study->setIschemia(ischemiaCheckBox->isChecked());
    // handle heart here
}

Sex PatientDialog::getSex() {
    return sexComboBox->currentItem() == 0 ? Male : Female;
}

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
#include <q3combobox.h>
#include <qdatetime.h>
#include <q3datetimeedit.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qspinbox.h>
#include <qstring.h>

#include <math.h>

PatientDialog::PatientDialog(QWidget* parent)
    : QDialog(parent), metricHeight_(0.0),
    metricWeight_(0.0) {
    setupUi(this);
    connect(manualEditBsaCheckBox, SIGNAL(stateChanged(int)),
        this, SLOT(manualEditBsaCheckBox_toggled(int)));
    connect(heightInLineEdit, SIGNAL(lostFocus()),
        this, SLOT(heightInLineEdit_lostFocus()));
    connect(heightCmLineEdit, SIGNAL(lostFocus()),
        this, SLOT(heightCmLineEdit_lostFocus()));
    connect(weightLbsLineEdit, SIGNAL(lostFocus()),
        this, SLOT(weightLbsLineEdit_lostFocus()));
    connect(weightKgLineEdit, SIGNAL(lostFocus()),
        this, SLOT(weightKgLineEdit_lostFocus()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void PatientDialog::accept() {
    if (lastNameLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Missing Information"),
                            tr("Last Name can't be blank.  Please "
                               "enter it or cancel." ));    
        lastNameLineEdit->setFocus();  
    }
    else 
        QDialog::accept();
}

double PatientDialog::inchesToCentimeters(double inches) const {
    return inches * 2.54;
}

double PatientDialog::centimetersToInches(double cm) const {
    return cm / 2.54;
}

double PatientDialog::poundsToKilograms(double pounds) const {
    return pounds * 0.45359237;
}

double PatientDialog::kilogramsToPounds(double kg) const {
    return kg / 0.45359237;
}

void PatientDialog::manualEditBsaCheckBox_toggled(int state) {
    bsaLineEdit->setEnabled(state == Qt::Checked);
}

void PatientDialog::setBsaText() {
    if (!manualEditBsaCheckBox->isChecked())
        bsaLineEdit->setText(QString::number(bsa()));
}

double PatientDialog::bsa() {
    double height = heightCmLineEdit->text().toDouble();
    double weight = weightKgLineEdit->text().toDouble();
    // Mosteller formula
    // see http://www.halls.md/body-surface-area/refs.htm
    return sqrt((height * weight) / 3600);
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

void PatientDialog::getFields(Study* study) const {
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

Sex PatientDialog::getSex() const {
    return sexComboBox->currentItem() == 0 ? Male : Female;
}

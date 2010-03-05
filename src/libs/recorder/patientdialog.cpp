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

#include <QMessageBox>
#include <QString>

#include <math.h>

using EpGui::PatientDialog;
//using EpStudy::Female;
//using EpStudy::Male;
using EpStudy::Name;
using EpPatient::Sex;
using EpStudy::Study;

PatientDialog::PatientDialog(QWidget* parent) :
            QDialog(parent), metricHeight_(0.0),
            metricWeight_(0.0) {
    setupUi(this);
    ageLineEdit->setEnabled(false);

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

void PatientDialog::reject() {
    // Actual Prucka shows idiotic Dialog box informing you that
    // cancelling Patient Information Dialog will cause loss
    // of your changes, but then only shows Ok button, so you
    // have no choice but to lose the changes anyway.
    // Most people understand what cancelling a dialog means,
    // and there is no mandate that every idiotic design used
    // in the Prucka system be slavishly followed.  If you
    // ever want to implement this "feature" do so here.
    QDialog::reject();
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

void PatientDialog::on_manualEditBsaCheckBox_stateChanged(int state) {
    bsaLineEdit->setEnabled(state == Qt::Checked);
    // recalculate bsa if necessary
    setBsaText();
}

void PatientDialog::setBsaText() {
    if (!manualEditBsaCheckBox->isChecked())
        bsaLineEdit->setText(QString::number(bsa()));
}

double PatientDialog::bsa() {
    double height = heightCmLineEdit->text().toDouble();
    double weight = weightKgLineEdit->text().toDouble();
    // Mosteller formula - see http://www.halls.md/body-surface-area/refs.htm
    return sqrt((height * weight) / 3600);
}

void PatientDialog::on_heightInLineEdit_textEdited() {
    heightCmLineEdit->setText(QString::number(
            inchesToCentimeters(heightInLineEdit->text().toDouble())));
    setBsaText();
}

void PatientDialog::on_weightLbsLineEdit_textEdited() {
    weightKgLineEdit->setText(QString::number(
            poundsToKilograms(weightLbsLineEdit->text().toDouble())));
    setBsaText();
}

void PatientDialog::on_heightCmLineEdit_textEdited() {
    heightInLineEdit->setText(QString::number(
            centimetersToInches(heightCmLineEdit->text().toDouble())));
    setBsaText();
}

void PatientDialog::on_weightKgLineEdit_textEdited() {
    weightLbsLineEdit->setText(QString::number(
            poundsToKilograms(weightKgLineEdit->text().toDouble())));
    setBsaText();
}

void PatientDialog::on_dobDateEdit_dateChanged(const QDate& date) {
    calculateAge();
    dobLabel->setText(date.toString("MMM d yyyy"));
}

void PatientDialog::on_studyDateTimeEdit_dateChanged(const QDate&) {
    calculateAge();
}

void PatientDialog::calculateAge() {
    int birthYear = dobDateEdit->date().year();
    int birthMonth = dobDateEdit->date().month();
    int birthDay = dobDateEdit->date().day();
    int studyYear = studyDateTimeEdit->date().year();
    int studyMonth = studyDateTimeEdit->date().month();
    int studyDay = studyDateTimeEdit->date().day();
    int years = studyYear - birthYear;
    int months = studyMonth - birthMonth;
    int days = studyDay - birthDay;
    int age = 0;
    if (months >= 0 && days >= 0)
        age = years;
    else
        age = --years; 
    ageLineEdit->setText(QString::number(age));
}

void PatientDialog::setFields(const Study* study) {
    Name name = study->name();
    lastNameLineEdit->setText(name.last());
    firstNameLineEdit->setText(name.first());
    middleNameLineEdit->setText(name.middle());
    mrnLineEdit->setText(study->mrn());
    accountNumberLineEdit->setText(study->accountNumber());
    studyDateTimeEdit->setDateTime(study->dateTime());
    studyNumberLineEdit->setText(study->number());
    dobDateEdit->setDate(study->dateOfBirth());
    sexComboBox->setCurrentIndex(study->sex());
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
    Name name(lastNameLineEdit->text(), 
              firstNameLineEdit->text(),
              middleNameLineEdit->text());
    study->setName(name);
    study->setMrn(mrnLineEdit->text());
    study->setAccountNumber(accountNumberLineEdit->text());
    study->setDateTime(studyDateTimeEdit->dateTime());
    study->setNumber(studyNumberLineEdit->text());
    study->setDateOfBirth(dobDateEdit->date());
    study->setSex(sex());
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

Sex PatientDialog::sex() const {
    return sexComboBox->currentIndex() == 0
            ? EpPatient::Male : EpPatient::Female;
}

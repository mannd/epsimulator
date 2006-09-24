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
#include <qdatetime.h>
#include <qdatetimeedit.h>

#include <math.h>

PatientDialog::PatientDialog(Study* study, QWidget *parent, 
			     const char *name)
    :PatientDialogBase(parent, name) {
    study_ = study;
    setFields();
    // below set by above
    //studyDateEdit->setDate(QDate::currentDate());
    //studyTimeEdit->setTime(QTime::currentTime());
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

void PatientDialog::accept() {
    getFields();
    PatientDialogBase::accept();
}

void PatientDialog::setFields() {
    Name name = study_->patient()->name();
    lastNameLineEdit->setText(name.last);
    firstNameLineEdit->setText(name.first);
    middleNameLineEdit->setText(name.middle);
    mrnLineEdit->setText(study_->patient()->mrn());
    studyDateEdit->setDate(study_->date());
    studyTimeEdit->setTime(study_->time());
}

void PatientDialog::getFields() {
    Name name;
    name.last = lastNameLineEdit->text();
    name.first = firstNameLineEdit->text();
    name.middle = middleNameLineEdit->text();
    study_->patient()->setName(name);
    study_->patient()->setMrn(mrnLineEdit->text());
    //study_->patient()->setSex(
    study_->patient()->setHeight(heightCmLineEdit->text().toDouble());
    study_->patient()->setWeight(weightKgLineEdit->text().toDouble()); 
    study_->patient()->setHeightIn(heightInLineEdit->text().toDouble());
    study_->patient()->setWeightLbs(weightLbsLineEdit->text().toDouble());
}

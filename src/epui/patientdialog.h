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
#ifndef PATIENTDIALOG_H
#define PATIENTDIALOG_H

#include "ui_patientdialog.h"
#include "study.h"


/// TODO Consider not allowing any edits of study date and time in PatientDialogBase.ui
class PatientDialog: public QDialog, private Ui::PatientDialog {
    Q_OBJECT
public:
    PatientDialog(QWidget *parent = 0);
    void setFields(const Study* study);
    void getFields(Study* study) const;
   
public slots:
    virtual void manualEditBsaCheckBox_toggled(int);
    virtual void weightKgLineEdit_lostFocus();
    virtual void weightLbsLineEdit_lostFocus();
    virtual void heightCmLineEdit_lostFocus();
    virtual void heightInLineEdit_lostFocus();
    virtual void accept();

private:
    double inchesToCentimeters(double) const;
    double centimetersToInches(double) const;
    double poundsToKilograms(double) const;
    double kilogramsToPounds(double) const;
    double bsa();
    void setBsaText();
    Sex getSex() const;

    double metricHeight_;
    double metricWeight_;
};

#endif

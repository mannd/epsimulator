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

#include "study.h"      // for Sex definition

#include <QDialog>

class QDate;
class QWidget;

namespace EpGui {

using EpStudy::Study;
using EpStudy::Name;
using EpStudy::Sex;

/// TODO Consider not allowing any edits of study date and time in PatientDialogBase.ui
class PatientDialog: public QDialog, private Ui::PatientDialog {
    Q_OBJECT

public:
    PatientDialog(QWidget *parent = 0);
    ~PatientDialog() {}

    void setFields(const Study* study);
    void getFields(Study* study) const;
   
public slots:
    void on_manualEditBsaCheckBox_stateChanged(int);
    void on_weightKgLineEdit_textEdited();
    void on_weightLbsLineEdit_textEdited();
    void on_heightCmLineEdit_textEdited();
    void on_heightInLineEdit_textEdited();
    void on_dobDateEdit_dateChanged(const QDate&);
    void on_studyDateTimeEdit_dateChanged(const QDate&);
    void accept();
    void reject();

private:
    void setBsaText();
    void calculateAge();

    double inchesToCentimeters(double) const;
    double centimetersToInches(double) const;
    double poundsToKilograms(double) const;
    double kilogramsToPounds(double) const;
    double bsa();
    Sex sex() const;

    double metricHeight_;
    double metricWeight_;
};

}

#endif

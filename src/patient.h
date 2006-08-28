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
#ifndef PATIENT_H
#define PATIENT_H

/**
The Patient class includes demographics, baseline autonomic tone, ejection fraction, diagnoses and the heart classes.  Drugs and sedation are part of the specific Study for each patient.
 
	@author David Mann <mannd@epstudiossoftware.com>
*/

#include <qstring.h>
#include <qdatetime.h>

class Heart;

struct Name {
    QString first;
    QString last;
    QString middle;
};

typedef int AutonomicTone;

enum Sex {MALE, FEMALE};

class Patient {
public:
    Patient();

    Name name() const {return name_;};
    QString fullName(bool lastFirst = false,
                     bool useMiddleName = false) const;
    QString mrn() const {return mrn_;}
    QDate dateOfBirth() const {return dateOfBirth_;}
    Sex sex() const {return sex_;}
    double height() const {return height_;}
    double weight() const {return weight_;}
    double bsa() const;
    bool doBsaManualEdit() const {return bsaManualEdit_;}
    int ef() {return ef_;}
    bool hasIschemia() {return ischemia_;}
    AutonomicTone vagalTone() {return vagalTone_;}
    AutonomicTone sympatheticTone() {return sympatheticTone_;}

    void setManualBsa(const int manualBsa) {manualBsa_ = manualBsa;}
    void setName(const Name name);
    void setMrn(const QString mrn) {mrn_ = mrn;}
    void setHeight(const double height) {height_ = height;}
    void setWeight(const double weight) {weight_ = weight;}
    void setBsaManualEdit(const bool bsaManualEdit) {
	bsaManualEdit_ = bsaManualEdit;}

    ~Patient();

private:
    // these don't need to be here, need to be on data entry form
    double inchesToCentimeters(double inches);
    double poundsToKilograms(double pounds);

    Name name_;
    QString mrn_;	// medical record number
    QDate dateOfBirth_;
    Sex sex_;
    // should below be in patient or in study?  These change over time
    double height_;	// use metric units: cm for height,
                        //convert to English in form
    double weight_;	// in kg
    double manualBsa_;  //manually entered BSA in m2
    bool bsaManualEdit_;
    Heart *heart_;
    AutonomicTone vagalTone_;
    AutonomicTone sympatheticTone_;
    int ef_;
    bool ischemia_;
    // other factors?

};

#endif

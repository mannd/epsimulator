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
    friend void patientCopy(Patient* lhs, Patient* rhs);
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
    double heightIn() const {return heightIn_;}
    double weightLbs() const {return weightLbs_;}
    bool bsaManualEdit() const {return bsaManualEdit_;}
    int ef() const {return ef_;}
    bool hasIschemia() const {return ischemia_;}
    AutonomicTone vagalTone() const {return vagalTone_;}
    AutonomicTone sympatheticTone() const {return sympatheticTone_;}

    void setBsa(const int bsa) {bsa_ = bsa;}
    void setName(const Name name);
    void setMrn(const QString mrn) {mrn_ = mrn;}
    void setHeight(const double height) {height_ = height;}
    void setWeight(const double weight) {weight_ = weight;}
    void setHeightIn(const double heightIn) {heightIn_ = heightIn;}
    void setWeightLbs(const double weightLbs) {weightLbs_ = weightLbs;}
    void setBsaManualEdit(const bool bsaManualEdit) {
	bsaManualEdit_ = bsaManualEdit;}

    ~Patient();

private:
    Name name_;
    QString mrn_;	// medical record number
    QDate dateOfBirth_;
    Sex sex_;
    double height_;	// use metric units: cm for height,
    double weight_;	// in kg
    double heightIn_;   // English versions
    double weightLbs_;
    double bsa_;  // BSA either manually edited or calculated
    bool bsaManualEdit_;  // should BSA be manually edited or calculated?
    Heart *heart_;
    AutonomicTone vagalTone_;
    AutonomicTone sympatheticTone_;
    int ef_;
    bool ischemia_;
    // other factors?

};

#endif

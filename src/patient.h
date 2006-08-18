/***************************************************************************
 *   Copyright (C) 2006 by David Mann   *
 *   mannd@epstudiossoftware.com   *
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
    QString firstName;
    QString lastName;
    QString middleInitial;
};

typedef int AutonomicTone;

enum Sex {MALE, FEMALE};

class Patient {
public:
    Patient();

    Name name() const {
        return theName;
    };
    QString fullName(bool lastFirst = false,
                     bool useMiddleName = false) const;
    QString mrn() const {
        return theMrn;
    }
    QDate dateOfBirth() const {
        return theDateOfBirth;
    }
    Sex sex() const {
        return theSex;
    }
    double height() const {
        return theHeight;
    }
    double weight() const {
        return theWeight;
    }
    double bsa() const;

    void setManualBsa(const int value) {
        manualBsa = value;
    }  //

    ~Patient();
private:
    Name theName;
    QString theMrn;	// medical record number
    QDate theDateOfBirth;
    Sex theSex;
    // should below be in patient or in study?  These change over time
    double theHeight;	// use metric units: cm for height, convert to English in form
    double theWeight;	// in kg
    double theBsa() const;    // calculated or manually edited
    double manualBsa;  //manually entered BSA in m2

    Heart heart;
    AutonomicTone vagalTone;
    AutonomicTone sympatheticTone;
    int ef;
    bool ischemia;
    // other factors?

};

#endif

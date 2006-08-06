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
#ifndef DEMOGRAPHICS_H
#define DEMOGRAPHICS_H

#include <qstring.h>
#include <qdatetime.h>

/**
Patient demographics are stored here.  A Demographics instance is part of the Patient class.
This is basically a struct, the data members are public.  Data is validated by the entry
form.  The only real processing is the BSA.
 
	@author David Mann <mannd@epstudiossoftware.com>
*/

class Demographics
{
public:
    enum Sex {male, female};

    Demographics();

    QString lastName;
    QString firstName;
    QString middleName;
    QString mrn;	// medical record number
    QDate dateOfBirth;
    double height;	// use metric units: cm for height, convert to English in form
    double weight;	// in kg
    bool bsaManualEdit;
    Sex sex;
    QString race;
    QString ssn;
    QString insurance;
    QString address1;
    QString address2;
    QString city;
    QString state;
    QString postalCode;
    QString country;
    QString homePhone;
    QString workPhone;
    QString fax;
    QString email;

    double bsa() const;    // calculated or manually edited
    void setManualBsa(const int value) {manualBsa = value;}  //

    ~Demographics();

private:
    double manualBsa;  //manually entered BSA in m2

};

#endif

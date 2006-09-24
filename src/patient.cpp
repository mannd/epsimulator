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

#include "patient.h"
#include <math.h>
#include <qstring.h>

void patientCopy(Patient* lhs, Patient* rhs) {
    lhs->name_ = rhs->name_;
    lhs->mrn_ = rhs->mrn_;
    lhs->sex_ = rhs->sex_;
    lhs->height_ = rhs->height_;
    lhs->weight_ = rhs->weight_;
    lhs->heightIn_ = rhs->heightIn_;
    lhs->weightLbs_ = rhs-> weightLbs_;
    lhs->bsa_ = rhs->bsa_;
    lhs->bsaManualEdit_ = rhs->bsaManualEdit_;
    lhs->vagalTone_ = rhs->vagalTone_;
    lhs->sympatheticTone_ = rhs->sympatheticTone_;
    lhs->ef_ = rhs->ef_;
    lhs->ischemia_ = rhs->ischemia_;
    // copyHeart(lhs->heart_, rhs->heart_);
}

Patient::Patient() : dateOfBirth_(1950, 1, 1) {
}

QString Patient::fullName(bool lastFirst, bool useMiddleName) const {
    QString middleName;
    if (useMiddleName && !name_.middle.isEmpty()) 
	middleName = " " + name_.middle + " ";
    else
	middleName = " ";
    if (lastFirst) 
	return name_.last + "," + name_.first + middleName;
    else
	return name_.first + middleName + name_.last;
}

void Patient::setName(const Name name) {
    name_.last = name.last;
    name_.first = name.first;
    name_.middle = name.middle;
}




Patient::~Patient() {}

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

#include "study.h"
#include "heart.h"

#include <cassert> 


QString Name::fullName(bool lastFirst, bool useMiddleName) const {
    QString middleName;
    if (useMiddleName && !middle.isEmpty()) 
	middleName = " " + middle.stripWhiteSpace() + " ";
    else
	middleName = " ";
    if (lastFirst) 
	return (last.stripWhiteSpace() + ", " + first.stripWhiteSpace()
                 + middleName).stripWhiteSpace();
    else
	return first.stripWhiteSpace() + middleName + last.stripWhiteSpace();
}


Study::Study() : dateTime_(QDateTime::currentDateTime()),
    dateOfBirth_(DEFAULT_BIRTH_DATE),
    sex_(FEMALE), height_(0), weight_(0), 
    heightIn_(0), weightLbs_(0), bsa_(0), 
    bsaManualEdit_(false), vagalTone_(DEFAULT_VAGAL_TONE),
    sympatheticTone_(DEFAULT_SYMPATHETIC_TONE), ef_(DEFAULT_EF), 
    ischemia_(false), heart_(0) {
    heart_ = new Heart;
    testInvariant();
}

void Study::copyStudy(const Study& study) {
    name_ = study.name_;    // default copy constructor works here
    dateTime_ = study.dateTime_;
    dateOfBirth_ = study.dateOfBirth_;
    mrn_ = study.mrn_;
    number_ = study.number_;
    sex_ = study.sex_;
    height_ = study.height_;
    weight_ = study.weight_;
    heightIn_ = study.heightIn_;
    weightLbs_ = study. weightLbs_;
    bsa_ = study.bsa_;
    bsaManualEdit_ = study.bsaManualEdit_;
    vagalTone_ = study.vagalTone_;
    sympatheticTone_ = study.sympatheticTone_;
    ef_ = study.ef_;
    ischemia_ = study.ischemia_;
    // copy the heart pointer
    heart_ = new Heart(*study.heart_);
}

Study::Study(const Study& study) {
    copyStudy(study);
    testInvariant();
}

void Study::setName(const Name& name) {
    name_.last = name.last;
    name_.first = name.first;
    name_.middle = name.middle;
}

AutonomicTone Study::adjustTone(AutonomicTone tone) {
    tone = tone > MAX_TONE ? MAX_TONE : tone;
    tone = tone < MIN_TONE ? MIN_TONE : tone;
    return tone;
}

void Study::setVagalTone(AutonomicTone tone) {
    vagalTone_ = adjustTone(tone);
}

void Study::setSympatheticTone(AutonomicTone tone) {
    sympatheticTone_ = adjustTone(tone);
}

Study& Study::operator =(const Study& rhs) {
    if (this == &rhs)
        return *this;
    copyStudy(rhs);
    testInvariant();
    return *this;
   
}

void Study::setEf(int ef) {
    ef_ = ef > MAX_EF ? MAX_EF : ef;
    ef_ = ef < MIN_EF ? MIN_EF : ef;
}

inline void Study::testInvariant() const {
    assert(vagalTone_ >= MIN_TONE && vagalTone_ <= MAX_TONE);
    assert(sympatheticTone_ >= MIN_TONE && sympatheticTone_ <= MAX_TONE);
    assert (sex_ == MALE || sex_ == FEMALE);
    assert (ef_ >= MIN_EF && ef_ <= MAX_EF);
}


Study::~Study() {
    delete heart_;
}

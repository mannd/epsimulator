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

 

void studyCopy(Study* lhs, Study* rhs) {
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

Study::Study() : date_(QDate::currentDate()),
    time_(QTime::currentTime()), dateOfBirth_(1950, 1, 1),
    sex_(MALE), height_(0), weight_(0), 
    heightIn_(0), weightLbs_(0), bsa_(0), 
    bsaManualEdit_(false), vagalTone_(50),
    sympatheticTone_(50), ef_(50), ischemia_(false), heart_(0) {
}

Study::Study(const Study& study) {
    name_ = study.name_;    // default copy constructor works here
    mrn_ = study.mrn_;
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
    heart_ = 0; //don't deal with it yet
}

QString Study::fullName(bool lastFirst, bool useMiddleName) const {
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

void Study::setName(const Name name) {
    name_.last = name.last;
    name_.first = name.first;
    name_.middle = name.middle;
}


void Study::setNumber(QString number) {
    number_ = number;
}

QDate Study::date() const {
    return date_;
}


QTime Study::time() const {
    return time_;
}

QString Study::number() const {
    return number_;
}

Study& Study::operator =(const Study& rhs) {
    if (this == &rhs)
        return *this;
    name_ = rhs.name_;
    mrn_ = rhs.mrn_;
    sex_ = rhs.sex_;
    height_ = rhs.height_;
    weight_ = rhs.weight_;
    heightIn_ = rhs.heightIn_;
    weightLbs_ = rhs. weightLbs_;
    bsa_ = rhs.bsa_;
    bsaManualEdit_ = rhs.bsaManualEdit_;
    vagalTone_ = rhs.vagalTone_;
    sympatheticTone_ = rhs.sympatheticTone_;
    ef_ = rhs.ef_;
    ischemia_ = rhs.ischemia_;
    // deal with heart pointer
    heart_ = 0;
    return *this;
}


Study::~Study() {
    delete heart_;
}

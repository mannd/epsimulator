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

Study::Study() : dateOfBirth_(1950, 1, 1) {
    setDate(QDate::currentDate());
    setTime(QTime::currentTime());
}

void Study::setDate(QDate date) {
    date_ = date;
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


void Study::setTime(QTime time) {
    time_ = time;
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

Study::~Study() {
}

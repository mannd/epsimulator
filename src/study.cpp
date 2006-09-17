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

Study::Study() {
    patient_ = new Patient;
    setDate(QDate::currentDate());
    setTime(QTime::currentTime());
}

void Study::setPatient(Patient* patient) {
    patient_ = patient;
}

void Study::setDate(QDate date) {
    date_ = date;
}


void Study::setTime(QTime time) {
    time_ = time;
}

void Study::setNumber(QString number) {
    number_ = number;
}

Patient* Study::patient() const {
    return patient_;
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
    delete patient_;
}

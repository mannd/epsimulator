/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

#include "fileutilities.h"
#include "heart.h"

#include <QDir>

using namespace Ep;

using EpPatient::Patient;
using EpPatient::HeartRate;

const QString Patient::fileName_ = "patient.dat";

Patient::Patient(const QString& name)
    : name_(name), path_(0),
    sex_(Male),
    vagalTone_(50),
    sympatheticTone_(50),
    o2Saturation_(0),
    bp_(0,0), secs_(0) {
    heart_ = new Heart;
    // just for fun
    o2Saturation_ = 90;
}

Patient::~Patient() {
    delete heart_;
}

void Patient::load() {
    EpCore::loadData(filePath(), MagicNumber, *this);
}

void Patient::save() {
    EpCore::saveData(filePath(), MagicNumber, *this);
}

/** Called every second by simulation -- at present by
  PatientStatusBar -- and updates vital signs.
Needs to take into account:
    previous VS values
    sedation levels
    autonomic tone
    Heart simulation heart rate
    FiO2 level
    random fluctations
    others?

**/
void Patient::updatePhysiology() {
    // fake for fun
    ++secs_;
    if (secs_ % 5 == 0)
        ++o2Saturation_;
    if (secs_ % 3 == 0)
        --o2Saturation_;
    if (o2Saturation_ < 90)
       ++o2Saturation_;
}

void Patient::setHeartRate(const HeartRate rate) {
    heart_->setHeartRate(rate);
}

HeartRate Patient::heartRate() const {
    return heart_->meanHeartRate();
}

TimeInterval Patient::meanCL() const {
    return heart_->meanCL();
}

QString Patient::filePath() {
    return EpCore::joinPaths(path_, fileName_);
}

namespace EpPatient {

// friends

QDataStream& operator<<(QDataStream& out, const Patient& patient) {
    out << patient.name_ << patient.path_ << patient.o2Saturation_
            << patient.bp_ << *patient.heart_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Patient& patient) {
    in >> patient.name_ >> patient.path_ >> patient.o2Saturation_
            >> patient.bp_ >> *patient.heart_;
    return in;
}

}

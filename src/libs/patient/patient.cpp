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
#include "probability.h"

#include <QDir>

#include <QtDebug>

using namespace Ep;

using EpCore::Probability;
using EpPatient::Patient;
using EpPatient::HeartRate;
using EpPatient::Name;

Name::Name(const QString& last,
           const QString& first,
           const QString& middle) {
    setLastFirstMiddle(last, first, middle);
}

void Name::setLastFirstMiddle(const QString& last,
                              const QString& first,
                              const QString& middle) {
    last_ = last.simplified();
    first_ = first.simplified();
    middle_ = middle.simplified();
}

QString Name::firstMiddleLast() const {
    return fullName(false, true);
}

QString Name::lastFirstMiddle() const {
    return fullName(true, true);
}

QString Name::lastFirst() const {
    return fullName(true, false);
}

QString Name::fullName(bool lastFirst, bool useMiddleName) const {
    QString middleName;
    if (useMiddleName && !middle_.isEmpty())
        middleName = " " + middle_ + " ";
    else
        middleName = " ";
    if (lastFirst)
        return (last_ + ", " + first_ + middleName).simplified();
    else
        return (first_ + middleName + last_).simplified();
}

namespace EpPatient {

QDataStream& operator<<(QDataStream& out, const Name& name) {
    out << name.first_ << name.middle_ << name.last_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Name& name) {
    in >> name.first_ >> name.middle_ >> name.last_;
    return in;
}

}

const QString Patient::fileName_ = "patient.dat";

Patient::Patient(const QString& name)
    : name_(name), path_(),
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
    static Probability p(0.5);
    ++secs_;
    if (secs_ % 5 == 0 && p.result())
        ++o2Saturation_;
    if (secs_ % 4 == 0  && !p.result())
        --o2Saturation_;
    if (o2Saturation_ < 90) {
        p.setValue(0.6);
        ++o2Saturation_;
   }
    if (o2Saturation_ > 95) {
        p.setValue(0.4);
        --o2Saturation_;
    }
    //qDebug() << p.result();
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

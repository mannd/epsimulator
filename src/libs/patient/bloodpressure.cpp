/***************************************************************************
 *   Copyright (C) 2010 by EP Studios, Inc.                                *
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

#include "bloodpressure.h"

#include <QDataStream>

using EpPatient::BloodPressure;

const int BloodPressure::rateCutoff = 120;

BloodPressure::BloodPressure(const int systolic, const int diastolic)
    : systolic_(systolic), diastolic_(diastolic) {
    normalizeBp();
}

void BloodPressure::normalizeBp() {
    // make sure BP is reasonable
    // no negatives
    if (systolic_ < 0)
        systolic_ = 0;
    if (diastolic_ < 0)
        diastolic_ = 0;
    // make sure systolic >= diastolic
    if (systolic_ < diastolic_)
        systolic_ = diastolic_;
}

void BloodPressure::setSystolic(const int systolic) {
    systolic_ = systolic;
    normalizeBp();
}

void BloodPressure::setDiastolic(const int diastolic) {
    diastolic_ = diastolic;
    normalizeBp();
}

void BloodPressure::setBp(const int systolic, const int diastolic) {
    systolic_ = systolic;
    diastolic_ = diastolic;
    normalizeBp();
}

QString BloodPressure::bp() const {
    return QString::number(systolic_) + "/" + QString::number(diastolic_);
}

int BloodPressure::meanBp() const {
    // doesn't account for heart rate
    return diastolic_ + (systolic_ - diastolic_)/3;
}

int BloodPressure::meanBp(int heartRate) const {
    if (heartRate < rateCutoff)
        return meanBp();
    else
        // better estimate of mean BP at faster heart rates
        // http://en.wikipedia.org/wiki/Mean_arterial_pressure
        return (systolic_ + diastolic_) / 2;
}

namespace EpPatient {

QDataStream& operator<<(QDataStream& out, const BloodPressure& bp) {
    out << bp.systolic_ << bp.diastolic_;
    return out;
}

QDataStream& operator>>(QDataStream& in, BloodPressure& bp) {
    in >> bp.systolic_ >> bp.diastolic_;
    return in;
}

}

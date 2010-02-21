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

#include "interval.h"

#include "fileutilities.h"
#include "options.h"

#include <QDataStream>
#include <QStringList>

namespace EpCore {

QDataStream& operator<<(QDataStream& out, const Interval& interval) {
    out << interval.name_ << interval.mark1_ << interval.mark2_
            << (qint32)interval.width_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Interval& interval) {
    in >> interval.name_ >> interval.mark1_ >> interval.mark2_
            >> interval.width_;
    return in;
}

bool operator<(const Interval& value1, const Interval& value2) {
    return value1.name() < value2.name();
}

}

using EpCore::Interval;
using EpCore::Mark;

const QString Interval::fileName_ = "intervals.dat";

Interval::Interval(const QString& name, const Mark& mark1,
                   const Mark& mark2, int width)
    : name_(name), mark1_(mark1), mark2_(mark2), width_(width) {}

Interval::Interval(const Interval& interval) {
    copyInterval(interval);
}

Interval& Interval::operator=(const Interval& rhs) {
    if (this == &rhs)
        return *this;
    copyInterval(rhs);
    return *this;
}

const Interval::IntervalParameter Interval::defaultParameters_[]
        = {{tr("None"), Mark::None, Mark::None},
           {tr("A1A1"), Mark::A1, Mark::A1},
           {tr("A1A2"), Mark::A1, Mark::A2},
           {tr("A2A3"), Mark::A2, Mark::A3},
           {tr("A3A4"), Mark::A3, Mark::A4},
           {tr("A1H1"), Mark::A1, Mark::H1},
           {tr("A2H2"), Mark::A2, Mark::H2},
           {tr("AA"), Mark::A, Mark::A},
           {tr("SCL"), Mark::A, Mark::A},
           {tr("PR"), Mark::P, Mark::QRSonset},
           {tr("QRS"), Mark::QRSonset, Mark::QRSoffset},
           {tr("QT"), Mark::QRSonset, Mark::Toffset},
           {tr("PA"), Mark::P, Mark::A},
           {tr("AH"), Mark::A, Mark::H},
           {tr("HV"), Mark::H, Mark::QRSonset},
           {tr("H1H1"), Mark::H1, Mark::H1},
           {tr("H1H2"), Mark::H1, Mark::H2},
           {tr("V1V2"), Mark::V1, Mark::V2},
           {tr("V2V3"), Mark::V2, Mark::V3},
           {tr("V3V4"), Mark::V3, Mark::V4},
           // note that CardioLab uses S1 label for S1S1, etc.
           {tr("S1"), Mark::S1, Mark::S1},
           {tr("S2"), Mark::S1, Mark::S2},
           {tr("S3"), Mark::S2, Mark::S3},
           {tr("S4"), Mark::S3, Mark::S4},
           {tr("S5"), Mark::S4, Mark::S5}};


QList<Interval> Interval::defaultItems() {
    QList<Interval> intervals;
    int size = sizeof(defaultParameters_) / sizeof(IntervalParameter);
    for (int i = 0; i < size; ++i)
        intervals.append(Interval(defaultParameters_[i].name,
                                  Mark(defaultParameters_[i].mark1),
                                  Mark(defaultParameters_[i].mark2),
                                  5));
    return intervals;
}

void Interval::copyInterval(const Interval& interval) {
    name_ = interval.name_;
    mark1_ = interval.mark1_;
    mark2_ = interval.mark2_;
    width_ = interval.width_;
}

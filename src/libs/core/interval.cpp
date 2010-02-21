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

class QDataSteam;

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


QList<Interval> Interval::defaultItems() {
    QList<Interval> intervals;
    intervals.append(Interval(tr("A1A1"), Mark(Mark::A1), Mark(Mark::A1), 5));
    intervals.append(Interval(tr("A1A2"), Mark(Mark::A1), Mark(Mark::A2), 5));
    intervals.append(Interval(tr("A1H1"), Mark(Mark::A1), Mark(Mark::H1), 5));
    intervals.append(Interval(tr("A2A3"), Mark(Mark::A2), Mark(Mark::A3), 5));
    intervals.append(Interval(tr("A2H2"), Mark(Mark::A2), Mark(Mark::H2), 5));
    intervals.append(Interval(tr("A3A4"), Mark(Mark::A3), Mark(Mark::A4), 5));
    intervals.append(Interval(tr("AA"), Mark(Mark::A), Mark(Mark::A), 5));
    intervals.append(Interval(tr("AH"), Mark(Mark::A), Mark(Mark::H), 5));
    intervals.append(Interval(tr("SCL"), Mark(Mark::A), Mark(Mark::A), 5));
    intervals.append(Interval(tr("PR"), Mark(Mark::P),
                              Mark(Mark::QRSonset), 5));
    intervals.append(Interval(tr("QRS"), Mark(Mark::QRSonset),
                              Mark(Mark::QRSoffset), 5));
    intervals.append(Interval(tr("QT"), Mark(Mark::QRSonset),
                              Mark(Mark::Toffset), 5));
    intervals.append(Interval(tr("HV"), Mark(Mark::H),
                              Mark(Mark::QRSonset), 5));
    intervals.append(Interval(tr("H1H1"), Mark(Mark::H1), Mark(Mark::H1), 5));
    intervals.append(Interval(tr("H1H2"), Mark(Mark::H1), Mark(Mark::H2), 5));
    intervals.append(Interval(tr("V1V1"), Mark(Mark::V1), Mark(Mark::V1), 5));
    intervals.append(Interval(tr("V1V2"), Mark(Mark::V1), Mark(Mark::V2), 5));
    intervals.append(Interval(tr("V2V3"), Mark(Mark::V2), Mark(Mark::V3), 5));
    intervals.append(Interval(tr("V3V4"), Mark(Mark::V3), Mark(Mark::V4), 5));
    // note that CardioLab uses S1 label for S1S1, etc.
    intervals.append(Interval(tr("S1"), Mark(Mark::S1), Mark(Mark::S1), 5));
    intervals.append(Interval(tr("S2"), Mark(Mark::S1), Mark(Mark::S2), 5));
    intervals.append(Interval(tr("S3"), Mark(Mark::S2), Mark(Mark::S3), 5));
    intervals.append(Interval(tr("S4"), Mark(Mark::S3), Mark(Mark::S4), 5));
    intervals.append(Interval(tr("S5"), Mark(Mark::S4), Mark(Mark::S5), 5));
    return intervals;
}

void Interval::copyInterval(const Interval& interval) {
    name_ = interval.name_;
    mark1_ = interval.mark1_;
    mark2_ = interval.mark2_;
    width_ = interval.width_;
}

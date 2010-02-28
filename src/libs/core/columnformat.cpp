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

#include "columnformat.h"
#include "interval.h"

#include <QDataStream>
#include <QStringList>

namespace EpCore {

    QDataStream& operator<<(QDataStream& out, const ColumnFormat& cf) {
        out << cf.name_ << cf.selectedIntervals_;
        return out;
    }

    QDataStream& operator>>(QDataStream& in, ColumnFormat& cf) {
        in >> cf.name_ >> cf.selectedIntervals_;
        return in;
    }

    bool operator<(const ColumnFormat& value1, const ColumnFormat& value2) {
        return value1.name_ < value2.name_;
    }




}

using EpCore::ColumnFormat;
using EpCore::Interval;

const QString ColumnFormat::fileName_ = "columnformats.dat";

ColumnFormat::ColumnFormat(const QString& name,
                           const QList<Interval>& selectedIntervals)
                               : name_(name)
                               , selectedIntervals_(selectedIntervals) {
}

QList<Interval> ColumnFormat::unselectedIntervals() const {
    QList<Interval> unselectedIntervals = intervals_.list();
    QListIterator<Interval> iter(selectedIntervals_);
    while (iter.hasNext())
        unselectedIntervals.removeAll(iter.next());
    return unselectedIntervals;
}

QList<Interval> ColumnFormat::allIntervals() {
    ItemList<Interval> allIntervals;
    return allIntervals.list();
}

QStringList ColumnFormat::allIntervalNames() {
    return intervalNames(allIntervals());
}

QList<ColumnFormat> ColumnFormat::defaultItems() {
    QList<ColumnFormat> columnFormats;
    QList<Interval> intervals;
    ItemList<Interval> defaultIntervals;
    QString name;
    name = tr("BASELINE");
    intervals.append(defaultIntervals[tr("SCL")]);
    intervals.append(defaultIntervals[tr("PR")]);
    intervals.append(defaultIntervals[tr("QRS")]);
    intervals.append(defaultIntervals[tr("QT")]);
    intervals.append(defaultIntervals[tr("AH")]);
    columnFormats.append(ColumnFormat(name, intervals));
    name = tr("A PACE");
    intervals.clear();
    intervals.append(defaultIntervals[tr("S1")]);
    intervals.append(defaultIntervals[tr("S2")]);
    intervals.append(defaultIntervals[tr("S3")]);
    intervals.append(defaultIntervals[tr("S4")]);
    intervals.append(defaultIntervals[tr("S5")]);
    intervals.append(defaultIntervals[tr("SCL")]);
    intervals.append(defaultIntervals[tr("A1A1")]);
    intervals.append(defaultIntervals[tr("A1A2")]);
    intervals.append(defaultIntervals[tr("A2A3")]);
    intervals.append(defaultIntervals[tr("H1H1")]);
    intervals.append(defaultIntervals[tr("H1H2")]);
    intervals.append(defaultIntervals[tr("V1V1")]);
    intervals.append(defaultIntervals[tr("V1V2")]);
    intervals.append(defaultIntervals[tr("V2V3")]);
    intervals.append(defaultIntervals[tr("V3V4")]);
    columnFormats.append(ColumnFormat(name, intervals));
    name = tr("AEST");
    intervals.clear();
    // add intervals here
    columnFormats.append(ColumnFormat(name, intervals));
    name = tr("V Pace");
    intervals.clear();
    columnFormats.append(ColumnFormat(name, intervals));
    name = tr("VEST");
    intervals.clear();
    columnFormats.append(ColumnFormat(name, intervals));
    // etc.
    return columnFormats;
}

QStringList ColumnFormat::intervalNames(const QList<Interval>& intervals) {
    QListIterator<Interval> iter(intervals);
    QStringList names;
    while (iter.hasNext())
        names << iter.next().name();
    return names;
}

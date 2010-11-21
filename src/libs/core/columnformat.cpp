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
    out << cf.name_ << cf.intervals_;
    return out;
}

QDataStream& operator>>(QDataStream& in, ColumnFormat& cf) {
    in >> cf.name_ >> cf.intervals_;
    return in;
}

bool operator<(const ColumnFormat& value1, const ColumnFormat& value2) {
    return value1.name_ < value2.name_;
}

} // namespace EpCore

using EpCore::ColumnFormat;
using EpCore::Interval;

const QString ColumnFormat::fileName_ = "columnformats.dat";

ColumnFormat::ColumnFormat(const QString& name,
                           const QList<Interval>& intervals)
                               : name_(name),
                               intervals_(intervals) {
}

QStringList ColumnFormat::intervalNames(const QList<Interval>& intervals) {
    QListIterator<Interval> iter(intervals);
    QStringList names;
    while (iter.hasNext())
        names << iter.next().name();
    return names;
}

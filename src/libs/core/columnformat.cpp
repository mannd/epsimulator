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

using EpCore::ColumnFormat;
using EpCore::Interval;

ColumnFormat::ColumnFormat(const QString& name,
                           const QList<Interval>& selectedIntervals)
                               : name_(name)
                               , selectedIntervals_(selectedIntervals) {

}

QList<Interval> ColumnFormat::unselectedIntervals() const {
    QList<Interval> unselectedIntervals;
    QListIterator<Interval> iter(selectedIntervals_);
    Interval interval;
    while (iter.hasNext()) {
        if (!intervals_.contains(interval = iter.next()))
            unselectedIntervals.append(interval);
    }
    return unselectedIntervals;
}

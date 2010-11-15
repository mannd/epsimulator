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

#ifndef COLUMNFORMAT_H
#define COLUMNFORMAT_H

#include "interval.h"
#include "itemlist.h"

#include <QtCore/QCoreApplication>

class QDataStream;

namespace EpCore {

class ColumnFormat {
    Q_DECLARE_TR_FUNCTIONS(ColumnFormat)

public:
    friend QDataStream& operator<<(QDataStream&, const ColumnFormat&);
    friend QDataStream& operator>>(QDataStream&, ColumnFormat&);
    friend bool operator<(const ColumnFormat&, const ColumnFormat&);

    ColumnFormat(const QString& name = QString(),
                 const QList<Interval>& selectedIntervals = QList<Interval>());

    static unsigned int magicNumber() {return MagicNumber;}
    static QString fileName() {return fileName_;}
    static QList<ColumnFormat> defaultItems();
    static QStringList intervalNames(const QList<Interval>&);
    static QList<Interval> allIntervals();
    static QStringList allIntervalNames();

    void setName(const QString& name) {name_ = name;}
    void setIntervals(const QList<Interval>& intervals) {intervals_ = intervals;}

    QString name() const {return name_;}
    QList<Interval> selectedIntervals() const {return selectedIntervals_;}
    QList<Interval> unselectedIntervals() const;


private:
    enum {MagicNumber = 0x00304010};

    static const QString fileName_;
    QString name_;
    QList<Interval> selectedIntervals_;
    QList<Interval> intervals_;
};

}

#endif // COLUMNFORMAT_H

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

class QDataStream;

namespace EpCore {

class ColumnFormat {
public:
    friend QDataStream& operator<<(QDataStream&, const ColumnFormat&);
    friend QDataStream& operator>>(QDataStream&, ColumnFormat&);
    friend bool operator<(const ColumnFormat&, const ColumnFormat&);

    ColumnFormat(const QString& name = QString(),
                 const QList<Interval>& selectedIntervals = QList<Interval>());

    static unsigned int magicNumber() {return MagicNumber;}
    static QString fileName() {return fileName_;}
    static QStringList intervalNames(const QList<Interval>&);

    void setName(const QString& name) {name_ = name;}

    QString name() const {return name_;}
    QList<Interval> intervals() const {return intervals_;}

private:
    enum {MagicNumber = 0x00304010};

    static const QString fileName_;
    QString name_;
    QList<Interval> intervals_;
};

} // namespace EpCore

#endif // COLUMNFORMAT_H

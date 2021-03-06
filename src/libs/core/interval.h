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

#ifndef INTERVAL_H
#define INTERVAL_H

#include "mark.h"

class QDataStream;

namespace EpCore {

class Interval {
public:
    friend QDataStream& operator<<(QDataStream&, const Interval&);
    friend QDataStream& operator>>(QDataStream&, Interval&);
    friend bool operator<(const Interval&, const Interval&);

    Interval(const QString& name = QString(),
             const Mark& mark1 = Mark(),
             const Mark& mark2 = Mark(),
             int width = 5);
    Interval(const Interval&);

    Interval& operator=(const Interval&);
    bool operator==(const Interval& rhs) const {
        return name_ == rhs.name_;
    }

    static unsigned int magicNumber() {return MagicNumber;}
    static QString fileName() {return fileName_;}

    void setName(const QString& name) {name_  = name;}
    void setMark1(const Mark& mark) {mark1_ = mark;}
    void setMark2(const Mark& mark) {mark2_ = mark;}
    void setWidth(int width) {width_ = width;}

    QString name() const {return name_;}
    Mark mark1() const {return mark1_;}
    Mark mark2() const {return mark2_;}
    int width() const {return width_;}

    // can convert below to inches using EpGui::appDpiX().
    int rawInterval() const {return mark2_.x() - mark1_.x();}

private:
    enum {MagicNumber = 0x99f00010};
    typedef struct {
        QString name;
        Mark::MarkType mark1;
        Mark::MarkType mark2;
    } IntervalParameter;

    void copyInterval(const Interval&);

    static const QString fileName_;
    QString name_;
    Mark mark1_;
    Mark mark2_;
    int width_;
};

} // namespace EpCore

#endif // INTERVAL_H

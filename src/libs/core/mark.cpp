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

#include "mark.h"

#include <QDataStream>
#include <QStringList>

namespace EpCore {

    const QMap<Mark::MarkType, QString> Mark::markMap_ = Mark::initMarkMap();

    QDataStream& operator<<(QDataStream& out, const Mark& mark) {
        out << (qint32)mark.type_ << mark.point_;
        return out;
    }

    QDataStream& operator>>(QDataStream& in, Mark& mark) {
        int name;
        in >> name >> mark.point_;
        mark.type_ = static_cast<Mark::MarkType>(name);
        return in;
    }

    Mark::Mark(MarkType type) : type_(type) {}

    Mark::Mark(const Mark& rhs) {
        type_ = rhs.type_;
        point_ = rhs.point_;
    }

    Mark& Mark::operator=(const Mark& rhs) {
        if (&rhs == this)
            return *this;
        type_ = rhs.type_;
        point_ = rhs.point_;
        return *this;
    }

    QStringList Mark::markNames() {
        int key = static_cast<int>(Mark::None);
        int lastKey = static_cast<int>(Mark::LastType);
        QStringList names;
        for (key = 0; key <= lastKey; ++key)
            names << markMap_[static_cast<Mark::MarkType>(key)];
        return names;
    }

}


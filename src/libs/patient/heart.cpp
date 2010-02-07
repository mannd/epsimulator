/***************************************************************************
 *   Copyright (C) 2006 by EP Studios, Inc.                                *
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

#include "heart.h"

#include <QDataStream>

using EpPatient::Heart;

Heart::Heart(const QString& name) : name_(name) {}

Heart::Heart(const Heart& heart) : name_(heart.name_) {}

Heart::~Heart() {}

namespace EpPatient {

    QDataStream& operator<<(QDataStream& out, const Heart& heart) {
        out << heart.name_;
        return out;
    }

    QDataStream& operator>>(QDataStream& in, Heart& heart) {
        in >> heart.name_;
        return in;
    }

}

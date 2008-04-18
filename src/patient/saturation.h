/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

#ifndef SATURATION_H
#define SATURATION_H

#include <QCoreApplication>
#include <QString>

#include <cassert>

class QDataStream;

/**
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Saturation{
    Q_DECLARE_TR_FUNCTIONS(Saturation)

public:
    friend QDataStream& operator<<(QDataStream&, const Saturation&);
    friend QDataStream& operator>>(QDataStream&, Saturation&);
    friend Saturation operator+(Saturation lhs, const Saturation& rhs);
    friend Saturation operator-(Saturation lhs, const Saturation& rhs);
    friend bool operator==(const Saturation& lhs, const Saturation& rhs);
    friend bool operator!=(const Saturation& lhs, const Saturation& rhs);
    friend bool operator<(const Saturation& lhs, const Saturation& rhs);
    friend bool operator<=(const Saturation& lhs, const Saturation& rhs);
    friend bool operator>(const Saturation& lhs, const Saturation& rhs);
    friend bool operator>=(const Saturation& lhs, const Saturation& rhs); 

    Saturation(int number = 0);
    Saturation(const Saturation&);
    ~Saturation();

    Saturation& operator=(const Saturation& rhs);
    Saturation& operator+=(const Saturation& rhs);
    Saturation& operator-=(const Saturation& rhs);
    Saturation& operator++();
    Saturation operator++(int);
    Saturation& operator--();
    Saturation operator--(int);

    void setNumber(int);

    int number() const {return number_;}
    QString percent() const;

private:
    void testInvariant() const;

    int number_;

};

inline void Saturation::testInvariant() const {
    assert(number_ >= 0 && number_ <= 100);
}

#endif

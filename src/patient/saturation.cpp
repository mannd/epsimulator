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

#include "saturation.h"

#include <QDataStream>

namespace EpPatient {

Saturation::Saturation(int number) {
    setNumber(number);
    testInvariant();
}

Saturation::Saturation(const Saturation& sat) {
    number_ = sat.number_;
    testInvariant();
}

Saturation::~Saturation() {
}

Saturation& Saturation::operator=(const Saturation& rhs) {
    if (this == &rhs)
        return *this;
    number_ = rhs.number_;
    testInvariant();
    return *this;
}

Saturation& Saturation::operator+=(const Saturation& rhs) {
    if (this == &rhs)
        return *this;
    int number = number_;
    setNumber(number += rhs.number_);
    return *this;
}

Saturation& Saturation::operator-=(const Saturation& rhs) {
    if (this == &rhs)
        return *this;
    int number = number_;
    setNumber(number -= rhs.number_);
    return *this;
}

Saturation& Saturation::operator++() {
    setNumber(++number_);
    return *this;
}

Saturation Saturation::operator++(int) {
    Saturation tmp(*this);
    ++*this;
    return tmp;
}
Saturation& Saturation::operator--() {
    setNumber(--number_);
    return *this;
}

Saturation Saturation::operator--(int) {
    Saturation tmp(*this);
    --*this;
    return tmp;
}

void Saturation::setNumber(int number) {
    number_ = qBound(0, number, 100);
}

QString Saturation::percent() const {
    return QString::number(number_) + tr("%");
}

// friends

QDataStream& operator<<(QDataStream& out, const Saturation& sat) {
    out << sat.number_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Saturation& sat) {
    in >> sat.number_;
    return in;
}

Saturation operator+(Saturation lhs, const Saturation& rhs) {
    return lhs += rhs;
}

Saturation operator-(Saturation lhs, const Saturation& rhs) {
    return lhs -= rhs;
}

bool operator==(const Saturation& lhs, const Saturation& rhs) {
    return lhs.number_ == rhs.number_;
}

bool operator!=(const Saturation& lhs, const Saturation& rhs) {
    return lhs.number_ != rhs.number_;
}

bool operator<(const Saturation& lhs, const Saturation& rhs) {
    return lhs.number_ < rhs.number_;
}

bool operator<=(const Saturation& lhs, const Saturation& rhs) {
    return lhs.number_ <= rhs.number_;
}

bool operator>(const Saturation& lhs, const Saturation& rhs) {
    return lhs.number_ > rhs.number_;
}

bool operator>=(const Saturation& lhs, const Saturation& rhs) {
    return lhs.number_ >= rhs.number_;
}

}

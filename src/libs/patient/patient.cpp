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

#include "patient.h"

#include "fileutilities.h"

#include <QDir>

using EpPatient::Patient;
using EpPatient::Saturation;

const QString Patient::fileName_ = "patient.dat";

Patient::Patient() : path_(0), 
    o2Saturation_(0) {}

void Patient::load() {
    EpCore::loadData(filePath(), MagicNumber, *this);
}

void Patient::save() {
    EpCore::saveData(filePath(), MagicNumber, *this);
}

Saturation Patient::o2Saturation() {
    /// TODO calculate o2Saturation here
    o2Saturation_ = o2Saturation_ + 1;
    return o2Saturation_;
}

QString Patient::filePath() {
    return QDir::cleanPath(path_ + "/" + fileName_);
}

Patient::~Patient() {}

namespace EpPatient {

// friends

QDataStream& operator<<(QDataStream& out, const Patient& patient) {
    out << patient.path_ << patient.o2Saturation_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Patient& patient) {
    in >> patient.path_ >> patient.o2Saturation_;
    return in;
}

}

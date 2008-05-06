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

#include "studyconfiguration.h"

#include <QDataStream>

using EpStudy::StudyConfiguration;

namespace EpStudy {

QDataStream& operator<<(QDataStream& out, const Channel&) {
    return out;
}

QDataStream& operator>>(QDataStream& in, Channel&) {
    return in;
}

QDataStream& operator<<(QDataStream& out, const Protocol&) {
    return out;
}

QDataStream& operator>>(QDataStream& in, Protocol&) {
    return in;
}

// QDataStream& operator<<(QDataStream&, const Channel&) {
//     return out;
// }
// 
// QDataStream& operator>>(QDataStream&, Channel&) {
//     return in;
// }


QDataStream& operator<<(QDataStream& out, const StudyConfiguration& studyConfig) {
    out << studyConfig.name_ << studyConfig.protocolList_ 
        << studyConfig.channelList_;
    return out;
}

QDataStream& operator>>(QDataStream& in, StudyConfiguration& studyConfig) {
    in >> studyConfig.channelList_>> studyConfig.protocolList_
        >> studyConfig.name_;
    return in;
}

}

const QString configFileName_ = "config.dat";


StudyConfiguration::StudyConfiguration(const QString& name) : name_(name) {}


StudyConfiguration::~StudyConfiguration() {}

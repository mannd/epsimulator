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

#include "fileutilities.h"
#include "options.h"

#include <QDataStream>

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

QDataStream& operator<<(QDataStream& out, const StudyConfiguration& studyConfig) {
    out << studyConfig.name_;
    //<< studyConfig.protocolList_ << studyConfig.channelList_;
    return out;
}

QDataStream& operator>>(QDataStream& in, StudyConfiguration& studyConfig) {
    //in >> studyConfig.channelList_>> studyConfig.protocolList_
    in >> studyConfig.name_;
    return in;
}

/// TODO StudyConfiguration in Study class is just the config.name(), i.e. a QString.
/// We can continue this, but we need to write the configuration to a file
/// in the Study file folder.  That way the configuration can be changed on the fly
/// during the procedure without necessarily saving a new System study configuration.
/// So, when a study configuration is chosen, it must be written to disk.
/// Need saveStudyConfiguration and loadStudyConfiguration functions in Study.

StudyConfiguration::StudyConfiguration(const QString& name) : name_(name) {}

StudyConfiguration::~StudyConfiguration() {}

StudyConfigList readStudyConfigurations() {
    StudyConfigList configList;
    EpCore::loadSystemData(magicNumber, configFileName_, 
                           configList, EpCore::Options::instance());
    if (configList.isEmpty()) {
        StudyConfiguration config;
        configList.append(config);
        writeStudyConfigurations(configList);
    }
    return configList;
}

void writeStudyConfigurations(StudyConfigList configList) {
    EpCore::saveSystemData(magicNumber, configFileName_, 
                           configList, EpCore::Options::instance());
}

}

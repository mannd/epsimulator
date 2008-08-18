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

QDataStream& operator<<(QDataStream& out, const Channel& channel) {
    out << (qint32)channel.number_ << channel.label_ << (qint16)channel.clip_ 
        << channel.displayPages_ << channel.alwaysSave_ << (qint16)channel.type_ 
        << (qint32)channel.posInput_ << (qint32)channel.negInput_ 
        << (qint32)channel.gain_ << channel.highPassFilter_ 
        << channel.lowPassFilter_ 
        << channel.notchFilter_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Channel& channel) {
    qint16 clip, type;
    in >> channel.number_ >> channel.label_ >> clip 
       >> channel.displayPages_ >> channel.alwaysSave_ >> type 
       >> channel.posInput_ >> channel.negInput_ 
       >> channel.gain_ >> channel.highPassFilter_ 
       >> channel.lowPassFilter_ 
       >> channel.notchFilter_;
    channel.clip_ = static_cast<Channel::Clip>(clip);
    channel.type_ = static_cast<Channel::Type>(type);
    return in;
}

Channel::Channel(int number) : number_(number), label_(QString()),
    clip_(NoClip), color_(Qt::white), displayPages_(QBitArray()),
    alwaysSave_(false), type_(NotUsed), posInput_(0), negInput_(0),
    gain_(500), highPassFilter_(0), lowPassFilter_(0), notchFilter_(false) {} 

QDataStream& operator<<(QDataStream& out, const ColumnFormat& format) {
    out << format.name_;
    return out;
}

QDataStream& operator>>(QDataStream& in, ColumnFormat& format) {
    in >> format.name_;
    return in;
}

QDataStream& operator<<(QDataStream& out, const WindowSettings& settings) {
    out << settings.name_;
    return out;
}

QDataStream& operator>>(QDataStream& in, WindowSettings& settings) {
    in >> settings.name_;
    return in;
}

QDataStream& operator<<(QDataStream& out, const MacroList& m) {
    out << m.name_;
    return out;
}

QDataStream& operator>>(QDataStream& in, MacroList& m) {
    in >> m.name_;
    return in;
}

QDataStream& operator<<(QDataStream& out, const Protocol& p) {
    out << p.name_ << p.senseChannel_ << p.columnFormat_ 
        << p.windowSettings_ << p.macroList_ << p.updateReviewWindow_
        << p.focalPoint_ << p.displayPage_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Protocol& p) {
    in >> p.name_ >> p.senseChannel_ >> p.columnFormat_
       >> p.windowSettings_ >> p.macroList_ >> p.updateReviewWindow_
       >> p.focalPoint_ >> p.displayPage_;
    return in;
}

QDataStream& operator<<(QDataStream& out, const StudyConfiguration& studyConfig) {
    out << studyConfig.name_ << studyConfig.protocolList_
        << studyConfig.channelList_;
    return out;
}

QDataStream& operator>>(QDataStream& in, StudyConfiguration& studyConfig) {
    in >> studyConfig.name_ >> studyConfig.protocolList_
       >> studyConfig.channelList_;
    return in;
}

/// TODO StudyConfiguration in Study class is just the config.name(), i.e. a QString.
/// We can continue this, but we need to write the configuration to a file
/// in the Study file folder.  That way the configuration can be changed on the fly
/// during the procedure without necessarily saving a new System study configuration.
/// So, when a study configuration is chosen, it must be written to disk.
/// Need saveStudyConfiguration and loadStudyConfiguration functions in Study.

const QString StudyConfiguration::configFileName_ = "config.dat";


StudyConfiguration::StudyConfiguration(const QString& name) : name_(name) {}

StudyConfiguration::StudyConfiguration(const StudyConfiguration& config) {
    name_ = config.name_;
    protocolList_ = config.protocolList_;
    channelList_ = config.channelList_;
}

//StudyConfiguration::~StudyConfiguration() {}

StudyConfigList readStudyConfigurations() {
    StudyConfigList configList;
    EpCore::loadSystemData(StudyConfiguration::MagicNumber, 
                           StudyConfiguration::configFileName_, 
                           configList, EpCore::Options::instance());
    if (configList.isEmpty()) {
        StudyConfiguration config;
        configList.append(config);
        writeStudyConfigurations(configList);
    }
    return configList;
}

void writeStudyConfigurations(StudyConfigList configList) {
    EpCore::saveSystemData(StudyConfiguration::MagicNumber, 
                           StudyConfiguration::configFileName_, 
                           configList, EpCore::Options::instance());
}

}

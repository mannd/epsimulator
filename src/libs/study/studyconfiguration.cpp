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

#include "amplifier.h"
#include "fileutilities.h"
#include "options.h"

#include <QDataStream>

namespace EpStudy {

using EpCore::Options;
using EpHardware::EpAmplifier::Amplifier;

QDataStream& operator<<(QDataStream& out, const Channel& channel) {
    out << (qint32)channel.number_ << (qint16)channel.clip_ 
        << channel.displayPages_ << channel.alwaysSave_; 
    return out;
}

QDataStream& operator>>(QDataStream& in, Channel& channel) {
    qint16 clip;
    in >> channel.number_ >> clip 
       >> channel.displayPages_ >> channel.alwaysSave_;
    channel.clip_ = static_cast<Channel::Clip>(clip);
    return in;
}

Channel::Channel(int number) : number_(number), clip_(NoClip), 
			       color_(Qt::white), displayPages_(QBitArray()),
			       alwaysSave_(false) {} 

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

const QString StudyConfiguration::configFileName_ = "config.dat";

StudyConfiguration::StudyConfiguration(const QString& name) : name_(name) {
    amplifier_ = new Amplifier(Options::instance()->numChannels);
}

StudyConfiguration::StudyConfiguration(const StudyConfiguration& rhs) {
    copyStudyConfiguration(rhs);
}

StudyConfiguration::~StudyConfiguration() {
    delete amplifier_;
}

StudyConfiguration& StudyConfiguration::operator=(const StudyConfiguration& rhs) {
    if (this == &rhs)
        return *this;
    copyStudyConfiguration(rhs);
    return *this;
}

Channel& StudyConfiguration::channel(int n) {
    n = qBound(1, n, amplifier_->numChannels());
    return channelList_[n-1];
}

void StudyConfiguration::copyStudyConfiguration(const StudyConfiguration& rhs) {
    name_ = rhs.name_;
    protocolList_ = rhs.protocolList_;
    channelList_ = rhs.channelList_;
    amplifier_ = new Amplifier(*rhs.amplifier_);
}

StudyConfigList readStudyConfigurations() {
    StudyConfigList configList;
    EpCore::loadSystemData(StudyConfiguration::MagicNumber, 
                           StudyConfiguration::configFileName(), 
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
                           StudyConfiguration::configFileName(), 
                           configList, EpCore::Options::instance());
}

}
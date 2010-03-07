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

#include <QtCore/QDataStream>

#include <QtDebug>

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
        << p.windowSetting_ << p.macroList_ << p.updateReviewWindow_
        << p.focalPoint_ << p.displayPage_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Protocol& p) {
    in >> p.name_ >> p.senseChannel_ >> p.columnFormat_
       >> p.windowSetting_ >> p.macroList_ >> p.updateReviewWindow_
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

const QString Protocol::fileName_ = "protocols.dat";

QList<Protocol> Protocol::defaultItems() {
    QList<Protocol>  protocols;
    protocols << Protocol("<default>");
    return protocols;
}

const QString StudyConfiguration::configFileName_ = "config.dat";

StudyConfiguration::StudyConfiguration(const QString& name) : name_(name),
    protocolList_(), channelList_(), currentProtocolIndex_(0) {
    amplifier_ = new Amplifier(epOptions->numChannels);
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

StudyConfigurations::StudyConfigurations()
    : configList_() {
    readStudyConfigurations();
}

void StudyConfigurations::readStudyConfigurations() {
    EpCore::loadSystemData(StudyConfiguration::MagicNumber,
                           StudyConfiguration::configFileName(),
                           configList_, epOptions);
    if (configList_.isEmpty()) {
        StudyConfiguration config;
        configList_.append(config);
        writeStudyConfigurations();
    }
}

void StudyConfigurations::writeStudyConfigurations() {
    EpCore::saveSystemData(StudyConfiguration::MagicNumber,
                           StudyConfiguration::configFileName(),
                           configList_, epOptions);
}

const StudyConfiguration& StudyConfigurations::operator [](int i) const {
    return configList_[i];
}

void StudyConfigurations::add(const StudyConfiguration& config) {
    // check for duplicates first, only adds if no duplicates,
    // otherwise does nothing.
    if (isPresent(config.name()))
        return;
    configList_.append(config);
    writeStudyConfigurations();
}

void StudyConfigurations::replace(const StudyConfiguration& config) {
    // remove study configuration with same name
    bool found = false;
    for (int i = 0; i < size(); ++i)
        if (configList_.at(i).name() == config.name()) {
            configList_.replace(i, config);
            found = true;
            break;
        }
    if (!found)
        configList_.append(config);
    writeStudyConfigurations();
}

void StudyConfigurations::remove(const QString& name) {
    bool found = false;
    for (int i = 0; i < size(); ++i)
        if (configList_.at(i).name() == name) {
            configList_.removeAt(i);
            found = true;
            break;
        }
    if (found)
        writeStudyConfigurations();
}

bool StudyConfigurations::isPresent(const QString& name) const {
    for (int i = 0; i < size(); ++i) {
        if (configList_.at(i).name() == name)
            return true;
    }
    return false;
}

// returns index of name in StudyConfigurations, -1 if not found
int StudyConfigurations::index(const QString& name) const {
    for (int i = 0; i < size(); ++i) {
        if (configList_.at(i).name() == name)
            return i;
    }
    return -1;
}

StudyConfiguration* StudyConfigurations::
        studyConfiguration(const QString& name) {
    for (int i = 0; i < size(); ++i) {
        if (configList_.at(i).name() == name)
            return new StudyConfiguration(configList_.at(i));
    }
    return 0;
}

}

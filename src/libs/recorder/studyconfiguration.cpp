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

#include "options.h"

#include <QtCore/QDataStream>
#include <QtCore/QStringList>

#include <QtDebug>

namespace EpStudy {

using EpCore::DataStream;
using EpCore::Options;

QDataStream& operator<<(QDataStream& out, const Channel& channel) {
    out << (qint16)channel.number_ << (qint16)channel.clip_ 
        << channel.color_ << channel.displayPages_ 
        << channel.alwaysSave_ << (qint16)channel.scale_
        << (qint16)channel.channelType_ << (qint16)channel.posInput_
        << (qint16)channel.negInput_ << (qint16)channel.gain_
        << channel.highPassFilter_ << channel.lowPassFilter_
        << channel.notchFilter_ ; 
    return out;
}

QDataStream& operator>>(QDataStream& in, Channel& channel) {
    qint16 clip;
    qint16 scale;
    qint16 channelType;
    in >> channel.number_ >> clip >> channel.color_
       >> channel.displayPages_ >> channel.alwaysSave_
       >> scale >> channelType >> channel.posInput_
       >> channel.negInput_ >> channel.gain_
       >> channel.highPassFilter_ >> channel.lowPassFilter_
       >> channel.notchFilter_;
    channel.clip_ = static_cast<Channel::Clip>(clip);
    channel.scale_ = static_cast<Channel::Scale>(scale);
    channel.channelType_ = static_cast<Channel::ChannelType>(channelType);
    return in;
}

Channel::Channel(int number) : number_(number), 
                               clip_(NoClip), 
			       color_(Qt::white), 
                               alwaysSave_(false),
                               channelType_(NotUsed),
                               posInput_(0),
                               negInput_(0),
                               gain_(500),
                               highPassFilter_(30.0),
                               lowPassFilter_(500.0),
                               notchFilter_(false) {} 

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
        << (qint16)p.focalPoint_ << p.displayPage_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Protocol& p) {
    qint16 focalPoint;
    in >> p.name_ >> p.senseChannel_ >> p.columnFormat_
       >> p.windowSetting_ >> p.macroList_ >> p.updateReviewWindow_
       >> focalPoint >> p.displayPage_;
    p.focalPoint_ = static_cast<Protocol::FocalPoint>(focalPoint);
    return in;
}

QDataStream& operator<<(QDataStream& out,
                        const StudyConfiguration& studyConfig) {
    out << studyConfig.name_ << studyConfig.numChannels_
        << studyConfig.protocolList_ 
        << studyConfig.channelList_
        << (qint16)studyConfig.currentProtocolIndex_
        << (qint16)studyConfig.surfaceLeadChannel1Index_
        << (qint16)studyConfig.surfaceLeadChannel2Index_
        << (qint16)studyConfig.surfaceLeadChannel3Index_
        << (qint16)studyConfig.atrialChannelIndex_
        << (qint16)studyConfig.hisBundleChannelIndex_
        << (qint16)studyConfig.ventricularChannelIndex_
        << (qint16)studyConfig.triggerChannelIndex_
        << (qint16)studyConfig.minIntervalBetweenStimTrains_
        << (qint16)studyConfig.hrChannel1HighThreshold_
        << (qint16)studyConfig.hrChannel1LowThreshold_
        << (qint16)studyConfig.hrChannel2HighThreshold_
        << (qint16)studyConfig.hrChannel2LowThreshold_
        << studyConfig.saveUsingHrChannel1_
        << studyConfig.saveUsingHrChannel2_
        << studyConfig.saveForStimDetection_
        << studyConfig.monitoringBPIndex_
        << (qint16)studyConfig.vitalLogBPIndex_
        << (qint16)studyConfig.hrChannel1Index_
        << (qint16)studyConfig.hrChannel2Index_
        << studyConfig.useAutoNbp_
        << (qint16)studyConfig.nbpTimeInterval_
        << (qint16)studyConfig.cuffSize_
        << studyConfig.useAutoVitalLog_
        << (qint16)studyConfig.vitalLogTimeInterval_;
    return out;
}

QDataStream& operator>>(QDataStream& in, StudyConfiguration& studyConfig) {
    qint16 currentProtocolIndex;
    qint16 surfaceLeadChannel1Index;
    qint16 surfaceLeadChannel2Index;
    qint16 surfaceLeadChannel3Index;
    qint16 atrialChannelIndex;
    qint16 hisBundleChannelIndex;
    qint16 ventricularChannelIndex;
    qint16 triggerChannelIndex;
    qint16 minIntervalBetweenStimTrains;
    qint16 hrChannel1HighThreshold;
    qint16 hrChannel1LowThreshold;
    qint16 hrChannel2HighThreshold;
    qint16 hrChannel2LowThreshold;
    qint16 vitalLogBPIndex;
    qint16 hrChannel1Index;
    qint16 hrChannel2Index;
    qint16 nbpTimeInterval;
    qint16 cuffSize;
    qint16 vitalLogTimeInterval;

    
    in >> studyConfig.name_ >> studyConfig.protocolList_
       >> studyConfig.channelList_
       >> currentProtocolIndex
       >> surfaceLeadChannel1Index
       >> surfaceLeadChannel2Index
       >> surfaceLeadChannel3Index
       >> atrialChannelIndex
       >> hisBundleChannelIndex
       >> ventricularChannelIndex
       >> triggerChannelIndex
       >> minIntervalBetweenStimTrains
       >> hrChannel1HighThreshold
       >> hrChannel1LowThreshold
       >> hrChannel2HighThreshold
       >> hrChannel2LowThreshold
       >> studyConfig.saveUsingHrChannel1_
       >> studyConfig.saveUsingHrChannel2_
       >> studyConfig.saveForStimDetection_
       >> studyConfig.monitoringBPIndex_
       >> vitalLogBPIndex
       >> hrChannel1Index
       >> hrChannel2Index
       >> studyConfig.useAutoNbp_
       >> nbpTimeInterval
       >> cuffSize
       >> studyConfig.useAutoVitalLog_
       >> vitalLogTimeInterval;
    studyConfig.currentProtocolIndex_ = currentProtocolIndex;
    studyConfig.surfaceLeadChannel1Index_ = surfaceLeadChannel1Index;
    studyConfig.surfaceLeadChannel2Index_ = surfaceLeadChannel2Index;
    studyConfig.surfaceLeadChannel3Index_ = surfaceLeadChannel3Index;
    studyConfig.atrialChannelIndex_ = atrialChannelIndex;
    studyConfig.hisBundleChannelIndex_ = hisBundleChannelIndex;
    studyConfig.ventricularChannelIndex_ = ventricularChannelIndex;
    studyConfig.triggerChannelIndex_ = triggerChannelIndex;
    studyConfig.minIntervalBetweenStimTrains_ = minIntervalBetweenStimTrains;
    studyConfig.hrChannel1HighThreshold_ = hrChannel1HighThreshold;
    studyConfig.hrChannel1LowThreshold_ = hrChannel1LowThreshold;
    studyConfig.hrChannel2HighThreshold_ = hrChannel2HighThreshold;
    studyConfig.hrChannel2LowThreshold_ = hrChannel2LowThreshold;
    studyConfig.vitalLogBPIndex_ = vitalLogBPIndex;
    studyConfig.hrChannel1Index_ = hrChannel1Index;
    studyConfig.hrChannel2Index_ = hrChannel2Index;
    studyConfig.nbpTimeInterval_ = nbpTimeInterval;
    studyConfig.cuffSize_ = cuffSize;
    studyConfig.vitalLogTimeInterval_ = vitalLogTimeInterval;
    return in;
}

const QString Protocol::fileName_ = "protocols.dat";

Protocol::Protocol(const Protocol& rhs) {
    copyProtocol(rhs);
}

Protocol& Protocol::operator =(const Protocol& rhs) {
    if (this == &rhs)
        return *this;
    copyProtocol(rhs);
    return *this;
}

void Protocol::copyProtocol(const Protocol& rhs) {
    name_ = rhs.name_;
    senseChannel_ = rhs.senseChannel_;
    columnFormat_ = rhs.columnFormat_;
    windowSetting_ = rhs.windowSetting_;
    macroList_ = rhs.macroList_;
    updateReviewWindow_ = rhs.updateReviewWindow_;
    focalPoint_ = rhs.focalPoint_;
    displayPage_ = rhs.displayPage_;
}

QStringList Protocol::protocolNames(const QList<Protocol>& protocols) {
    QListIterator<Protocol> iter(protocols);
    QStringList theNames;
    while (iter.hasNext()) {
        theNames << iter.next().name();
    }
    return theNames;
}

QList<Protocol> Protocol::defaultItems() {
    QList<Protocol>  protocols;
    return protocols << Protocol(tr("<default>"));
}

const QString StudyConfiguration::configFileName_ = "config.dat";

StudyConfiguration::StudyConfiguration(const QString& name,
                                       int numChannels)
    : name_(name), numChannels_(numChannels),
      protocolList_(Protocol::defaultItems()),
      channelList_(), currentProtocolIndex_(0) {}

StudyConfiguration::StudyConfiguration(const StudyConfiguration& rhs) {
    copyStudyConfiguration(rhs);
}

    StudyConfiguration::~StudyConfiguration() {}

StudyConfiguration& StudyConfiguration::operator=(
        const StudyConfiguration& rhs) {
    if (this == &rhs)
        return *this;
    copyStudyConfiguration(rhs);
    return *this;
}

Channel& StudyConfiguration::channel(int n) {
    n = qBound(1, n, numChannels_);
    return channelList_[n-1];
}

void StudyConfiguration::copyStudyConfiguration(const StudyConfiguration& rhs) {
    name_ = rhs.name_;
    numChannels_ = rhs.numChannels_;
    protocolList_ = rhs.protocolList_;
    channelList_ = rhs.channelList_;
    currentProtocolIndex_ = rhs.currentProtocolIndex_;
}

QList<Protocol> StudyConfiguration::unselectedProtocols() const {
    return QList<Protocol>();
//    EpCore::ItemList<Protocol> protocols;
//    QList<Protocol> unselectedProtocols = protocols.list();
//    QListIterator<Protocol> iter(protocolList_);
//    while (iter.hasNext())
//        unselectedProtocols.removeAll(iter.next());
//    return unselectedProtocols;
}


}

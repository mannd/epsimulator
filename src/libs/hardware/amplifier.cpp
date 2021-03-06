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

#include "amplifier.h"

#include "fileutilities.h"

#include <QApplication>
#include <QDataStream>
#include <QtGlobal>

#include <QtDebug>

namespace EpHardware { namespace EpAmplifier {

QDataStream& operator<<(QDataStream& out, const Amplifier& amp) {
    out << (qint32)amp.numChannels_; 
    for (int i = 0; i < amp.channels_.size(); ++i)
        out << *(amp.channels_[i]);
    return out;
}

QDataStream& operator>>(QDataStream& in, Amplifier& amp) {
    in >> amp.numChannels_; 
    for (int i = 0; i < amp.channels_.size(); ++i)
        in >> *(amp.channels_[i]);
    return in;
}

const QString Amplifier::fileName_ = "amplifier.dat";


Amplifier::Amplifier(int n) : numChannels_(n) {
    Q_ASSERT(n == 32 || n == 48 || n == 64 || n == 96 || n == 128);
    // note that channels are 1 based, so that
    // channels_[0].number() ==1;
    for (EcgChannel::EcgLabel i = EcgChannel::I; i <= EcgChannel::V6;
         i = EcgChannel::EcgLabel(i + 1))
        channels_.append(new EcgChannel(i, EcgChannel::ecgLabel(i)));

    channels_.append(new PressureChannel(13, tr("P1")));
    channels_.append(new PressureChannel(14, tr("P2")));
    channels_.append(new PressureChannel(15, tr("P3")));
    channels_.append(new PressureChannel(16, tr("P4")));
    for (int i = 17; i <= numChannels_-2; ++i) 
        channels_.append(new  Channel(i));
    // last 2 channels are by default Stim1 and Stim2
    channels_.append(new  StimChannel(numChannels_-1, 
                                     tr("Stim1"), 
                                     Channel::Stim1));
    channels_.append(new  StimChannel(numChannels_, 
                                     tr("Stim2"), 
                                     Channel::Stim2));
}

Amplifier::Amplifier(const Amplifier& rhs) {
    copyAmplifier(rhs);
}


Amplifier& Amplifier::operator =(const Amplifier& rhs) {
    if (this == &rhs)
        return *this;
    copyAmplifier(rhs);
    return *this;
}

Amplifier::~Amplifier() {
    qDeleteAll(channels_);
}

void Amplifier::load(EpCore::DataStream<Amplifier>* const dataStream) {
    try {
        dataStream->load(*this);
    }
    catch (EpCore::IoError&) {
        // ignore failure to read, keep default amplifier settings
    }
}

void Amplifier::save(EpCore::DataStream<Amplifier>* const dataStream) {
    dataStream->save(*this);
}

void Amplifier::reset() {
   QApplication::beep(); 
}

Channel* Amplifier::channel(int n) {
    // adjust n if necessary
    n = qBound(1, n, numChannels_);
    return channels_[n-1];
}

void Amplifier::copyAmplifier(const Amplifier& rhs) {
    numChannels_ = rhs.numChannels_;
    for (int i = 0; i < rhs.channels_.size(); ++i) 
        channels_.append(rhs.channels_.at(i)->clone());
}

QDataStream& operator<<(QDataStream& out, const Channel& channel) {
    out << (qint32)channel.number_ << channel.label_ 
        << (qint16)channel.channelType_ 
        << (qint32)channel.posInput_ << (qint32)channel.negInput_ 
        << (qint32)channel.gain_ << channel.highPassFilter_ 
        << channel.lowPassFilter_ 
        << channel.notchFilter_;
    return out;
}

QDataStream& operator>>(QDataStream& in, Channel& channel) {
    qint16 type;
    in >> channel.number_ >> channel.label_ 
       >> type 
       >> channel.posInput_ >> channel.negInput_ 
       >> channel.gain_ >> channel.highPassFilter_ 
       >> channel.lowPassFilter_ 
       >> channel.notchFilter_;
    channel.channelType_ = static_cast<Channel::ChannelType>(type);
    return in;
}


Channel::Channel(int n, QString label) 
                 : number_(n), label_(label),
                   channelType_(NotUsed), negInput_(0),
                   posInput_(0), gain_(500), highPassFilter_(30.0),
                   lowPassFilter_(500), notchFilter_(false) {
}

QStringList EcgChannel::ecgLabels_ = QStringList() << tr("None")
                                     << tr("I") << tr("II") << tr("III")
                                     << tr("avR") << tr("avL") << tr("avF")
                                     << tr("V1") << tr("V2") << tr("V3")
                                     << tr("V4") << tr("V5") << tr("V6");

EcgChannel::EcgChannel(int n, const QString& label) 
    : Channel(n) {
    setLabel(label);
    setHighPassFilter(0.05);
    setLowPassFilter(150);
    setNotchFilter(true);
}

PressureChannel::PressureChannel(int n, const QString& label) 
    : Channel(n) {
    setLabel(label);
}

StimChannel::StimChannel(int n, const QString& label, ChannelType type)
    : Channel(n) {
    setLabel(label);
    type = qBound(Stim1, type, Stim4);
    setChannelType(type);
}

}}

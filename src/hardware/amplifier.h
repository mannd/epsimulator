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

#ifndef AMPLIFIER_H
#define AMPLIFIER_H

#include <QColor>
#include <QCoreApplication>
#include <QList>
#include <QString>

class QDataStream;

namespace EpHardware { 
    namespace EpAmplifier {

class Channel;

/**
 * Emulates the amplifier, with different number of 
 * catheter input blocks and channels.  Hardware
 * channel settings are all here.  Software settings
 * (such as Clip level) are in StudyConfiguration.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Amplifier {
    Q_DECLARE_TR_FUNCTIONS(Amplifier)
public:
    explicit Amplifier(int numChannels = 48);
    Amplifier(const Amplifier&);
    ~Amplifier();

    friend QDataStream& operator<<(QDataStream&, const Amplifier&);
    friend QDataStream& operator>>(QDataStream&, Amplifier&);

    Amplifier& operator=(const Amplifier&);

    // overloaded function to calculate number of amplifier blocks.
    // there are 12 ECG channels and 4 pressure channels = 16.
    // the rest are intracardiac channels, with 16 per input block.
    // first 12 channels are hardwired as ECG channels.
    // Next 4 are pressure channels.  
    // The rest are plain intracardiac channels.
    static int numCIMConnections(int numChannels) {
        return (numChannels - 16) / 16;}

    int numChannels() const {return numChannels_;}
    int numCIMConnections() const {
        return numCIMConnections(numChannels_);}

    Channel* channel(int n);  // return channel number n

private:
    void copyAmplifier(const Amplifier&);

    int numChannels_;
    QList<Channel*> channels_;
};

/// This is the hardware channel, Channel in StudyConfiguration includes this
/// and software settings.
class Channel {

public:
    enum ChannelType { NotUsed, ECG, Pressure, Bipolar, 
                       UnipolarWCT, UnipolarAuxRef,
                       AblateRecord, Stim1, Stim2, Stim3, Stim4 };
    Channel(int number = 1, QString label = QString());
    virtual ~Channel() {}

    friend QDataStream& operator<<(QDataStream&, const Channel&);
    friend QDataStream& operator>>(QDataStream&, Channel&);

    // determines if the channel type can be changed
    virtual bool typeModifiable() const {return true;}
    virtual Channel* clone() const {return new Channel(*this);}

    virtual ChannelType channelType() const {return channelType_;}
    int number() const {return number_;}
    QString label() const {return label_;} 
    int negInput() const {return negInput_;}
    int posInput() const {return posInput_;}
    int gain() const {return gain_;}
    double highPassFilter() const {return highPassFilter_;}
    double lowPassFilter() const {return lowPassFilter_;}
    bool notchFilter() const {return notchFilter_;}

    virtual void setChannelType(const ChannelType& type) {channelType_ = type;}
    void setLabel(const QString& label) {label_ = label;}
    void setNegInput(int n) {negInput_ = n;}
    void setPosInput(int n) {posInput_ = n;}
    void setGain(int gain) {gain_ = gain;}
    void setHighPassFilter(double freq) {highPassFilter_ = freq;}
    void setLowPassFilter(double freq) {lowPassFilter_ = freq;}
    void setNotchFilter(bool enable) {notchFilter_ = enable;}

private:
    int number_;
    QString label_; // label is the term used by Prucka, not name
    ChannelType channelType_;
    int negInput_;
    int posInput_;
    int gain_;
    double highPassFilter_;
    double lowPassFilter_;
    bool notchFilter_;
};

class EcgChannel : public Channel {
public:
    EcgChannel(int number, const QString& label);
    ~EcgChannel() {}
    
    virtual bool typeModifiable() const {return false;}
    virtual EcgChannel* clone() const {return new EcgChannel(*this);} 

    virtual ChannelType channelType() const {return ECG;}

    virtual void setChannelType(const ChannelType&) {}  // noop
};

class PressureChannel : public Channel {
public:
    PressureChannel(int number, const QString& label);
    ~PressureChannel() {}

    virtual bool typeModifiable() const {return false;}
    virtual PressureChannel* clone() const {return new PressureChannel(*this);}

    virtual ChannelType channelType() const {return Pressure;}

    virtual void setChannelType(const ChannelType&) {}  // noop

};

class StimChannel : public Channel {
public:
    StimChannel(int number, const QString& label, ChannelType type);
    ~StimChannel() {}

    virtual StimChannel* clone() const {return new StimChannel(*this);}
};

}}

#endif

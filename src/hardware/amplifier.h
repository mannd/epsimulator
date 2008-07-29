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

namespace EpHardware { 
    namespace EpAmplifier {
        class Channel;

/**
 * Emulates the amplifier, with different number of 
 * catheter input blocks and channels.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Amplifier {
    Q_DECLARE_TR_FUNCTIONS(Amplifier)
public:
    explicit Amplifier(int numChannels = 48);
    ~Amplifier();

    int numChannels() const {return numChannels_;}
    unsigned int numCIMConnections() const {return numCIMConnections_;}

private:
    int numChannels_;
    int numCIMConnections_;
    double scale_;  // 1/16 is default
    
    QList<Channel*> channels_;
};

class Channel {

public:
    enum ChannelType { NotUsed, ECG, Bipolar, UnipolarWCT, UnipolarAuxRef,
                       AblateRecord, Stim1, Stim2, Stim3, Stim4 };
    enum ClipLevel { None, C1, C2 };
    explicit Channel(int number);
    virtual ~Channel() {}

    int number() const {return number_;}
    QString label() const {return label_;}
    ChannelType channelType() const {return channelType_;}
    int negInput() const {return negInput_;}
    int posInput() const {return posInput_;}
    int gain() const {return gain_;}
    double highPassFilter() const {return highPassFilter_;}
    double lowPassFilter() const {return lowPassFilter_;}
    bool notchFilter() const {return notchFilter_;}
    ClipLevel clip() const {return clip_;} 
    bool alwaysSave() const {return alwaysSave_;}
    
    QList<bool>& displayPages() {return displayPages_;}

    void setLabel(const QString& label) {label_ = label;}
    void setChannelType(const ChannelType& type) {channelType_ = type;}
    void setNegInput(int n) {negInput_ = n;}
    void setPosInput(int n) {posInput_ = n;}
    void setGain(int gain) {gain_ = gain;}
    void setHighPassFilter(double freq) {highPassFilter_ = freq;}
    void setLowPassFilter(double freq) {lowPassFilter_ = freq;}
    void setNotchFilter(bool enable) {notchFilter_ = enable;}
    void setClip(ClipLevel level) {clip_ = level;}
    void setAlwaysSave(bool save) {alwaysSave_ = save;}

private:
    // hardware settings
    int number_;
    QString label_;
    ChannelType channelType_;
    int negInput_;
    int posInput_;
    int gain_;
    double highPassFilter_;
    double lowPassFilter_;
    bool notchFilter_;
    // display settings
    ClipLevel clip_;
    QList<bool> displayPages_;
    bool alwaysSave_;
};

class EcgChannel : public Channel {
public:
    EcgChannel(int number, QString label);
};

class PressureChannel : public Channel {};

}}

#endif

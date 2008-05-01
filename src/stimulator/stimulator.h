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

#ifndef STIMULATOR_H
#define STIMULATOR_H

#include "epdefs.h"

#include <QWidget>

#include <set>
       
namespace EpHardware { namespace EpStimulator {

using namespace Ep;

    enum Stimulus {S1 = 1, S2, S3, S4, S5};
    enum StimulusPair {S1S1, S1S2, S2S3, S3S4, S4S5};
    typedef unsigned int ChannelNumber;

/**
 * @class Channel
 * Implements features of a pacing output channel from a stimulator.
 */
class Channel {
public:
    Channel(ChannelNumber number = 1,
        Amplitude current = 0, Duration pw = 2 ) : number_(number),
        current_(current), pulseWidth_(pw) {}
    virtual ~Channel() {}
    
    ChannelNumber number() const {return number_;}
    Amplitude current() const {return current_;}
    Duration pulseWidth() const {return pulseWidth_;}

    void setNumber(ChannelNumber number) {number_ = number;}
    void setCurrent(Amplitude current) {current_ = current;}
    void setPulseWidth(Duration pw) {pulseWidth_ = pw;}

private:
    ChannelNumber number_;
    Amplitude current_;
    Duration pulseWidth_;

};

/// XXX Experimental class below
/*
    This is an abstract base class for egms, ie electrical events that
are depicted in the recorder windows.  A study consists of a series of
events over time in a series of channels.  During real-time acquistion, these
events are triggered by the heart simulation and depend on the elements
being sampled by the catheters.  During saving, the egms are streamed to 
disk, ? directly to the optical disk, or buffered and intermittently saved.
The input to each channel is either quiet (can simulate baseline wander
however) or an egm is emitted.  This can be a pacing stimulus, a single
egm (atrial egm), a complex egm (3 part egm of His recording) or a
surface ECG.  Noise, loose connections can also be simulated, though 
probably not as individual egsm.  A realtime or review window must be able
to display an egm.  Possibly it can display itself, although the display
must be affected by the channel settings: amplification, clipping, sweep
speed.  E.g., an atrial EGM occurs at time 1122 msec into the study, 
The duration, amplitude and the egm template are combined with the
sweep speed, amplification, etc. to actually draw the egm.  All egms start
with a 0 deflection, then add or subtract to give the actual egm appearance.
The study is saved by recording these deflections.  The signals are compressed
by only recording the differences over time.  Note that the raw signals are
saved, things like sweep speed,etc. can be changed in the playback.  So:
    898:3:+25:8:-22
    899:3:+33:8:-15:9:3
    etc.
Times without changes are not recorded, and no change is made in any amplitude.
Eg.
    Until 898 msec, all channels are flat (0)
    After 898, channel 3 increases, 8 decreases, then increases, etc.

*/
class Egm {
public:
    Egm();
    virtual ~Egm();

private:
    // onset time ?? msec after start of study
    // amplitude
    // duration
    // image

};

/**
 * @class Stimulator
 * Abstract base class for custom stimulators.  Derive concrete 
 * stimulators from this class and a private user interface class,
 * 
 *      class NewStimulator : public Stimulator, private Ui::NewStimulator
 * It may be possible later on to implement this as a plugin, using
 * this class as an interface.  See Qt Plugins in Qt manual.
 */
class Stimulator : public QWidget {
    Q_OBJECT
public:
    enum {MaxChannel = 4};  // 
    enum Channel {One = 1, Two, Three, Four};
    typedef std::set<Channel> ChannelSet;
    virtual ~Stimulator() = 0;

    void setS1S1(msec s1S1) {s1S1_ = s1S1;}
    void setS1S2(msec s1S2) {s1S2_ = s1S2;}
    void setS2S3(msec s2S3) {s2S3_ = s2S3;}
    void setS3S4(msec s3S4) {s3S4_ = s3S4;}
    void setS4S5(msec s4S5) {s4S5_ = s4S5;}
    void setDelay(msec delay) {delay_ = delay;}
    void setPauseToggle(bool enable) {pauseToggle_ = enable;}
    void setSyncToggle(bool enable) {syncToggle_ = enable;}

    msec s1S1() const {return s1S1_;}
    msec s1S2() const {return s1S2_;}
    msec s2S3() const {return s2S3_;}
    msec s3S4() const {return s3S4_;}
    msec s4S5() const {return s4S5_;}
    msec delay() const {return delay_;}
    bool pauseToggle() const {return pauseToggle_;}
    bool syncToggle() const {return syncToggle_;}
    
protected:
    Stimulator(QWidget* parent = 0, Qt::WindowFlags f1 = Qt::Tool);

protected slots:
    void activate();
    void inactivate();
    void activateChannel(Channel);
    void inactivateChannel(Channel);
    void sendStimulus();

signals:
    void s1(ChannelSet);
    void s2(ChannelSet);
    void s3(ChannelSet);
    void s4(ChannelSet);
    void s5(ChannelSet);
    void stimulus(ChannelSet);

private:
    msec s1S1_;
    msec s1S2_; 
    msec s2S3_;
    msec s3S4_;
    msec s4S5_;

    msec delay_;

    bool pauseToggle_;  
    bool syncToggle_;

    Volt channel1Output_;
    Volt channel2Output_;
    Volt channel3Output_;
    Volt channel4Output_;
    Volt channel5Output_;

    ChannelSet activeChannels_;

    

    QTimer* timer_;


};

}}

#endif

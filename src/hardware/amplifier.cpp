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

#include <QtGlobal>

using namespace EpHardware::EpAmplifier;

Amplifier::Amplifier(int n) : numChannels_(n) {
    Q_ASSERT(n == 48 || n == 64 || n == 96 || n == 128);
    switch (numChannels_) {
        case 64 : numCIMConnections_ = 3; break;
        case 96 : numCIMConnections_ = 5; break;
        case 128 : numCIMConnections_ = 7; break;
        case 48 : 
        default: numCIMConnections_ = 2;
    }
    // first 12 channels are hardwired as ECG channels.
    channels_.append(new EcgChannel(1, tr("I")));
    channels_.append(new EcgChannel(2, tr("II")));
    channels_.append(new EcgChannel(3, tr("III")));
    channels_.append(new EcgChannel(4, tr("avR")));
    channels_.append(new EcgChannel(5, tr("avL")));
    channels_.append(new EcgChannel(6, tr("avF")));
    channels_.append(new EcgChannel(7, tr("V1")));
    channels_.append(new EcgChannel(8, tr("V2")));
    channels_.append(new EcgChannel(9, tr("V3")));
    channels_.append(new EcgChannel(10, tr("V4")));
    channels_.append(new EcgChannel(11, tr("V5")));
    channels_.append(new EcgChannel(12, tr("V6")));
}

Amplifier::~Amplifier() {}

Channel::Channel(int n) : number_(n), label_(QString()),
                          channelType_(NotUsed), negInput_(0),
                          posInput_(0), gain_(500), highPassFilter_(30.0),
                          lowPassFilter_(500), notchFilter_(false),
                          clip_(None), // displayPages(??),
                          alwaysSave_(false) {
}

EcgChannel::EcgChannel(int n, QString label) 
    : Channel(n) {
    setLabel(label);
    displayPages()[1] = true;
    setHighPassFilter(0.05);
    setLowPassFilter(150);
    setNotchFilter(true);
}





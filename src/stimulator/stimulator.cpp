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

#include "stimulator.h"

#include <QTimer>

using EpHardware::EpStimulator::Stimulator;

Stimulator::Stimulator(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl), s1S1_(0), s1S2_(0), s2S3_(0),
    s3S4_(0), s4S5_(0), delay_ (3000), pauseToggle_(false),
    syncToggle_(false) {
    timer_ = new QTimer(this);

}

Stimulator::~Stimulator() {}

void Stimulator::inactivate() {
    timer_->stop();
}

void Stimulator::activate() {
    connect(timer_, SIGNAL(timeout()), this, SLOT(sendStimulus()));
    //if (!activeChannels_.empty())
        timer_->start(s1S1_);
}

void Stimulator::sendStimulus() {
    emit stimulus(activeChannels_);
};

void Stimulator::activateChannel(Channel c) {
    activeChannels_.insert(c);
}

void Stimulator::inactivateChannel(Channel c) {
    activeChannels_.erase(c);
}

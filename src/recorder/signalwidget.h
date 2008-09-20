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

#ifndef EPRECORDERSIGNALWIDGET_H
#define EPRECORDERSIGNALWIDGET_H

#include "amplifier.h"

#include <QLabel>
#include <QList>
#include <QScrollArea>
#include <QSplitter>

class QFrame;

namespace EpRecorder {

using EpHardware::EpAmplifier::Channel;

/**
 * Channel label in Channel Label Area of SignalWidget.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class ChannelLabel : public QLabel {
public:
    ChannelLabel(int number, QWidget* parent = 0);

private:
    Channel channel_;
};

/**
 * Central widget of SignalDisplayWindow, 
 * used by RealTimeWindow, ReviewWindow, etc.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class SignalWidget : public QSplitter {
public:
    SignalWidget(QWidget* parent = 0);
    ~SignalWidget() {}

private:
    QList<ChannelLabel*> channelLabels_;
    QFrame* channelLabelArea_;
    QFrame* signalArea_;
};

}

#endif

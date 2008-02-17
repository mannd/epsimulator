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

#ifndef SIGNALDISPLAYWINDOW_H
#define SIGNALDISPLAYWINDOW_H

#include <QFrame>
#include <QMainWindow>

class ChannelBar;

/**
 * Abstract base class for RealTimeWindow and ReviewWindow.  Implements common 
 * funcionality, such as the ChannelBar, but does not implement toolbars and 
 * updating of signal displays.
 *
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class SignalDisplayWindow : public QMainWindow  {
    Q_OBJECT
public:
    SignalDisplayWindow(QWidget* parent = 0);

    void setChannelBar(ChannelBar* cb) {channelBar_ = cb;}

    ChannelBar* channelBar() const {return channelBar_;}

    virtual void createCentralWidget();
    virtual void createActions() = 0;
    virtual void createToolBars() = 0;

    virtual void saveSettings() = 0;
    virtual void readSettings() = 0;

    ~SignalDisplayWindow();
private:
    ChannelBar* channelBar_;
};

/**
 * This is the channel label area.  This is an ABC, mostly so that
 * the right click menu can be overriden, as it is different on 
 * the real-time vs the review window.
 */
class ChannelBar : public QFrame {
    Q_OBJECT
public:
    ChannelBar(QWidget* parent = 0);

    void setUp();

    ~ChannelBar();
};

/**
 * This is the black signal area to the right of the channel bar.
 */
class SignalArea : public QWidget {
    Q_OBJECT
public:
    SignalArea(QWidget* parent = 0);

    ~SignalArea();
};

#endif

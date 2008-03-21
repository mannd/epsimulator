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

#ifndef REVIEWWINDOW_H
#define REVIEWWINDOW_H

#include "displaywindow.h"

class QAction;
class QComboBox;
class Settings;


/**
This window is used to review already saved signal.  You can scroll through the data, or click on a time-stamp in the log window and bring up a window of signals.  This behaves like the RealTimeWindow, except there is no live updating of signals.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class ReviewWindow : public SignalDisplayWindow  {
    Q_OBJECT
public:
    ReviewWindow(int number = 1, QWidget *parent = 0);

    virtual void writeSettings(Settings&);
    virtual void readSettings(const Settings&);
    virtual QString key() {return QString("/review%1Window").arg(number());}

    ~ReviewWindow();

    virtual void createToolBars();
    virtual void createActions();


public slots:
    //virtual void updateWindowTitle();
    void increaseSweepSpeed();
    void decreaseSweepSpeed();

signals:
    void startTimer(bool);
    void startStopwatch(bool);

private:
    QComboBox* sweepSpeedComboBox_;

    QAction* minusAct_;
    QAction* plusAct_;
    QAction* studyConfigAct_;
    QAction* timeCalipersAct_;
    QAction* amplitudeCalipersAct_;
    QAction* deleteAllCalipersAct_;
    QAction* msCalipersAct_;
    QAction* bpmCalipersAct_;
    QAction* makeWindowActiveAct_;
    QAction* updateWindowAct_;
    QAction* offsetSignalsAct_;
    QAction* snapshotAct_;
    QAction* realTime12LeadAct_;
    QAction* printAct_;
    QAction* createLogEventAct_;
    QAction* timerAct_;
    QAction* stopwatchAct_;  

};

// class Review2Window : public ReviewWindow {
// public:
//     Review2Window() : ReviewWindow(2) {}
//     ~Review2Window() {}
// };

#endif

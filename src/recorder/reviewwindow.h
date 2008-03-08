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

#include <signaldisplaywindow.h>

class QAction;
class QComboBox;


/**
This window is used to review already saved signal.  You can scroll through the data, or click on a time-stamp in the log window and bring up a window of signals.  This behaves like the RealTimeWindow, except there is no live updating of signals.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class ReviewWindow : public SignalDisplayWindow  {
    Q_OBJECT
public:
    ReviewWindow(int number = 1, QWidget *parent = 0);

    virtual void saveSettings();
    virtual void readSettings();


    ~ReviewWindow();

    virtual void createToolBars();
    virtual void createActions();


public slots:
    virtual void updateWindowTitle();

private:
    int number_;    // only 2 review windows allowed, 1 or 2

    QComboBox* sweepSpeedComboBox_;

    QAction* minusAct_;
    QAction* plusAct_;
    QAction* studyConfigAct_;
    QAction* timeCalipersAct_;
    QAction* amplitudeCalipersAct_;
    QAction* deleteAllCalipersAct_;
    QAction* msCalipersAct_;
    QAction* bpmCalipersAct_;
    QAction* offsetSignalsAct_;
    QAction* triggeredModeAct_;
    QAction* toggleAblationWindowAct_;
    QAction* realTime12LeadAct_;
    QAction* timerAct_;
    QAction* stopwatchAct_;  

};

#endif

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
class QCloseEvent;
class QComboBox;
class QSettings;

namespace EpCore {
    class Options;
}

namespace EpGui {
    class StudyConfigurationDialog;
}

namespace EpStudy {
    class Study;
}

namespace EpRecorder {

using EpGui::StudyConfigurationDialog;

/**
This window is used to review already saved signal.
You can scroll through the data, or click on a time-stamp in the log window
and bring up a window of signals.
This behaves like the RealTimeWindow,
except there is no live updating of signals.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class ReviewWindow : public SignalDisplayWindow  {
    Q_OBJECT
public:
    ReviewWindow(EpStudy::Study*, 
		 const EpCore::Options* const options,
		 int number = 1, 
		 QWidget *parent = 0);
    ~ReviewWindow();

    virtual void writeSettings(QSettings&);
    virtual void readSettings(QSettings&);
    virtual QString key() const {
	return QString("review%1Window").arg(number());
    }

    virtual void createToolBars();
    virtual void createActions();


public slots:
    //virtual void updateWindowTitle();
    void increaseSweepSpeed();
    void decreaseSweepSpeed();
    void makeWindowActive(bool);
    void otherWindowActive(bool);
    void updateToolBars();

protected:
    virtual StudyConfigurationDialog* studyConfigurationDialog();

signals:
    void startTimer(bool);
    void startStopwatch(bool);
    void windowActivated(bool);
    void windowClosing(bool);

private:
    void closeEvent(QCloseEvent*);

    bool windowActive_; // this is window that updates go to

    QComboBox* sweepSpeedComboBox_;

    QAction* minusAction_;
    QAction* plusAction_;
    QAction* studyConfigAction_;
    QAction* timeCalipersAction_;
    QAction* amplitudeCalipersAction_;
    QAction* deleteAllCalipersAction_;
    QAction* msCalipersAction_;
    QAction* bpmCalipersAction_;
    QAction* makeWindowActiveAction_;
    QAction* updateWindowAction_;
    QAction* offsetSignalsAction_;
    QAction* snapshotAction_;
    QAction* realTime12LeadAction_;
    QAction* printAction_;
    QAction* createLogEventAction_;
    QAction* timerAction_;
    QAction* stopwatchAction_;  
};

}

#endif

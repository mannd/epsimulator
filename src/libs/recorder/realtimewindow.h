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
#ifndef REALTIMEWINDOW_H
#define REALTIMEWINDOW_H

#include "displaywindow.h"
#include "recorderdefs.h"

#include <QString>
#include <QPushButton>

class QAction;
class QComboBox;
class QSettings;

namespace EpGui {
    class RealTimeStudyConfigurationDialog;
}

namespace EpRecorder {

    class ColorButton : public QPushButton {
        Q_OBJECT
    public:
        ColorButton(QColor activeColor, QWidget* parent=0);
        bool activated() const {return activated_;}
        void setActivated(bool activated=true);
    public slots:
        void changeState();
    private:
       QPalette originalPalette_;
       QPalette activatedPalette_;
       bool activated_;
    };

/**
The real-time recording window, central widget of recorder.  Uses multiple inheritance to provide a toolbar.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class RealTimeWindow : public SignalDisplayWindow  {
    Q_OBJECT
public:
    RealTimeWindow(EpStudy::Study* study, int number = 0, QWidget* parent = 0);
    
    virtual void writeSettings(QSettings&);
    virtual void readSettings(QSettings&);
    virtual QString key() const {return EpRecorder::realTimeWindowKey;}

    ~RealTimeWindow();

public slots:
//     virtual void updateWindowTitle();

protected:
    virtual EpGui::StudyConfigurationDialog* studyConfigurationDialog();

signals:
    void startTimer(bool);
    void startStopwatch(bool);

private slots:
    void increaseSweepSpeed();
    void decreaseSweepSpeed();

private:
    virtual void createActions();
    virtual void createToolBars();

    QComboBox* sweepSpeedComboBox_;

    QAction* minusAction_;
    QAction* plusAction_;
    QAction* studyConfigAction_;
    QAction* timeCalipersAction_;
    QAction* amplitudeCalipersAction_;
    QAction* deleteAllCalipersAction_;
    QAction* msCalipersAction_;
    QAction* bpmCalipersAction_;
    QAction* offsetSignalsAction_;
    QAction* triggeredModeAction_;
    QAction* toggleAblationWindowAction_;
    QAction* realTime12LeadAction_;
    QAction* timerAction_;
    QAction* stopwatchAction_;  
};

}

#endif

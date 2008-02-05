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

#include <QMainWindow>

class QAction;
class QComboBox;

/**
The real-time recording window, central widget of recorder.  Uses multiple inheritance to provide a toolbar.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class RealTimeWindow : public QMainWindow  {
    Q_OBJECT
public:
    RealTimeWindow(QWidget* parent = 0);

    ~RealTimeWindow();

private:
    void createActions();
    void createToolBars();

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
    QAction* updateWindowToggleAct_;    

};

#endif

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

#include "realtimewindow.h"

#include "actions.h"

#include <QAction>
#include <QComboBox>
#include <QIcon>
#include <QPalette>
#include <QToolBar>

using EpGui::createAction;

RealTimeWindow::RealTimeWindow(QWidget* parent)
 : QMainWindow(parent) {
    setWindowTitle(tr("Real-Time Page 1"));
    QWidget* centralWidget = new QWidget;
    centralWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    centralWidget->setPalette(palette);
    setCentralWidget(centralWidget);
    createActions();
    createToolBars();
 
    
}

void RealTimeWindow::createToolBars() {
    QToolBar* toolBar = new QToolBar(this);
    toolBar->setAutoFillBackground(true);
    toolBar->addAction(minusAct_);
    toolBar->addSeparator();
    sweepSpeedComboBox_ = new QComboBox(this);
    sweepSpeedComboBox_->addItem(tr("400   "));
    sweepSpeedComboBox_->addItem(tr("200   "));
    sweepSpeedComboBox_->addItem(tr("100   "));
    sweepSpeedComboBox_->addItem(tr("50    "));
    sweepSpeedComboBox_->addItem(tr("25    "));
    toolBar->addWidget(sweepSpeedComboBox_);
    toolBar->addSeparator();
    toolBar->addAction(plusAct_);
    toolBar->addAction(studyConfigAct_);
    toolBar->addSeparator();
    toolBar->addAction(timeCalipersAct_);
    toolBar->addAction(amplitudeCalipersAct_);
    toolBar->addAction(deleteAllCalipersAct_);
    toolBar->addAction(msCalipersAct_);
    toolBar->addAction(bpmCalipersAct_);
    toolBar->addSeparator();
    toolBar->addAction(makeWindowActiveAct_);
    toolBar->addAction(updateWindowToggleAct_);   

    addToolBar(toolBar); 
}

void RealTimeWindow::createActions() {
    minusAct_ = createAction(this, tr("Minus"), 
        tr("Decrease sweep speed"), 0, 0, "hi32-minus.png"); 
    plusAct_ = createAction(this, tr("Plus"),
        tr("Increase sweep speed"), 0, 0, "hi32-plus.png");
    studyConfigAct_ = createAction(this, tr("Study Configuration"),
        tr("Open study configuration"), 0, 0, "hi32-studyconfig.png");
    timeCalipersAct_ = createAction(this, tr("Time Calipers"),
        tr("Time calipers"), 0, 0, "hi32-timecalipers.png");
    timeCalipersAct_->setCheckable(true);
    amplitudeCalipersAct_ = createAction(this, tr("Amplitude Calipers"),
        tr("Amplitude calipers"), 0, 0, "hi32-amplitudecalipers.png");
    amplitudeCalipersAct_->setCheckable(true);
    deleteAllCalipersAct_ = createAction(this, tr("Delete All Calipers"),
        tr("Delete all calipers"), 0, 0, "hi32-deleteallcalipers.png");
    msCalipersAct_ = createAction(this, tr("Millisecond Calipers"),
        tr("Caliper value in milliseconds"), 0, 0, "hi32-mscalipers.png");
    msCalipersAct_->setCheckable(true);
    bpmCalipersAct_ = createAction(this, tr("BPM Calipers"),
        tr("Caliper value in BPM"), 0, 0, "hi32-bpmcalipers.png");
    bpmCalipersAct_->setCheckable(true);
    makeWindowActiveAct_ = createAction(this, tr("Make Window Active"),
        tr("Enable this review window for updates"), 0, 0,
        "hi32-makewindowactive.png");
    makeWindowActiveAct_->setCheckable(true);
    updateWindowToggleAct_ = createAction(this, tr("Update Window On/Off"),
        tr("Toggle review window updating"), 0, 0,
        "hi32-updatewindowtoggle.png");
    updateWindowToggleAct_->setCheckable(true);   

}


RealTimeWindow::~RealTimeWindow()
{
}



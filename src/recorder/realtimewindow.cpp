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
#include "study.h"
#include "studyconfigurationdialog.h"

#include <QAction>
#include <QComboBox>
#include <QIcon>
#include <QMouseEvent>
#include <QPalette>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QToolBar>

#include <QtDebug>

using EpGui::createAction;
using EpGui::RealTimeStudyConfigurationDialog;
using EpGui::StudyConfigurationDialog;
using EpRecorder::RealTimeWindow;
using EpStudy::Study;

RealTimeWindow::RealTimeWindow(Study* study, int number, QWidget* parent)
 : SignalDisplayWindow(tr("Real-Time"), study,number, parent) {
    updateWindowTitle();
    createCentralWidget();
    createActions();
    createToolBars();
}

void RealTimeWindow::increaseSweepSpeed() {
    int index = sweepSpeedComboBox_->currentIndex();
    // decreasing index increases number in combobox
    if (--index < 0)
        index = 0;
    sweepSpeedComboBox_->setCurrentIndex(index);
}

void RealTimeWindow::decreaseSweepSpeed() {
    int index = sweepSpeedComboBox_->currentIndex();
    int maxIndex = sweepSpeedComboBox_->count() -1;
    if (++index > maxIndex)  
        index = maxIndex;
    sweepSpeedComboBox_->setCurrentIndex(index);
}

StudyConfigurationDialog* RealTimeWindow::studyConfigurationDialog() {
    return new RealTimeStudyConfigurationDialog(study()->studyConfiguration(), 
                                                this);
}

void RealTimeWindow::createToolBars() {
    QToolBar* toolBar = new QToolBar(tr("Real-Time Window Tool Bar"));
    toolBar->setObjectName("RealTimeWindowToolBar");
    toolBar->setAutoFillBackground(true);
    toolBar->addAction(minusAction_);
    toolBar->addSeparator();
    sweepSpeedComboBox_ = new QComboBox(this);
    /// TODO Which direction does the Prucka combobox go?
    sweepSpeedComboBox_->addItem(tr("400   "));
    sweepSpeedComboBox_->addItem(tr("200   "));
    sweepSpeedComboBox_->addItem(tr("100   "));
    sweepSpeedComboBox_->addItem(tr("50    "));
    sweepSpeedComboBox_->addItem(tr("25    "));
    sweepSpeedComboBox_->setCurrentIndex(3);    // 50 sweep speed
    toolBar->addWidget(sweepSpeedComboBox_);
    toolBar->addSeparator();
    toolBar->addAction(plusAction_);
    toolBar->addAction(studyConfigAction_);
    toolBar->addSeparator();
    toolBar->addAction(timeCalipersAction_);
    toolBar->addAction(amplitudeCalipersAction_);
    toolBar->addAction(deleteAllCalipersAction_);
    toolBar->addAction(msCalipersAction_);
    toolBar->addAction(bpmCalipersAction_);
    toolBar->addSeparator();
    toolBar->addAction(offsetSignalsAction_);
    toolBar->addAction(triggeredModeAction_);
    toolBar->addAction(toggleAblationWindowAction_);
    toolBar->addSeparator();
    toolBar->addAction(realTime12LeadAction_);
    toolBar->addSeparator();
    toolBar->addAction(timerAction_);
    toolBar->addAction(stopwatchAction_);
    // add stim buttons
    toolBar->addSeparator();
    QPushButton* toggleStimChannelButton = 
        new QPushButton(tr("hRA : 2-1"));
    QPalette palette;
    palette.setColor(QPalette::Button, Qt::cyan);
    toggleStimChannelButton->setPalette(palette);
    toolBar->addWidget(toggleStimChannelButton);
    toolBar->addSeparator();
    QPushButton* enableStimOneButton =
        new QPushButton(tr("hRA : 2-1"));
    palette.setColor(QPalette::Button, Qt::red);
    enableStimOneButton->setPalette(palette);
    toolBar->addWidget(enableStimOneButton);
    toolBar->addSeparator();
    QPushButton* enableStimTwoButton =
        new QPushButton(tr("RVa : 18-17"));
    enableStimTwoButton->setPalette(palette);
    toolBar->addWidget(enableStimTwoButton);
    toolBar->addSeparator();
    QPushButton* enableStimThreeButton =
        new QPushButton(tr("CS : 2-1"));
    toolBar->addWidget(enableStimThreeButton);
    toolBar->addSeparator();
    QPushButton* enableStimFourButton =
        new QPushButton(tr("ABL : 18-17"));
    toolBar->addWidget(enableStimFourButton);
    toolBar->addSeparator();

    addToolBar(toolBar); 
}

void RealTimeWindow::createActions() {
    minusAction_ = createAction(this, tr("Minus"), 
        tr("Decrease sweep speed"), SLOT(decreaseSweepSpeed()), 
        0, "hi32-minus.png"); 
    plusAction_ = createAction(this, tr("Plus"),
        tr("Increase sweep speed"), SLOT(increaseSweepSpeed()),
        0, "hi32-plus.png");
    studyConfigAction_ = createAction(this, tr("Study Configuration"),
        tr("Open study configuration"), SLOT(studyConfiguration()),
        0, "hi32-studyconfig.png");
    timeCalipersAction_ = createAction(this, tr("Time Calipers"),
        tr("Time calipers"), 0, 0, "hi32-timecalipers.png");
    timeCalipersAction_->setCheckable(true);
    timeCalipersAction_->setChecked(true); // default to time calipers
    amplitudeCalipersAction_ = createAction(this, tr("Amplitude Calipers"),
        tr("Amplitude calipers"), 0, 0, "hi32-amplitudecalipers.png");
    amplitudeCalipersAction_->setCheckable(true);
    deleteAllCalipersAction_ = createAction(this, tr("Delete All Calipers"),
        tr("Delete all calipers"), 0, 0, "hi32-deleteallcalipers.png");
    msCalipersAction_ = createAction(this, tr("Millisecond Calipers"),
        tr("Caliper value in milliseconds"), 0, 0, "hi32-mscalipers.png");
    msCalipersAction_->setCheckable(true);
    msCalipersAction_->setChecked(true);   // default to ms calipers
    bpmCalipersAction_ = createAction(this, tr("BPM Calipers"),
        tr("Caliper value in BPM"), 0, 0, "hi32-bpmcalipers.png");
    bpmCalipersAction_->setCheckable(true);
    offsetSignalsAction_ = createAction(this, tr("Offset Signals"),
        tr("Offset signals"), 0, 0, "hi32-offsetsignals.png");
    triggeredModeAction_ = createAction(this, tr("Triggered Mode"),
        tr("Toggle triggered mode"), 0, 0, "hi32-triggeredmode.png");
    triggeredModeAction_->setCheckable(true);
    toggleAblationWindowAction_ = createAction(this, 
        tr("Toggle Ablation Window"),
        tr("Toggle ablation window"), 0, 0, "hi32-toggleablationwindow.png");
    toggleAblationWindowAction_->setCheckable(true);
    realTime12LeadAction_ = createAction(this, tr("Real-Time 12-Lead"),
        tr("Print 12-lead of real-time data"), 0, 0, 
        "hi32-realtime12lead.png");
    timerAction_ = createAction(this, tr("Timer"),
        tr("Open count down timer"), 0, 0, "hi32-timer.png");
    timerAction_->setCheckable(true);
    connect(timerAction_, SIGNAL(triggered(bool)),
        this, SIGNAL(startTimer(bool)));
    stopwatchAction_ = createAction(this, tr("Stopwatch"),
        tr("Open count up timer"), 0,0, "hi32-stopwatch.png");
    stopwatchAction_->setCheckable(true);
    connect(stopwatchAction_, SIGNAL(triggered(bool)),
        this, SIGNAL(startStopwatch(bool)));
}

/// TODO can move these to superclass, pass prefix (e.g. "/realTimeWindow")
void RealTimeWindow::writeSettings(QSettings& settings) {
    settings.setValue("state", saveState());
    //settings.setValue("splitter", 
    //    static_cast<QSplitter*>(centralWidget())->saveState()); 
}

void RealTimeWindow::readSettings(QSettings& settings) {
    restoreState(settings.value("state").toByteArray());
    //static_cast<QSplitter*>(centralWidget())->restoreState(settings.value(
    //    "splitter").toByteArray());
}



RealTimeWindow::~RealTimeWindow() {}



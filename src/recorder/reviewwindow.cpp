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

#include "reviewwindow.h"

#include "actions.h"

#include <QAction>
#include <QCloseEvent>
#include <QComboBox>
#include <QIcon>
#include <QSettings>
#include <QSplitter>
#include <QToolBar>

using EpRecorder::ReviewWindow;

ReviewWindow::ReviewWindow(int number, QWidget *parent)
 : SignalDisplayWindow(tr("Review"), number, parent) {
    createCentralWidget();
    createActions();
    createToolBars();
    updateWindowTitle();
}

void ReviewWindow::closeEvent(QCloseEvent* event) {
    emit windowClosing(true);
    event->accept();
}

void ReviewWindow::increaseSweepSpeed() {
    int index = sweepSpeedComboBox_->currentIndex();
    // decreasing index increases number in combobox
    if (--index < 0)
        index = 0;
    sweepSpeedComboBox_->setCurrentIndex(index);
}

void ReviewWindow::decreaseSweepSpeed() {
    int index = sweepSpeedComboBox_->currentIndex();
    int maxIndex = sweepSpeedComboBox_->count() -1;
    if (++index > maxIndex)  
        index = maxIndex;
    sweepSpeedComboBox_-> setCurrentIndex(index);
}

void ReviewWindow::makeWindowActive(bool enable) {
    windowActive_ = enable;
    emit windowActivated(enable);
}

void ReviewWindow::otherWindowActive(bool enable) {
    windowActive_ = !enable;
    makeWindowActiveAction_->setChecked(!enable);
}

void ReviewWindow::writeSettings(QSettings& settings) {
    settings.setValue("state", saveState());
    //settings.setValue("splitter", 
    //    static_cast<QSplitter*>(centralWidget())->saveState());
    settings.setValue("windowActive", windowActive_);   
}

void ReviewWindow::readSettings(QSettings& settings) {
    restoreState(settings.value("state").toByteArray());
    //static_cast<QSplitter*>(centralWidget())->restoreState(settings.value(
    //    "splitter").toByteArray());
    windowActive_ = settings.value("windowActive").toBool();
    updateToolBars();
}

void ReviewWindow::updateToolBars() {
    makeWindowActiveAction_->setChecked(windowActive_); 
}

void ReviewWindow::createActions() {
    using EpGui::createAction;

    minusAction_ = createAction(this, tr("Minus"), 
        tr("Decrease sweep speed"), SLOT(decreaseSweepSpeed()), 
        0, "hi32-minus.png"); 
    plusAction_ = createAction(this, tr("Plus"),
        tr("Increase sweep speed"), SLOT(increaseSweepSpeed()),
        0, "hi32-plus.png");
    studyConfigAction_ = createAction(this, tr("Study Configuration"),
        tr("Open study configuration"), 0, 0, "hi32-studyconfig.png");
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
    makeWindowActiveAction_ = createAction(this, tr("Make Window Active"),
        tr("Toggle between two review windows"), 
        SLOT(makeWindowActive(bool)), 0, "hi32-makewindowactive.png");
    makeWindowActiveAction_->setCheckable(true);
    updateWindowAction_ = createAction(this, tr("Update Window"),
        tr("Continuously update the review window"), 
        0, 0, "hi32-updatewindowtoggle.png");
    offsetSignalsAction_ = createAction(this, tr("Offset Signals"),
        tr("Offset signals"), 0, 0, "hi32-offsetsignals.png");
    snapshotAction_ = createAction(this, tr("Snapshot"), tr("Create a snapshot"),
        0, 0, "hi32-snapshot.png");
    realTime12LeadAction_ = createAction(this, tr("Real-Time 12-Lead"),
        tr("Print 12-lead of real-time data"), 0, 0, 
        "hi32-realtime12lead.png");
    printAction_ = createAction(this, tr("Print"), tr("Print the review window"),
        0, 0, "hi32-printreview.png");
    createLogEventAction_ = createAction(this, tr("Create Log Event"),
        tr("Create a log event"), 0, 0, "hi32-createlogevent.png");
    timerAction_ = createAction(this, tr("Timer"),
        tr("Open count down timer"), 0, 0, "hi32-timer.png");
    timerAction_->setCheckable(true);
    stopwatchAction_ = createAction(this, tr("Stopwatch"),
        tr("Open count up timer"), 0,0, "hi32-stopwatch.png");
    stopwatchAction_->setCheckable(true);
    

}

void ReviewWindow::createToolBars() {
    QToolBar* toolBar = new QToolBar(tr("Review Window Tool Bar"));
    toolBar->setObjectName("ReviewWindowToolBar");
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
    toolBar->addAction(makeWindowActiveAction_);
    toolBar->addAction(updateWindowAction_);
    toolBar->addSeparator();
    toolBar->addAction(offsetSignalsAction_);
    toolBar->addSeparator();
    toolBar->addAction(snapshotAction_);
    toolBar->addAction(realTime12LeadAction_);
    toolBar->addAction(printAction_);
    toolBar->addSeparator();
    toolBar->addAction(createLogEventAction_);
    toolBar->addSeparator();
    toolBar->addAction(timerAction_);
    toolBar->addAction(stopwatchAction_);

    addToolBar(toolBar); 
}


ReviewWindow::~ReviewWindow() {}



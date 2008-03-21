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
#include "settings.h"

#include <QAction>
#include <QComboBox>
#include <QIcon>
#include <QSplitter>
#include <QToolBar>

ReviewWindow::ReviewWindow(int number, QWidget *parent)
 : SignalDisplayWindow(tr("Review"), number, parent) {
    createCentralWidget();
    createActions();
    createToolBars();
    updateWindowTitle();
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

void ReviewWindow::writeSettings(Settings& settings) {
    QString prefix = "/reviewWindow" + QString::number(number());
    settings.setValue(prefix + "Size", size());
    settings.setValue(prefix + "Pos", pos()); 
    settings.setValue(prefix + "State", saveState());
    settings.setValue(prefix + "Splitter", 
        static_cast<QSplitter*>(centralWidget())->saveState());   
}

void ReviewWindow::readSettings(const Settings& settings) {
    QString prefix = "/reviewWindow" + QString::number(number());
    restoreState(settings.value(prefix + "State").toByteArray());
    QVariant size = settings.value(prefix + "Size");
    if (size.isNull()) {
        showMaximized();
        return;
    }
    resize(size.toSize());
    move(settings.value(prefix + "Pos").toPoint());
    static_cast<QSplitter*>(centralWidget())->restoreState(settings.value(
        prefix + "Splitter").toByteArray());
}

void ReviewWindow::createActions() {
    using EpGui::createAction;

    minusAct_ = createAction(this, tr("Minus"), 
        tr("Decrease sweep speed"), SLOT(decreaseSweepSpeed()), 
        0, "hi32-minus.png"); 
    plusAct_ = createAction(this, tr("Plus"),
        tr("Increase sweep speed"), SLOT(increaseSweepSpeed()),
        0, "hi32-plus.png");
    studyConfigAct_ = createAction(this, tr("Study Configuration"),
        tr("Open study configuration"), 0, 0, "hi32-studyconfig.png");
    timeCalipersAct_ = createAction(this, tr("Time Calipers"),
        tr("Time calipers"), 0, 0, "hi32-timecalipers.png");
    timeCalipersAct_->setCheckable(true);
    timeCalipersAct_->setChecked(true); // default to time calipers
    amplitudeCalipersAct_ = createAction(this, tr("Amplitude Calipers"),
        tr("Amplitude calipers"), 0, 0, "hi32-amplitudecalipers.png");
    amplitudeCalipersAct_->setCheckable(true);
    deleteAllCalipersAct_ = createAction(this, tr("Delete All Calipers"),
        tr("Delete all calipers"), 0, 0, "hi32-deleteallcalipers.png");
    msCalipersAct_ = createAction(this, tr("Millisecond Calipers"),
        tr("Caliper value in milliseconds"), 0, 0, "hi32-mscalipers.png");
    msCalipersAct_->setCheckable(true);
    msCalipersAct_->setChecked(true);   // default to ms calipers
    bpmCalipersAct_ = createAction(this, tr("BPM Calipers"),
        tr("Caliper value in BPM"), 0, 0, "hi32-bpmcalipers.png");
    bpmCalipersAct_->setCheckable(true);
    makeWindowActiveAct_ = createAction(this, tr("Make Window Active"),
        tr("Toggle between two review windows"), 
        0, 0, "hi32-makewindowactive.png");
    updateWindowAct_ = createAction(this, tr("Update Window"),
        tr("Continuously update the review window"), 
        0, 0, "hi32-updatewindowtoggle.png");
    offsetSignalsAct_ = createAction(this, tr("Offset Signals"),
        tr("Offset signals"), 0, 0, "hi32-offsetsignals.png");
    snapshotAct_ = createAction(this, tr("Snapshot"), tr("Create a snapshot"),
        0, 0, "hi32-snapshot.png");
    realTime12LeadAct_ = createAction(this, tr("Real-Time 12-Lead"),
        tr("Print 12-lead of real-time data"), 0, 0, 
        "hi32-realtime12lead.png");
    printAct_ = createAction(this, tr("Print"), tr("Print the review window"),
        0, 0, "hi32-printreview.png");
    createLogEventAct_ = createAction(this, tr("Create Log Event"),
        tr("Create a log event"), 0, 0, "hi32-createlogevent.png");
    timerAct_ = createAction(this, tr("Timer"),
        tr("Open count down timer"), 0, 0, "hi32-timer.png");
    timerAct_->setCheckable(true);
    stopwatchAct_ = createAction(this, tr("Stopwatch"),
        tr("Open count up timer"), 0,0, "hi32-stopwatch.png");
    stopwatchAct_->setCheckable(true);
    

}

void ReviewWindow::createToolBars() {
    QToolBar* toolBar = new QToolBar(tr("Review Window Tool Bar"));
    toolBar->setObjectName("ReviewWindowToolBar");
    toolBar->setAutoFillBackground(true);
    toolBar->addAction(minusAct_);
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
    toolBar->addAction(updateWindowAct_);
    toolBar->addSeparator();
    toolBar->addAction(offsetSignalsAct_);
    toolBar->addSeparator();
    toolBar->addAction(snapshotAct_);
    toolBar->addAction(realTime12LeadAct_);
    toolBar->addAction(printAct_);
    toolBar->addSeparator();
    toolBar->addAction(createLogEventAct_);
    toolBar->addSeparator();
    toolBar->addAction(timerAct_);
    toolBar->addAction(stopwatchAct_);

    addToolBar(toolBar); 
}


ReviewWindow::~ReviewWindow() {}



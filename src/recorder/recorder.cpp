/***************************************************************************
 *   Copyright (C) 2006 by EP Studios, Inc.                                *
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

#include "recorder.h"

#include "actions.h"
#include "logwindow.h"
#include "opticaldisk.h"
#include "options.h"
#include "patient.h"
#include "patientdialog.h"
#include "patientstatusbar.h"
#include "realtimewindow.h"
#include "recorderdefs.h"
#include "reviewwindow.h"
#include "satmonitor.h"
#include "simulatorsettingsdialog.h"
#include "stimulator.h"
#include "teststimulator.h"
#include "study.h"
#include "systemdialog.h"
#include "user.h"
#include "versioninfo.h"

#include <QAction>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
#include <QToolBar>
#include <QVariant>

#include <QtDebug>

using EpGui::PatientDialog;
using EpGui::SimulatorSettingsDialog;
using EpGui::SystemDialog;
using EpRecorder::Recorder;

using namespace EpHardware;
using namespace EpHardware::EpOpticalDisk;
using namespace EpHardware::EpStimulator;

Recorder::Recorder(QWidget* parent, 
                   Study* study, 
                   OpticalDisk* currentDisk,
                   bool allowAcquisition,
                   RecorderWindow recorderWindow)
                   : 
                   QMainWindow(parent), 
                   openDisplayWindowList_(LastDisplayWindow + 1),
                   study_(study), 
                   user_(User::instance()),
                   options_(Options::instance()),
                   currentDisk_(currentDisk),
                   allowAcquisition_(allowAcquisition),
                   recorderWindow_(recorderWindow),
                   realTimeWindow_(0),
                   review1Window_(0),
                   review2Window_(0),
                   logWindow_(0),
                   realTimeSubWindow_(0),
                   review1SubWindow_(0),
                   review2SubWindow_(0),
                   logSubWindow_(0) {
    Q_ASSERT(parent != 0);  // never call Recorder without parent
    setAttribute(Qt::WA_DeleteOnClose);

    if (recorderWindow_ == Primary && qApp->desktop()->numScreens() > 1) {
        Recorder* recorder = new Recorder(this, study_, 
            currentDisk_, false, Secondary);
        // resize and position recorder
        recorder->show();
    }
    // must build menus and toolbars before creating the central widget,
    // since createCentralWidget() updates the menus.
    createActions();
    createMenus();
    createToolBars();
    createPatientStatusBar();
    createStatusBar();
    createCentralWidget();


    connect(patientStatusBar_, SIGNAL(manualSave(bool)),
        this, SLOT(setManualSave(bool)));
    connect(patientStatusBar_, SIGNAL(showPatientInformation()),
        this, SLOT(patientInformation()));
    connect(centralWidget_, SIGNAL(subWindowActivated(QMdiSubWindow*)),
        this, SLOT(updateMenus()));
    connect(this, SIGNAL(patientInformationClosed()),
        patientStatusBar_, SLOT(patientInformationClosed()));

    updateAll();
}

Recorder::~Recorder() {
    /// FIXME
    /// The primary Recorder window owns the pointers below
    /// and so only it can delete them.  In addition
    /// The primary Recorder window created the patient_
    /// pointer, it could pass null pointers to the secondary
    /// Recorder to solve this, and not create a new patient_.
    /// NEED to FIX this!!!!!!!!!!!!!!
    delete patient_;
    // Recorder took possession of study_, so has to kill it now.
    delete study_;
}

bool Recorder::eventFilter(QObject* target, QEvent* event) {
    // logic here is dependent on SubWindow type.
    // If RealTimeWindow and 2 screens, then no movement allowed.
    // If RealTimeWindow and 1 screen, then on movement of right edge
    // and bottom allowed.
    // Review1Window by itself only allows movement of bottom edge,
    // otherwise if paired with RealTimeWindow, only left edge and
    // bottom, if with Review2Window only right edge and bottom,
    // and if all 3 windows (Realtime, Review1, Review2) then both
    // edges and bottom.  Review2 can only move left edge and bottom.
    // LogWindow can only move top.
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->buttons() & Qt::LeftButton) {
            qDebug() << mouseEvent->pos();
            return true;
        }
    }
    return QMainWindow::eventFilter(target, event);
}

void Recorder::createCentralWidget() {
    centralWidget_ = new QMdiArea;
    setCentralWidget(centralWidget_);
    readSettings();
//     // deal with no saved settings (first run-through of program).
//     ///  TODO this must be altered for multihead system.
//     if (centralWidget_->subWindowList().isEmpty()) {
//         if (Options::instance()->enableAcquisition()) {
//             realTimeWindow_ = new RealTimeWindow;
//             realTimeSubWindow_ = centralWidget_->addSubWindow(realTimeWindow_);
//             centralWidget_->tileSubWindows();
//         }
//         else {
//             review1Window_ = new ReviewWindow(1);
//             review1SubWindow_ = centralWidget_->addSubWindow(review1Window_);
//             logWindow_ = new LogWindow;
//             logSubWindow_ = centralWidget_->addSubWindow(logWindow_);
//             centralWidget_->tileSubWindows();
//             // trying to approximate a good layout for review1 and log windows.
//             // Unfortunately centralWidget_ does not give accurate height()
//             // and width() values at this stage of construction.
//             //int w = width();
// //             int h = height();            
// //             review1SubWindow_->move(0, 0);
// //             review1SubWindow_->resize(w, h * 2 / 3);
// //             logSubWindow_->move(0, h * 2 / 3);
// //             logSubWindow_->resize(w, h / 3);
//         } 
//     }

}

// bool Recorder::useOneScreen() {
//     if (options_->useOneScreen())
//         return true;
//     

/**
 * Sets up the screens when application first run, or number of screens
 * or emulateTwoScreens option or enableAcquisition options change, 
 * or if selected window settings doesn't match number of screens
 * (e.g. window setting alpha is chosen, but was saved in a one
 * screen system, and now there are 2 screens.  setupInitialScreen is
 * run in this situation to generate a standard screen appearance, that
 * can then be changes and saved as the 2 screen version of alpha).
 * @param tile set to true if "tiling" an already setup screen.
 */
void Recorder::setupInitialScreen(bool tile) {
    centralWidget_->closeAllSubWindows();
    if (qApp->desktop()->numScreens() > 1 || options_->emulateTwoScreens()) { 
        if (recorderWindow_ == Primary && allowAcquisition_) {
            realTimeWindowOpen(true);
            centralWidget_->tileSubWindows();
        }
        else { // recorderWindow_ == Secondary or no Acquistion
            review1WindowOpen(true);
            logWindowOpen(true);
            centralWidget_->tileSubWindows();       
        }
    }
    else {  // only one screen, should just be the Primary screen
        review1WindowOpen(true);
        logWindowOpen(true);
        // trying to approximate a good layout for review1 and log windows.
        int w = centralWidget()->width();
        int h = centralWidget()->height();
        // on initial setup, for some reason the central widget height
        // is not properly setup, the status bar height is not accounted
        // for.  This doesn't happen again after the initial setup.
        if (!tile)
            h -= statusBar()->height();
        if (allowAcquisition_) {
            realTimeWindowOpen(true);
            realTimeSubWindow_->move(0,0);
            realTimeSubWindow_->resize(w / 2, h * 2 /3);
            review1SubWindow_->move(w / 2, 0);
            review1SubWindow_->resize(w / 2, h * 2 / 3);
            centralWidget_->setActiveSubWindow(realTimeSubWindow_);
        }
        else {
            review1SubWindow_->move(0,0);
            review1SubWindow_->resize(w, h * 2 / 3);
            centralWidget_->setActiveSubWindow(review1SubWindow_);
        }   // logSubWindow_ always same position
        logSubWindow_->move(0, h * 2 / 3);
        logSubWindow_->resize(w, (h / 3));
    }
    if (realTimeSubWindow_)
        realTimeSubWindow_->setOption(QMdiSubWindow::RubberBandResize);
//     if (realTimeSubWindow_)
//         realTimeSubWindow_->installEventFilter(this);
    if (review1SubWindow_)
        review1SubWindow_->installEventFilter(this);
    if (logSubWindow_)
        logSubWindow_->installEventFilter(this);
    

}

void Recorder::setManualSave(bool enable) {
    manualSaveAction_->setChecked(enable);
}

void Recorder::updateWindowTitle() {
    QString title = tr("%1")
        .arg(EpCore::VersionInfo::instance()->programName());
    title = User::instance()->isAdministrator() ? 
        QString("%1 %2").arg(title).arg(tr("[Administrator]")) : title;
    setWindowTitle(title);
}

void Recorder::updateAll() {
    updateMenus();
    updateWindowTitle();
    updateSettings();
}



void Recorder::patientInformation() {
    PatientDialog* patientDialog = new PatientDialog(this);
    patientDialog->setFields(study_);
    if (patientDialog->exec() == QDialog::Accepted) {
        patientDialog->getFields(study_);
        study_->save();
        patientStatusBar_->setPatientInfo(study_->name(), 
            study_->weight(), study_->bsa());

    }
    delete patientDialog;
    emit patientInformationClosed();
}

void Recorder::systemSettings() {
    if (administrationAllowed()) {
        SystemDialog* systemDialog = new SystemDialog(options_, 
            currentDisk_->studiesPath(), currentDisk_->label(),
            currentDisk_->translatedSide(), this, false);
        if (systemDialog->exec() == QDialog::Accepted)
            systemDialog->setOptions();
        delete systemDialog;
    }
}

void Recorder::simulatorSettings() {
    if (administrationAllowed()) {
        SimulatorSettingsDialog* simDialog = 
            new SimulatorSettingsDialog(options_, this);
        //simDialog->removeNavigatorTab();
        if (simDialog->exec() == QDialog::Accepted) {
            simDialog->setOptions();
            updateMenus();
            updateSettings();
            // signal updates simulator settings in Navigator
            emit updateSimulatorSettings();
        }
        delete simDialog;
    }
}

void Recorder::updateSettings() {
    QDockWidget* dockWidget =
        qobject_cast<QDockWidget*>(patientStatusBar_->parentWidget());
    dockWidget->setWindowTitle(options_->patientStatusBarHasTitle()
        ? tr("Patient Status") : "");
    if (options_->immovablePatientStatusBar())
        dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    else
        dockWidget->setFeatures(QDockWidget::DockWidgetClosable
            | QDockWidget::DockWidgetMovable 
            | QDockWidget::DockWidgetFloatable);
}

void Recorder::connectReviewWindows() {
    if (subWindowIsOpen(review1SubWindow_) &&
        subWindowIsOpen(review2SubWindow_)) {
        connect(review1Window_, SIGNAL(windowActivated(bool)),
            review2Window_, SLOT(otherWindowActive(bool)));
        connect(review2Window_, SIGNAL(windowActivated(bool)),
            review1Window_, SLOT(otherWindowActive(bool)));
        connect(review1Window_, SIGNAL(windowClosing(bool)),
            this, SLOT(review2WindowOpen(bool)));
    }
}

void Recorder::realTimeWindowOpen(bool open) {
    openSubWindow(open, realTimeSubWindow_, realTimeWindow_);
}

void Recorder::review1WindowOpen(bool open) {
    // if closing and review2 is open, close that too
    if (!open && subWindowIsOpen(review2SubWindow_))
        review2WindowOpen(false);
    openSubWindow(open, review1SubWindow_, review1Window_, 1);
    if (open) {
        connectReviewWindows();
        review1Window_->makeWindowActive(true);
    }
    updateMenus();  // opening review windows affects menu
} 

void Recorder::review2WindowOpen(bool open) {
    // must have a review1window open first
    if (open && !subWindowIsOpen(review1SubWindow_))
        return;
    openSubWindow(open, review2SubWindow_, review2Window_, 2);
    if (open) {
        connectReviewWindows();
        review2Window_->makeWindowActive(false);
    }
    updateMenus();
} 

void Recorder::logWindowOpen(bool open) {
    openSubWindow(open, logSubWindow_, logWindow_);
}

void Recorder::closeEvent(QCloseEvent *event) {
    if (closeStudy()) {
        study_->save();
        patient_->save();
        patientStatusBar_->stop();
        if (parentWidget())
            parentWidget()->show();        
        writeSettings();
        event->accept();
    }
    else
        event->ignore();
}

void Recorder::resizeEvent(QResizeEvent*) {
    if (options_->emulateWindowsManager())
        tileSubWindows();
}

bool Recorder::closeStudy() {
    int ret = QMessageBox::question(this,
        tr("Close Study?"),
        tr("Select Yes to return to EP Simulator Navigator"),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No | QMessageBox::Escape);
    return ret == QMessageBox::Yes;
}

bool Recorder::subWindowIsOpen(QMdiSubWindow* subWindow) {
    QList<QMdiSubWindow*> windowList = centralWidget_->subWindowList();
    return windowList.contains(subWindow);
}

void Recorder::writeSettings() {
    QSettings settings;
    writeSettings(settings);
}

void Recorder::writeSettings(QSettings& settings) {
    //QSettings settings;
    // save overall Recorder size, position and state
    QDesktopWidget* desktop = qApp->desktop();
    settings.beginGroup(QString("screen%1").arg(desktop->screenNumber(this)));
    settings.beginGroup("recorder");
    settings.setValue("size", size());
    settings.setValue("pos", pos()); 
    settings.setValue("state", saveState());
    // erase previously save list of open display windows
    settings.remove("subWindowList"); 
    // crease empty list of open display window names
    QStringList subWindowKeys; 
    // get open windows
    QList<QMdiSubWindow*> subWindowList =  centralWidget_->subWindowList();
    DisplayWindow* dw = 0;
    QMdiSubWindow* activeSubWindow = centralWidget_->currentSubWindow();
    for (int i = 0; i < subWindowList.size(); ++i) {
        dw = static_cast<DisplayWindow*>(subWindowList[i]->widget());
        subWindowKeys << dw->key();
        if (subWindowList[i] == activeSubWindow)
            settings.setValue("currentWindowKey", dw->key());
        settings.beginGroup(dw->key());
        settings.setValue("size", subWindowList[i]->size());
        settings.setValue("pos", subWindowList[i]->pos());
        dw->writeSettings(settings);
        settings.endGroup();
    }
    settings.setValue("subWindowList", subWindowKeys);
    settings.endGroup();
    settings.endGroup();
}

void Recorder::updateOpenDisplayWindowList() {
    QList<QMdiSubWindow*> windowList = centralWidget_->subWindowList();
    openDisplayWindowList_.clear();
    openDisplayWindowList_[RealTime] = windowList.contains(realTimeSubWindow_);
    openDisplayWindowList_[Review1] = windowList.contains(review1SubWindow_);
    openDisplayWindowList_[Review2] = windowList.contains(review2SubWindow_);
    openDisplayWindowList_[Log] = windowList.contains(logSubWindow_);
    /// TODO other window types here.

}

void Recorder::restoreDisplayWindow(const QString& key, 
    QSettings& settings, const QString& currentWindowKey,
    QMdiSubWindow* sw, DisplayWindow* dw, 
    QMdiSubWindow*& currentSubWindow) {
    //(this->*openFunction)(true);
    settings.beginGroup(key);
    sw->move(settings.value("pos").toPoint());
    sw->resize(settings.value("size").toSize());
    dw->readSettings(settings);
    settings.endGroup(); 
    if (currentWindowKey == key)
        currentSubWindow = sw;
}  

void Recorder::readSettings() {
    QSettings settings;
    readSettings(settings);
}

void Recorder::readSettings(QSettings& settings) {
    
//    review1Window_->readSettings();
    //QSettings settings;
    QDesktopWidget* desktop = qApp->desktop();
    settings.beginGroup(QString("screen%1").arg(desktop->screenNumber(this)));
    settings.beginGroup("recorder");
    QVariant size = settings.value("size");
    if (size.isNull()) {
        setWindowState(windowState() ^ Qt::WindowMaximized);
        settings.endGroup();
        settings.endGroup();
        return;
    }    
    resize(size.toSize());
    move(settings.value("pos").toPoint());
    restoreState(settings.value("state").toByteArray());
    // clear the central widget - close all the subwindows
//     centralWidget_->closeAllSubWindows();
//     // read a list of the saved subwindows, open each one and 
//     // readSettings for each one.
//     // Note that below is clunky, but seems best solution after
//     // experimenting with less repetitive ways of doing this.
//     QStringList subWindowKeys = settings.value("subWindowList").toStringList();
//     QString currentWindowKey = settings.value("currentWindowKey").toString();
//     QMdiSubWindow* activeWindow = 0;
//     // sorry, no RealTime window if no acquisition
//     if (subWindowKeys.contains(realTimeWindowKey) &&
//         Options::instance()->enableAcquisition()) {
//         // apparently can't use pointer to member function here
//         // because Recorder is not fully constructed yet, so pointer to
//         // member is wild.
//         realTimeWindowOpen(true);
//         restoreDisplayWindow(realTimeWindowKey, 
//             settings, currentWindowKey, realTimeSubWindow_, 
//             realTimeWindow_, activeWindow);
//     }
//     if (subWindowKeys.contains(review1WindowKey)) {
//         review1WindowOpen(true);
//         restoreDisplayWindow(review1WindowKey, 
//             settings, currentWindowKey, review1SubWindow_, 
//             review1Window_, activeWindow);
//     }
//     if (subWindowKeys.contains(review2WindowKey)) {
//         review2WindowOpen(true);
//         restoreDisplayWindow(review2WindowKey, 
//             settings, currentWindowKey, review2SubWindow_, 
//             review2Window_, activeWindow);
//     }
//     if (subWindowKeys.contains(logWindowKey)) {
//         logWindowOpen(true);
//         restoreDisplayWindow(logWindowKey, 
//             settings, currentWindowKey, logSubWindow_, 
//             logWindow_, activeWindow);
//     }
//     /// TODO again add future DisplayWindow processing here.
//     settings.endGroup();
//     settings.endGroup();
//     if (activeWindow)
//         centralWidget_->setActiveSubWindow(activeWindow);
}

void Recorder::login() {
    EpGui::login(this, user_);
    updateAll();
}

void Recorder::logout() {
    EpGui::logout(user_);
    updateAll();
}

void Recorder::changePassword() {
    if (administrationAllowed())
        EpGui::changePassword(this);
}

bool Recorder::administrationAllowed() {
    if (!options_->administratorAccountRequired())
        return true;
    login();
    return user_->isAdministrator();
}

void Recorder::openStimulator() {
    Stimulator* stimulator = new TestStimulator(this);
    stimulator->show();
}

void Recorder::openSatMonitor() {
       SatMonitor* satMonitor = new SatMonitor(this);
       satMonitor->show();
}

void Recorder::help() {
    EpGui::help(this);
}

 void Recorder::about() {
    EpGui::about(this);
}

void Recorder::tileSubWindows() {
    /// TODO option to control regular tiling vs 
    /// return to initial window settings
    // regular tiling
    // centralWidget_->tileSubWindows();
    setupInitialScreen(true);
    // need to emit signals to other Recorder window
}

void Recorder::cascadeSubWindows() {
    centralWidget_->cascadeSubWindows();
}

/**
 * Gets the statusbar going, with a blank message.
 * Temporary messages from menu functions, etc. will
 * now show up here.  The "Press F1 for Help" message
 * that appears on the Navigator statusbar doesn't work
 * well here, since a frame is drawn around any widget
 * inserted into the statusbar, which looks ugly adjacent
 * to the PatientStatusBar.  TODO The status bar will
 * only appear when the option below is set.
 */
void Recorder::createStatusBar() {
    if (options_->recorderStatusBar())
        statusBar()->showMessage(QString());
}

void Recorder::createPatientStatusBar() {
    patientStatusBar_ = new PatientStatusBar(this);
    QDockWidget* bottomDockWidget = new QDockWidget(this);
    bottomDockWidget->setWindowTitle(tr("Patient Status"));
    bottomDockWidget->setObjectName("bottomDockWidget");
    bottomDockWidget->setWidget(patientStatusBar_);
    //bottomDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    bottomDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, bottomDockWidget);

    patient_ = new Patient;
    patient_->setPath(study_->path());
    patient_->load();
    patientStatusBar_->setPatient(patient_);
    patientStatusBar_->setPatientInfo(study_->name(), 
        study_->weight(), study_->bsa());
    patientStatusBar_->start();
}

void Recorder::createActions() {
    using EpGui::createAction;
    patientInformationAction_ = createAction(this, tr("Patient Information"), 
        tr("Create and modify patient information"),
        SLOT(patientInformation()));    
    consciousSedationAction_ = createAction(this, tr("Conscious Sedation"),
        tr("Conscious sedation list"), 0, tr("Alt+A"));
    complicationsAction_ = createAction(this, tr("Complications"), 
        tr("Complications list"), 0, tr("Alt+M"));
    radiologyAction_ = createAction(this, tr("Radiology"), 
        tr("Radiology list"), 0, tr("Alt+N"));
    medicationAction_ = createAction(this, tr("Medications"), 
        tr("Medication list"), 0, tr("Alt+B"));
    suppliesAction_ = createAction(this, tr("Supplies"),
        tr("Supplies list"), 0, tr("Alt+V"));
    staffAction_ = createAction(this, tr("Staff"), 
        tr("Staff list"), 0, tr("Alt+C"));
    clinicalProceduresAction_ = createAction(this, tr("Clinical Procedures"), 
        tr("Clinical procedures list"), 0, tr("Alt+X"));
    macrosAction_ = createAction(this, tr("Macros"), 
        tr("Macros list"), 0, tr("Alt+Z"));
    ordersAction_ = createAction(this, tr("Orders"), 
        tr("Enter orders"));
    reportsAction_ = createAction(this, tr("Reports..."), 
        tr("Create procedure reports"));
    exportDataAction_ = createAction(this, tr("Export Data"), 
        tr("Export data to external formats"));
    closeStudyAction_ = createAction(this, tr("Close Study"),
        tr("Close patient study"), SLOT(close()), 0,
        "hi32-closestudy.png");
    // Study Configuration
    switchAction_ = createAction(this, tr("Switch..."), 
        tr("Switch study configuration"), 0, 
        0, "hi32-switchwindowsettings.png");
    saveAction_ = createAction(this, tr("Save"),
        tr("Save study configuration"));
    saveAsAction_ = createAction(this, tr("Save As..."), 
        tr("Save study configuration under different name"));
    intervalsAction_ = createAction(this, tr("Intervals"), 
        tr("Configure study intervals"));
    columnFormatsAction_ = createAction(this, tr("Column Formats"), 
        tr("Configure column formats"));
    protocolsAction_ = createAction(this, tr("Protocols"), 
        tr("Configure protocols"));
    // Measurements
    conductionIntervalsAction_ = createAction(this, tr("Conduction Intervals..."),
        tr("Measure conduction intervals"), 0, tr("Alt+Q"));
    snrtAction_ = createAction(this, tr("SNRT..."), 
        tr("Measure sinus node recover time"), 0, tr("Alt+W"));
    anteRPsAction_ = createAction(this, tr("Ante RPs..."), 
        tr("Measure anterograde refractory periods"), 0, tr("Alt+E"));
    retroRPsAction_ = createAction(this, tr("Retro RPs..."), 
        tr("Measure retrograde refractory periods"), 0, tr("Alt+R"));
    manualMeasurementsAction_ = createAction(this, tr("Manual Measurements"),
        tr("Make manual measurements"));
    statVitalsAction_ = createAction(this, tr("Stat Vitals"),
        tr("Record stat vital signs"));
    startStopNBPAction_ = createAction(this, tr("Start/Stop NBP"),
        tr("Start or stop noninvasive BP measurements"), 0, tr("F8"));
    autoLogVitalsAction_ = createAction(this, tr("Auto-Log Vitals"),
        tr("Auto-log vital signs"));
    autoLogVitalsAction_->setCheckable(true);
    autoLogNBPAction_ = createAction(this, tr("Auto-Log NBP"),
        tr("Auto-log noninvasive BP measurements"));
    autoLogNBPAction_->setCheckable(true);
    measurementConfigurationAction_ = createAction(this, 
        tr("Measurement Configuration"),
        tr("Configure measurements"));
    dataExtractionAction_ = createAction(this, tr("Data Extraction"),
        tr("Extract data"));
    // Windows Menu
    winSaveAction_ = createAction(this, tr("Save"),
        tr("Save window configuration"));
    winSaveAsAction_ = createAction(this, tr("Save As..."),
        tr("Save windows configuration as..."));
    winSwitchAction_ = createAction(this, tr("Switch..."),
        tr("Switch windows configuration"));
    winDeleteAction_ = createAction(this, tr("Delete..."),
        tr("Delete windows configuration"));
    timerAction_ = createAction(this, tr("Timer"),
        tr("Show timer"));
    timerAction_->setCheckable(true);
    stopwatchAction_ = createAction(this, tr("Stopwatch"),
        tr("Show stopwatch"));
    stopwatchAction_->setCheckable(true);
    realTimeAction_ = createAction(this, tr("Real-Time"),
       tr("Show real-time window"));
    realTimeAction_->setCheckable(true);
    connect(realTimeAction_, SIGNAL(triggered(bool)),
        this, SLOT(realTimeWindowOpen(bool)));
    review1Action_ = createAction(this, tr("Review 1"),
       tr("Show review 1 window"));
    review1Action_->setCheckable(true);
    connect(review1Action_, SIGNAL(triggered(bool)),
        this, SLOT(review1WindowOpen(bool)));
    review2Action_ = createAction(this, tr("Review 2"),
       tr("Show review 2 window"));
    review2Action_->setCheckable(true);
    connect(review2Action_, SIGNAL(triggered(bool)),
        this, SLOT(review2WindowOpen(bool)));
    logAction_ = createAction(this, tr("Log"),
       tr("Show log window"));
    logAction_->setCheckable(true);
    connect(logAction_, SIGNAL(triggered(bool)),
        this, SLOT(logWindowOpen(bool)));
    // The windows below will be implemented in later versions of EP Simulator
    ablationAction_ = createAction(this, tr("Ablation"),
       tr("Show ablation window"));
    ablationAction_->setCheckable(true);
    ablationAction_->setEnabled(false);
    alignmentAction_ = createAction(this, tr("Alignment"),
       tr("Show alignment window"));
    alignmentAction_->setCheckable(true);
    alignmentAction_->setEnabled(false);
    mapAction_ = createAction(this, tr("Map"),
       tr("Show map window"));
    mapAction_->setCheckable(true);
    mapAction_->setEnabled(false);
    holterAction_ = createAction(this, tr("Holter"),
       tr("Show holter window"));
    holterAction_->setCheckable(true);
    holterAction_->setEnabled(false);
    plotAction_ = createAction(this, tr("Plot"),
       tr("Show plot window"));
    plotAction_->setCheckable(true);
    plotAction_->setEnabled(false);
    macroAction_ = createAction(this, tr("Macro"),
       tr("Show macro window"));
    macroAction_->setCheckable(true);
    macroAction_->setEnabled(false);
    image1Action_ = createAction(this, tr("Image 1"),
       tr("Show image 1 window"));
    image1Action_->setCheckable(true);
    image1Action_->setEnabled(false);
    image2Action_ = createAction(this, tr("Image 2"),
       tr("Show image 2 window"));
    image2Action_->setCheckable(true);
    image2Action_->setEnabled(false);
    imageLibraryAction_ = createAction(this, tr("Image Library"),
       tr("Show image library window"));
    imageLibraryAction_->setCheckable(true);
    imageLibraryAction_->setEnabled(false);
    tileAction_ = createAction(this, tr("Tile"),
        tr("Tile windows"), SLOT(tileSubWindows()));
    cascadeAction_ = createAction(this, tr("Cascade"),
        tr("Cascade windows"), SLOT(cascadeSubWindows()));

    // Administration menu
    loginAction_= createAction(this, tr("Login..."),
        tr("Login as administrator"), SLOT(login()));
    logoutAction_= createAction(this, tr("Logout"),
        tr("Logout from administrator"), SLOT(logout()));
    changePasswordAction_= createAction(this, tr("Change Password..."),
        tr("Change administrator password"), SLOT(changePassword()));
    systemSettingsAction_ = createAction(this, tr("System Settings"),
        tr("Configure system settings"), SLOT(systemSettings()));
    printSetupAction_ = createAction(this, tr("Print Setup"),
        tr("Setup printer"));
    adminReportsAction_ = createAction(this, tr("Reports"),
        tr("Generate procedure reports"), 0, 0, "hi32-reports.png");
    compressionRatioAction_ = createAction(this, tr("Compression Ratio"),
        tr("Set compression ratio"));
    amplifierTestAction_ = createAction(this, tr("Amplifier Test..."),
        tr("Test amplifier"));
    ejectOpticalDiskAction_ = createAction(this, tr("Eject Optical Disk"),
        tr("Eject optical disk"));
    simulatorSettingsAction_ = createAction(this, tr("*Simulator Settings*"),
        tr("Change simulator settings"), SLOT(simulatorSettings()));

    // Hardware menu -- NB No equivalent in Prucka system
    stimulatorAction_ = createAction(this, tr("Stimulator"),
        tr("Open stimulator"), SLOT(openStimulator()));
    satMonitorAction_ = createAction(this, tr("Sat Monitor"),
        tr("Open sat monitor"), SLOT(openSatMonitor()));

    // Help menu
    helpAction_ = createAction(this, tr("EP Simulator Help"),
        tr("EP Simulator help"), SLOT(help()), tr("F1"));
    aboutAction_ = createAction(this, tr("&About EP Simulator"),
        tr("About EP Simulator"), SLOT(about()));
    // only on system toolbar
    manualSaveAction_ = createAction(this, tr("Manual Save"),
        tr("Activate manual saving of data"),
        SIGNAL(manualSave(bool)), tr("F10"), "hi32-savestudytype.png");
    manualSaveAction_->setCheckable(true);
    //connect(manualSaveAction_, SIGNAL(triggered(bool)),
     //   this, SIGNAL(manualSave(bool)));
    // explicitly connect signal/slot to indicate if "checked"
//     connect(manualSaveAction_, SIGNAL(triggered(bool checked)),
//         this, SLOT(manualSaveToggle(checked)));
    autoSaveAction_ = createAction(this, tr("Auto Save"),
        tr("Toggle Auto Save"), SIGNAL(autoSave(bool)),
         tr("Shift+F10"), "hi32-autosavetoggle.png");
    autoSaveAction_->setCheckable(true);
    //connect(autoSaveAction_, SIGNAL(triggered(bool)),
    //    this, SIGNAL(autoSave(bool)));
}

void Recorder::createMenus() {
    studyMenu_ = menuBar()->addMenu(tr("&Study"));
    studyMenu_->addAction(patientInformationAction_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(consciousSedationAction_);
    studyMenu_->addAction(complicationsAction_);
    studyMenu_->addAction(radiologyAction_);
    studyMenu_->addAction(medicationAction_);
    studyMenu_->addAction(suppliesAction_);
    studyMenu_->addAction(staffAction_);
    studyMenu_->addAction(clinicalProceduresAction_);
    studyMenu_->addAction(macrosAction_);
    studyMenu_->addAction(ordersAction_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(reportsAction_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(exportDataAction_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(closeStudyAction_);

    studyConfigurationMenu_ = menuBar()->addMenu(tr("Study &Configuration"));
    studyConfigurationMenu_->addAction(switchAction_);
    studyConfigurationMenu_->addAction(saveAction_);
    studyConfigurationMenu_->addAction(saveAsAction_);
    studyConfigurationMenu_->addSeparator();
    studyConfigurationMenu_->addAction(intervalsAction_);
    studyConfigurationMenu_->addAction(columnFormatsAction_);
    studyConfigurationMenu_->addAction(protocolsAction_);

    measurementsMenu_ = menuBar()->addMenu(tr("&Measurements"));
    measurementsMenu_->addAction(conductionIntervalsAction_);
    measurementsMenu_->addAction(snrtAction_);
    measurementsMenu_->addAction(anteRPsAction_);
    measurementsMenu_->addAction(retroRPsAction_);
    measurementsMenu_->addAction(manualMeasurementsAction_);
    measurementsMenu_->addSeparator();
    measurementsMenu_->addAction(statVitalsAction_);
    measurementsMenu_->addAction(startStopNBPAction_);
    measurementsMenu_->addAction(autoLogVitalsAction_);
    measurementsMenu_->addAction(autoLogNBPAction_);
    measurementsMenu_->addSeparator();
    measurementsMenu_->addAction(measurementConfigurationAction_);
    measurementsMenu_->addSeparator();
    measurementsMenu_->addAction(dataExtractionAction_);

    windowsMenu_ = menuBar()->addMenu(tr("&Windows"));
    windowsMenu_->addAction(winSaveAction_);
    windowsMenu_->addAction(winSaveAsAction_);
    windowsMenu_->addAction(winSwitchAction_);
    windowsMenu_->addAction(winDeleteAction_);
    windowsMenu_->addSeparator();
    windowsMenu_->addAction(timerAction_);
    windowsMenu_->addAction(stopwatchAction_);
    windowsMenu_->addSeparator();
    windowsMenu_->addAction(realTimeAction_);
    windowsMenu_->addAction(review1Action_);
    windowsMenu_->addAction(review2Action_);
    windowsMenu_->addAction(logAction_);
    windowsMenu_->addAction(ablationAction_);
    windowsMenu_->addAction(alignmentAction_);
    windowsMenu_->addAction(mapAction_);
    windowsMenu_->addAction(holterAction_);
    windowsMenu_->addAction(plotAction_);
    windowsMenu_->addAction(macroAction_);
    windowsMenu_->addAction(image1Action_);
    windowsMenu_->addAction(image2Action_);
    windowsMenu_->addAction(imageLibraryAction_);
    windowsMenu_->addSeparator();
    windowsMenu_->addAction(tileAction_);
    windowsMenu_->addAction(cascadeAction_);

    administrationMenu_ = menuBar()->addMenu(tr("&Administration"));
    securitySubMenu_ = new QMenu(tr("Security"));
    securitySubMenu_->addAction(loginAction_);
    securitySubMenu_->addAction(logoutAction_);
    securitySubMenu_->addAction(changePasswordAction_);
    administrationMenu_->addMenu(securitySubMenu_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(systemSettingsAction_);
    administrationMenu_->addAction(printSetupAction_);
    administrationMenu_->addAction(adminReportsAction_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(compressionRatioAction_);
    administrationMenu_->addAction(amplifierTestAction_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(ejectOpticalDiskAction_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(simulatorSettingsAction_);

    hardwareMenu_ = menuBar()->addMenu(tr("&Hardware"));
    hardwareMenu_->addAction(stimulatorAction_);
    hardwareMenu_->addAction(satMonitorAction_);

    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(helpAction_);
    helpMenu_->addAction(aboutAction_);
    
}

void Recorder::createToolBars() {
    QToolBar* systemToolBar = new QToolBar(tr("System")); 
    systemToolBar->setObjectName("SystemToolBar");
    systemToolBar->setAutoFillBackground(true);
    systemToolBar->addAction(closeStudyAction_);
    systemToolBar->addAction(manualSaveAction_);
    systemToolBar->addAction(autoSaveAction_);
    systemToolBar->addAction(adminReportsAction_);
    systemToolBar->addAction(switchAction_);
    switchedVideoComboBox_ = new QComboBox(this);
    switchedVideoComboBox_->addItem(tr("Real-Time"));
    switchedVideoComboBox_->addItem(tr("Review"));
    switchedVideoComboBox_->addItem(tr("Image"));
    switchedVideoComboBox_->setMinimumWidth(200);
    systemToolBar->addWidget(switchedVideoComboBox_);
    systemToolBar->addSeparator();
    /// TODO replace below with a dynamic combobox generated from
    /// a list of protocols.  
    protocolComboBox_ = new QComboBox(this);
    protocolComboBox_->addItem(tr("Baseline"));
    protocolComboBox_->setMinimumWidth(100);
    systemToolBar->addWidget(protocolComboBox_);
  
    addToolBar(systemToolBar);
}

void Recorder::updateMenus() {
    simulatorSettingsAction_->setVisible(
        EpGui::showSimulatorSettings(options_, user_));
    QList<QMdiSubWindow*> windowList = centralWidget_->subWindowList();
    bool realTimePresent = windowList.contains(realTimeSubWindow_);
    bool review1Present = windowList.contains(review1SubWindow_);
    bool review2Present = windowList.contains(review2SubWindow_);
    bool logPresent = windowList.contains(logSubWindow_);
    realTimeAction_->setChecked(realTimePresent);
    review1Action_->setChecked(review1Present);
    // only allow Review2 if there is Review1
    review2Action_->setEnabled(review1Present); 
    review2Action_->setChecked(review2Present);
    logAction_->setChecked(logPresent);
    realTimeAction_->setEnabled(options_->
        filePathFlags.testFlag(Options::EnableAcquisition));
}


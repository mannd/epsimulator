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
#include "patientdialog.h"
#include "patientstatusbar.h"
#include "realtimewindow.h"
#include "recorderdefs.h"
#include "reviewwindow.h"
#include "settings.h"
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
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include <QSplitter>
#include <QStatusBar>
#include <QToolBar>
#include <QVariant>

#include <cassert>

namespace EpRecorder {

Recorder::Recorder(QWidget* parent, 
                   Study* study, 
                   OpticalDisk* currentDisk): 
                   QMainWindow(parent), 
                   openDisplayWindowList_(LastDisplayWindow + 1),
                   study_(study), 
                   user_(User::instance()),
                   options_(Options::instance()),
                   currentDisk_(currentDisk),
                   realTimeWindow_(0),
                   review1Window_(0),
                   review2Window_(0),
                   logWindow_(0),
                   realTimeSubWindow_(0),
                   review1SubWindow_(0),
                   review2SubWindow_(0),
                   logSubWindow_(0) {
    // ensure the Recorder window, PatientStatusBar, floating
    // hardware windows... the whole kit and kaboodle are deleted
    // when the window closes.  As long as the Navigator window 
    // is first made visible, the application will not close.
    setAttribute(Qt::WA_DeleteOnClose);

    assert(parent != 0);  // never call Recorder without parent

    // must build menus and toolbars before creating the central widget,
    // since createCentralWidget() updates the menus.
    createActions();
    createMenus();
    createToolBars();
    createPatientStatusBar();
    createStatusBar();
    createCentralWidget();

 

    patient_ = new Patient;
    patient_->setPath(study_->path());
    patient_->load();
    patientStatusBar_->setPatient(patient_);
    patientStatusBar_->setPatientInfo(study_->name(), 
        study_->weight(), study_->bsa());
    patientStatusBar_->start();
    connect(patientStatusBar_, SIGNAL(manualSave(bool)),
        this, SLOT(setManualSave(bool)));
    connect(centralWidget_, SIGNAL(subWindowActivated(QMdiSubWindow*)),
        this, SLOT(updateMenus()));


    updateAll();
}

void Recorder::createCentralWidget() {
    centralWidget_ = new QMdiArea;
    setCentralWidget(centralWidget_);
    readSettings();
    // deal with no saved settings (first run-through of program).
    ///  TODO this must be altered for multihead system.
    if (centralWidget_->subWindowList().isEmpty()) {
        if (Options::instance()->enableAcquisition()) {
            realTimeWindow_ = new RealTimeWindow;
            realTimeSubWindow_ = centralWidget_->addSubWindow(realTimeWindow_);
            centralWidget_->tileSubWindows();
        }
        else {
            review1Window_ = new ReviewWindow(1);
            review1SubWindow_ = centralWidget_->addSubWindow(review1Window_);
            logWindow_ = new LogWindow;
            logSubWindow_ = centralWidget_->addSubWindow(logWindow_);
            // trying to approximate a good layout for review1 and log windows.
            // Unfortunately centralWidget_ does not give accurate height()
            // and width() values at this stage of construction.
            int w = width();
            int h = height();            
            review1SubWindow_->move(0, 0);
            review1SubWindow_->resize(w, 5 * h / 8);
            logSubWindow_->move(0, 5 * h / 8);
            logSubWindow_->resize(w, h / 4);
        } 
    }

}

void Recorder::setManualSave(bool enable) {
    manualSaveAct_->setChecked(enable);
}

void Recorder::updateWindowTitle() {
    QString title = tr("%1")
        .arg(VersionInfo::instance()->programName());
    /// TODO need to have user stuff in Recorder
    title = User::instance()->isAdministrator() ? 
        QString("%1 %2").arg(title).arg(tr("[Administrator]")) : title;
    setWindowTitle(title);
}

void Recorder::updateAll() {
    updateMenus();
    updateWindowTitle();
}

Recorder::~Recorder() {
    delete patient_;
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
}

void Recorder::systemSettings() {
    if (administrationAllowed()) {
        SystemDialog* systemDialog = new SystemDialog(options_, 
            currentDisk_->studiesPath(), currentDisk_->label(),
            currentDisk_->translatedSide(), this);
        // I'm not sure what the Prucka does, but it's probably
        // crazy to change filepaths while in the Recorder!
        //systemDialog->removeFilePathsTab();
        if (systemDialog->exec() == QDialog::Accepted) {
            systemDialog->setOptions();
            if (!Options::instance()->enableAcquisition() && 
                subWindowIsOpen(realTimeSubWindow_))
                realTimeWindowOpen(false);
        }
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
            // signal updates simulator settings in Navigator
            emit updateSimulatorSettings();
//             if (Navigator* navigator = dynamic_cast<Navigator*>(parent()))
//                 navigator->updateSimulatorSettings();
        }
        delete simDialog;
    }
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
    Settings settings;
    writeSettings(settings);
}

void Recorder::writeSettings(Settings& settings) {
    //Settings settings;
    // save overall Recorder size, position and state
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
    Settings& settings, const QString& currentWindowKey,
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
    Settings settings;
    readSettings(settings);
}

void Recorder::readSettings(Settings& settings) {
    
//    review1Window_->readSettings();
    //Settings settings;
    settings.beginGroup("recorder");
    QVariant size = settings.value("size");
    if (size.isNull()) {
        setWindowState(Qt::WindowMaximized);
        settings.endGroup();
        return;
    }    
    resize(size.toSize());
    move(settings.value("pos").toPoint());
    restoreState(settings.value("state").toByteArray());
    // clear the central widget - close all the subwindows
    centralWidget_->closeAllSubWindows();
    // read a list of the saved subwindows, open each one and 
    // readSettings for each one.
    // Note that below is clunky, but seems best solution after
    // experimenting with less repetitive ways of doing this.
    QStringList subWindowKeys = settings.value("subWindowList").toStringList();
    QString currentWindowKey = settings.value("currentWindowKey").toString();
    QMdiSubWindow* activeWindow = 0;
    // sorry, no RealTime window if no acquisition
    if (subWindowKeys.contains(realTimeWindowKey) &&
        Options::instance()->enableAcquisition()) {
        // apparently can't use pointer to member function here
        // because Recorder is not fully constructed yet, so pointer to
        // member is wild.
        realTimeWindowOpen(true);
        restoreDisplayWindow(realTimeWindowKey, 
            settings, currentWindowKey, realTimeSubWindow_, 
            realTimeWindow_, activeWindow);
    }
    if (subWindowKeys.contains(review1WindowKey)) {
        review1WindowOpen(true);
        restoreDisplayWindow(review1WindowKey, 
            settings, currentWindowKey, review1SubWindow_, 
            review1Window_, activeWindow);
    }
    if (subWindowKeys.contains(review2WindowKey)) {
        review2WindowOpen(true);
        restoreDisplayWindow(review2WindowKey, 
            settings, currentWindowKey, review2SubWindow_, 
            review2Window_, activeWindow);
    }
    if (subWindowKeys.contains(logWindowKey)) {
        logWindowOpen(true);
        restoreDisplayWindow(logWindowKey, 
            settings, currentWindowKey, logSubWindow_, 
            logWindow_, activeWindow);
    }
    /// TODO again add future DisplayWindow processing here.
    settings.endGroup();
    if (activeWindow)
        centralWidget_->setActiveSubWindow(activeWindow);
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
        EpGui::changePassword(this, options_);
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
    centralWidget_->tileSubWindows();
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
    //. if (options_->recorderStatusBar())
        statusBar()->showMessage(QString());
}

void Recorder::createPatientStatusBar() {
    patientStatusBar_ = new PatientStatusBar(this);
    QDockWidget* bottomDockWidget = new QDockWidget(this);
    bottomDockWidget->setObjectName("bottomDockWidget");
    bottomDockWidget->setWidget(patientStatusBar_);
    bottomDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    //bottomDockWidget->setTitleBarWidget(0);
    bottomDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, bottomDockWidget);
}

void Recorder::createActions()
{
    // It appears the Prucka does not have typical shortcut keys 
    // and accelerator keys due to keyboard relabeling.  
    // We'll provide some anyway.
    // Note that the keyboard hints are totally non-mnemotic.  Should
    // have a function that delivers the right QString, depending on whether
    // using Prucka compatible or simplified menu, or just have a separate
    // set of menus -- probably that is easier -- for each configuration
    using EpGui::createAction;
    patientInformationAct_ = createAction(this, tr("Patient Information"), 
        tr("Create and modify patient information"),
        SLOT(patientInformation()));    
    consciousSedationAct_ = createAction(this, tr("Conscious Sedation"),
        tr("Conscious sedation list"), 0, tr("Alt+A"));
    complicationsAct_ = createAction(this, tr("Complications"), 
        tr("Complications list"), 0, tr("Alt+M"));
    radiologyAct_ = createAction(this, tr("Radiology"), 
        tr("Radiology list"), 0, tr("Alt+N"));
    medicationAct_ = createAction(this, tr("Medications"), 
        tr("Medication list"), 0, tr("Alt+B"));
    suppliesAct_ = createAction(this, tr("Supplies"),
        tr("Supplies list"), 0, tr("Alt+V"));
    staffAct_ = createAction(this, tr("Staff"), 
        tr("Staff list"), 0, tr("Alt+C"));
    clinicalProceduresAct_ = createAction(this, tr("Clinical Procedures"), 
        tr("Clinical procedures list"), 0, tr("Alt+X"));
    macrosAct_ = createAction(this, tr("Macros"), 
        tr("Macros list"), 0, tr("Alt+Z"));
    ordersAct_ = createAction(this, tr("Orders"), 
        tr("Enter orders"));
    reportsAct_ = createAction(this, tr("Reports..."), 
        tr("Create procedure reports"));
    exportDataAct_ = createAction(this, tr("Export Data"), 
        tr("Export data to external formats"));
    closeStudyAct_ = createAction(this, tr("Close Study"),
        tr("Close patient study"), SLOT(close()), 0,
        "hi32-closestudy.png");
    // Study Configuration
    switchAct_ = createAction(this, tr("Switch..."), 
        tr("Switch study configuration"), 0, 
        0, "hi32-switchwindowsettings.png");
    saveAct_ = createAction(this, tr("Save"),
        tr("Save study configuration"));
    saveAsAct_ = createAction(this, tr("Save As..."), 
        tr("Save study configuration under different name"));
    intervalsAct_ = createAction(this, tr("Intervals"), 
        tr("Configure study intervals"));
    columnFormatsAct_ = createAction(this, tr("Column Formats"), 
        tr("Configure column formats"));
    protocolsAct_ = createAction(this, tr("Protocols"), 
        tr("Configure protocols"));
    // Measurements
    conductionIntervalsAct_ = createAction(this, tr("Conduction Intervals..."),
        tr("Measure conduction intervals"), 0, tr("Alt+Q"));
    snrtAct_ = createAction(this, tr("SNRT..."), 
        tr("Measure sinus node recover time"), 0, tr("Alt+W"));
    anteRPsAct_ = createAction(this, tr("Ante RPs..."), 
        tr("Measure anterograde refractory periods"), 0, tr("Alt+E"));
    retroRPsAct_ = createAction(this, tr("Retro RPs..."), 
        tr("Measure retrograde refractory periods"), 0, tr("Alt+R"));
    manualMeasurementsAct_ = createAction(this, tr("Manual Measurements"),
        tr("Make manual measurements"));
    statVitalsAct_ = createAction(this, tr("Stat Vitals"),
        tr("Record stat vital signs"));
    startStopNBPAct_ = createAction(this, tr("Start/Stop NBP"),
        tr("Start or stop noninvasive BP measurements"), 0, tr("F8"));
    autoLogVitalsAct_ = createAction(this, tr("Auto-Log Vitals"),
        tr("Auto-log vital signs"));
    autoLogVitalsAct_->setCheckable(true);
    autoLogNBPAct_ = createAction(this, tr("Auto-Log NBP"),
        tr("Auto-log noninvasive BP measurements"));
    autoLogNBPAct_->setCheckable(true);
    measurementConfigurationAct_ = createAction(this, 
        tr("Measurement Configuration"),
        tr("Configure measurements"));
    dataExtractionAct_ = createAction(this, tr("Data Extraction"),
        tr("Extract data"));
    // Windows Menu
    winSaveAct_ = createAction(this, tr("Save"),
        tr("Save window configuration"));
    winSaveAsAct_ = createAction(this, tr("Save As..."),
        tr("Save windows configuration as..."));
    winSwitchAct_ = createAction(this, tr("Switch..."),
        tr("Switch windows configuration"));
    winDeleteAct_ = createAction(this, tr("Delete..."),
        tr("Delete windows configuration"));
    timerAct_ = createAction(this, tr("Timer"),
        tr("Show timer"));
    timerAct_->setCheckable(true);
    stopwatchAct_ = createAction(this, tr("Stopwatch"),
        tr("Show stopwatch"));
    stopwatchAct_->setCheckable(true);
    realTimeAct_ = createAction(this, tr("Real-Time"),
       tr("Show real-time window"));
    realTimeAct_->setCheckable(true);
    connect(realTimeAct_, SIGNAL(triggered(bool)),
        this, SLOT(realTimeWindowOpen(bool)));
    review1Act_ = createAction(this, tr("Review 1"),
       tr("Show review 1 window"));
    review1Act_->setCheckable(true);
    connect(review1Act_, SIGNAL(triggered(bool)),
        this, SLOT(review1WindowOpen(bool)));
    review2Act_ = createAction(this, tr("Review 2"),
       tr("Show review 2 window"));
    review2Act_->setCheckable(true);
    connect(review2Act_, SIGNAL(triggered(bool)),
        this, SLOT(review2WindowOpen(bool)));
    logAct_ = createAction(this, tr("Log"),
       tr("Show log window"));
    logAct_->setCheckable(true);
    connect(logAct_, SIGNAL(triggered(bool)),
        this, SLOT(logWindowOpen(bool)));
    // The windows below will be implemented in later versions of EP Simulator
    ablationAct_ = createAction(this, tr("Ablation"),
       tr("Show ablation window"));
    ablationAct_->setCheckable(true);
    ablationAct_->setEnabled(false);
    alignmentAct_ = createAction(this, tr("Alignment"),
       tr("Show alignment window"));
    alignmentAct_->setCheckable(true);
    alignmentAct_->setEnabled(false);
    mapAct_ = createAction(this, tr("Map"),
       tr("Show map window"));
    mapAct_->setCheckable(true);
    mapAct_->setEnabled(false);
    holterAct_ = createAction(this, tr("Holter"),
       tr("Show holter window"));
    holterAct_->setCheckable(true);
    holterAct_->setEnabled(false);
    plotAct_ = createAction(this, tr("Plot"),
       tr("Show plot window"));
    plotAct_->setCheckable(true);
    plotAct_->setEnabled(false);
    macroAct_ = createAction(this, tr("Macro"),
       tr("Show macro window"));
    macroAct_->setCheckable(true);
    macroAct_->setEnabled(false);
    image1Act_ = createAction(this, tr("Image 1"),
       tr("Show image 1 window"));
    image1Act_->setCheckable(true);
    image1Act_->setEnabled(false);
    image2Act_ = createAction(this, tr("Image 2"),
       tr("Show image 2 window"));
    image2Act_->setCheckable(true);
    image2Act_->setEnabled(false);
    imageLibraryAct_ = createAction(this, tr("Image Library"),
       tr("Show image library window"));
    imageLibraryAct_->setCheckable(true);
    imageLibraryAct_->setEnabled(false);
    tileAct_ = createAction(this, tr("Tile"),
        tr("Tile windows"), SLOT(tileSubWindows()));
    cascadeAct_ = createAction(this, tr("Cascade"),
        tr("Cascade windows"), SLOT(cascadeSubWindows()));

    // Administration menu
    loginAct_= createAction(this, tr("Login..."),
        tr("Login as administrator"), SLOT(login()));
    logoutAct_= createAction(this, tr("Logout"),
        tr("Logout from administrator"), SLOT(logout()));
    changePasswordAct_= createAction(this, tr("Change Password..."),
        tr("Change administrator password"), SLOT(changePassword()));
    systemSettingsAct_ = createAction(this, tr("System Settings"),
        tr("Configure system settings"), SLOT(systemSettings()));
    printSetupAct_ = createAction(this, tr("Print Setup"),
        tr("Setup printer"));
    adminReportsAct_ = createAction(this, tr("Reports"),
        tr("Generate procedure reports"), 0, 0, "hi32-reports.png");
    compressionRatioAct_ = createAction(this, tr("Compression Ratio"),
        tr("Set compression ratio"));
    amplifierTestAct_ = createAction(this, tr("Amplifier Test..."),
        tr("Test amplifier"));
    ejectOpticalDiskAct_ = createAction(this, tr("Eject Optical Disk"),
        tr("Eject optical disk"));
    simulatorSettingsAct_ = createAction(this, tr("*Simulator Settings*"),
        tr("Change simulator settings"), SLOT(simulatorSettings()));

    // Hardware menu -- NB No equivalent in Prucka system
    stimulatorAct_ = createAction(this, tr("Stimulator"),
        tr("Open stimulator"), SLOT(openStimulator()));
    satMonitorAct_ = createAction(this, tr("Sat Monitor"),
        tr("Open sat monitor"), SLOT(openSatMonitor()));

    // Help menu
    helpAct_ = createAction(this, tr("EP Simulator Help"),
        tr("EP Simulator help"), SLOT(help()), tr("F1"));
    aboutAct_ = createAction(this, tr("&About EP Simulator"),
        tr("About EP Simulator"), SLOT(about()));
    // only on system toolbar
    manualSaveAct_ = createAction(this, tr("Manual Save"),
        tr("Activate manual saving of data"),
        0, 0, "hi32-savestudytype.png");
    manualSaveAct_->setCheckable(true);
    connect(manualSaveAct_, SIGNAL(triggered(bool)),
        this, SIGNAL(manualSave(bool)));
    // explicitly connect signal/slot to indicate if "checked"
//     connect(manualSaveAct_, SIGNAL(triggered(bool checked)),
//         this, SLOT(manualSaveToggle(checked)));
    autoSaveAct_ = createAction(this, tr("Auto Save"),
        tr("Toggle Auto Save"), 0, 0, "hi32-autosavetoggle.png");
    autoSaveAct_->setCheckable(true);
    connect(autoSaveAct_, SIGNAL(triggered(bool)),
        this, SIGNAL(autoSave(bool)));
}

void Recorder::createMenus() {
    studyMenu_ = menuBar()->addMenu(tr("&Study"));
    studyMenu_->addAction(patientInformationAct_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(consciousSedationAct_);
    studyMenu_->addAction(complicationsAct_);
    studyMenu_->addAction(radiologyAct_);
    studyMenu_->addAction(medicationAct_);
    studyMenu_->addAction(suppliesAct_);
    studyMenu_->addAction(staffAct_);
    studyMenu_->addAction(clinicalProceduresAct_);
    studyMenu_->addAction(macrosAct_);
    studyMenu_->addAction(ordersAct_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(reportsAct_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(exportDataAct_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(closeStudyAct_);

    studyConfigurationMenu_ = menuBar()->addMenu(tr("Study &Configuration"));
    studyConfigurationMenu_->addAction(switchAct_);
    studyConfigurationMenu_->addAction(saveAct_);
    studyConfigurationMenu_->addAction(saveAsAct_);
    studyConfigurationMenu_->addSeparator();
    studyConfigurationMenu_->addAction(intervalsAct_);
    studyConfigurationMenu_->addAction(columnFormatsAct_);
    studyConfigurationMenu_->addAction(protocolsAct_);

    measurementsMenu_ = menuBar()->addMenu(tr("&Measurements"));
    measurementsMenu_->addAction(conductionIntervalsAct_);
    measurementsMenu_->addAction(snrtAct_);
    measurementsMenu_->addAction(anteRPsAct_);
    measurementsMenu_->addAction(retroRPsAct_);
    measurementsMenu_->addAction(manualMeasurementsAct_);
    measurementsMenu_->addSeparator();
    measurementsMenu_->addAction(statVitalsAct_);
    measurementsMenu_->addAction(startStopNBPAct_);
    measurementsMenu_->addAction(autoLogVitalsAct_);
    measurementsMenu_->addAction(autoLogNBPAct_);
    measurementsMenu_->addSeparator();
    measurementsMenu_->addAction(measurementConfigurationAct_);
    measurementsMenu_->addSeparator();
    measurementsMenu_->addAction(dataExtractionAct_);

    windowsMenu_ = menuBar()->addMenu(tr("&Windows"));
    windowsMenu_->addAction(winSaveAct_);
    windowsMenu_->addAction(winSaveAsAct_);
    windowsMenu_->addAction(winSwitchAct_);
    windowsMenu_->addAction(winDeleteAct_);
    windowsMenu_->addSeparator();
    windowsMenu_->addAction(timerAct_);
    windowsMenu_->addAction(stopwatchAct_);
    windowsMenu_->addSeparator();
    windowsMenu_->addAction(realTimeAct_);
    windowsMenu_->addAction(review1Act_);
    windowsMenu_->addAction(review2Act_);
    windowsMenu_->addAction(logAct_);
    windowsMenu_->addAction(ablationAct_);
    windowsMenu_->addAction(alignmentAct_);
    windowsMenu_->addAction(mapAct_);
    windowsMenu_->addAction(holterAct_);
    windowsMenu_->addAction(plotAct_);
    windowsMenu_->addAction(macroAct_);
    windowsMenu_->addAction(image1Act_);
    windowsMenu_->addAction(image2Act_);
    windowsMenu_->addAction(imageLibraryAct_);
    windowsMenu_->addSeparator();
    windowsMenu_->addAction(tileAct_);
    windowsMenu_->addAction(cascadeAct_);

    administrationMenu_ = menuBar()->addMenu(tr("&Administration"));
    securitySubMenu_ = new QMenu(tr("Security"));
    securitySubMenu_->addAction(loginAct_);
    securitySubMenu_->addAction(logoutAct_);
    securitySubMenu_->addAction(changePasswordAct_);
    administrationMenu_->addMenu(securitySubMenu_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(systemSettingsAct_);
    administrationMenu_->addAction(printSetupAct_);
    administrationMenu_->addAction(adminReportsAct_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(compressionRatioAct_);
    administrationMenu_->addAction(amplifierTestAct_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(ejectOpticalDiskAct_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(simulatorSettingsAct_);

    hardwareMenu_ = menuBar()->addMenu(tr("&Hardware"));
    hardwareMenu_->addAction(stimulatorAct_);
    hardwareMenu_->addAction(satMonitorAct_);

    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(helpAct_);
    helpMenu_->addAction(aboutAct_);
    
}

void Recorder::createToolBars() {
    QToolBar* systemToolBar = new QToolBar(tr("System")); 
    systemToolBar->setObjectName("SystemToolBar");
    systemToolBar->setAutoFillBackground(true);
    systemToolBar->addAction(closeStudyAct_);
    systemToolBar->addAction(manualSaveAct_);
    systemToolBar->addAction(autoSaveAct_);
    systemToolBar->addAction(adminReportsAct_);
    systemToolBar->addAction(switchAct_);
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
    simulatorSettingsAct_->setVisible(
        EpGui::showSimulatorSettings(options_, user_));
    QList<QMdiSubWindow*> windowList = centralWidget_->subWindowList();
    bool realTimePresent = windowList.contains(realTimeSubWindow_);
    bool review1Present = windowList.contains(review1SubWindow_);
    bool review2Present = windowList.contains(review2SubWindow_);
    bool logPresent = windowList.contains(logSubWindow_);
    realTimeAct_->setChecked(realTimePresent);
    review1Act_->setChecked(review1Present);
    // only allow Review2 if there is Review1
    review2Act_->setEnabled(review1Present); 
    review2Act_->setChecked(review2Present);
    logAct_->setChecked(logPresent);
    realTimeAct_->setEnabled(Options::instance()->enableAcquisition());
}

}

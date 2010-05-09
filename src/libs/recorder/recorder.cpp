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
#include "amplifier.h"
#include "itemlist.h"
#include "logwindow.h"
#include "opticaldisk.h"
#include "patient.h"
#include "patientdialog.h"
#include "patientstatusbar.h"
#include "realtimewindow.h"
#include "recorderdefs.h"
#include "reviewwindow.h"
#include "satmonitor.h"
#include "selectstudyconfigdialog.h"
#include "stimulator.h"
#include "study.h"
#include "studyconfiguration.h"
#include "teststimulator.h"
#include "user.h"
#include "windowsetting.h"

#include <QAction>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QCloseEvent>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>
#include <QVariant>

#include <QtDebug>

#include <QMouseEvent>

using EpCore::ItemList;
using EpCore::Options;
using EpCore::User;
using EpGui::AbstractMainWindow;
using EpGui::PatientDialog;
using EpGui::SelectStudyConfigDialog;
using EpPatient::Patient;
using EpRecorder::Recorder;
using EpStudy::Protocol;
using EpStudy::Study;
using EpStudy::StudyConfiguration;
using EpStudy::StudyConfigurations;
using EpStudy::StudyInformationDialog;

using namespace EpHardware;
using namespace EpHardware::EpAmplifier;
using namespace EpHardware::EpOpticalDisk;
using namespace EpHardware::EpStimulator;

const int Recorder::edgeWidth;

Recorder::Recorder(QWidget* parent, 
                   Study* study, 
                   OpticalDisk* currentDisk,
                   User* user,
                   bool allowAcquisition,
                   RecorderWindow recorderWindow)
                   : 
                   AbstractMainWindow(parent), 
                   openDisplayWindowList_(LastDisplayWindow + 1),
                   study_(study), 
                   user_(user),
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
    Q_ASSERT(study_ != 0);  // should never be called with a null Study

    setAttribute(Qt::WA_DeleteOnClose);

    if (recorderWindow_ == Primary &&
        epOptions->screenFlags.testFlag(Options::TwoRecorderWindows)) {
        Recorder* recorder = new Recorder(this, study_, 
            currentDisk_, user_, false, Secondary);
        recorder->restore();
    }

    loadAmplifier();
    study_->loadStudyConfiguration();
    // applyProtocol(study_->studyConfiguration()->currentProtocol();
    qDebug() << "Study configuration name = "
            << study_->studyConfiguration()->name();
    qDebug() << "Protocol name = "
            << study_->studyConfiguration()->currentProtocol().name();
    qDebug() << "Window Settings = "
            << study_->studyConfiguration()->currentProtocol().windowSetting()
            .name();
    loadPatient();

    // must build menus and toolbars before creating the central widget,
    // since createCentralWidget() updates the menus.
    createActions();
    createMenus();
    createToolBars();
    createPatientStatusBar();
    createStatusBar();
    createCentralWidget();

    // connect(parent, SIGNAL(opticalDiskChanged(OpticalDisk*)),
    //    this, SLOT(changeOpticalDisk(OpticalDisk*)));
    connect(patientStatusBar_, SIGNAL(manualSave(bool)),
        this, SLOT(setManualSave(bool)));
    connect(this, SIGNAL(emergencySave(bool)),
        this, SLOT(setEmergencySave(bool)));
    connect(patientStatusBar_, SIGNAL(showPatientInformation()),
        this, SLOT(patientInformation()));
    connect(centralWidget_, SIGNAL(subWindowActivated(QMdiSubWindow*)),
        this, SLOT(updateMenus()));
    connect(this, SIGNAL(patientInformationClosed()),
        patientStatusBar_, SLOT(patientInformationClosed()));

    connect(this, SIGNAL(displayWindowResized(QWidget*)),
        this, SLOT(resizeDisplayWindows(QWidget*)));

    /// TODO
    // if (recorderWindow_ == Secondary) {
    // /* connect signals and slots that only affect Secondary window */
    // }

    /// FIXME this is bad! This is probably why initial screen doesn't
    /// work because you are updating in the constructor!!!!!!!!!!
    updateAll();

}

Recorder::~Recorder() {
    writeSettings();
    if (recorderWindow_ == Primary) {
        delete patient_;
        // Recorder took possession of study_, so has to kill it now.
        delete study_;
    }
}

void Recorder::loadAmplifier() {
    // amplifier_->load();
}

void Recorder::loadPatient() {
    patient_ = new Patient;
    patient_->setPath(study_->path());
    patient_->load();
}

void Recorder::resizeDisplayWindows(QWidget*) {
    qDebug() << "new new " << static_cast<QMdiArea*>(centralWidget_)->
            activeSubWindow()->size();
}

/**
 * Defines area in central Widget that mouse can't drag.
 * @return true if in the edges of the central Widget.
 */
inline bool Recorder::noMansZone(const QPoint& p) {
    QPoint origin = centralWidget_->mapToGlobal(QPoint(0,0));
    return (p.x() < origin.x() + edgeWidth) ||
           (p.x() > origin.x() + centralWidget_->width() - edgeWidth) ||
           (p.y() < origin.y() + edgeWidth) ||
           (p.y() > origin.y() + centralWidget_->height() - edgeWidth);
}

bool Recorder::eventFilter(QObject* target, QEvent* event) {
    // if we are emulating the windows event manager then we must:
    //     1) prevent moving the mdisubwindows around in the central widget
    //     2) prevent resizing the outside edges of the mdisubwindows
    //     3) retile the mdisubwindows when the mouse if released

    /// FIXME Goals 1 and 2 work now, at least in KDE and Linux.
    /// Need to retile based on window positions when mouse button released.
    /// One way to do this would be to take the sizes of the focus window,
    /// if it is the log window, change the other windows' height to that
    /// of the central widget - the log window height, otherwise
    /// change the log window top position to the bottom of the resized
    /// window and change the other windows accordingly.

    // no event filtering unless we are emulating the Prucka windows manager
    if (!epOptions->screenFlags.testFlag(Options::EmulateWindowsManager))
        return QMainWindow::eventFilter(target, event);
    static bool inNoMansZone = false;
    static bool atStartPosition = true;
    if (event->type() == QEvent::MouseMove) {
         QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
         if (mouseEvent->buttons() & Qt::LeftButton)   {
            if (atStartPosition) {
                //qDebug() << "mouseEvent->y() = " << mouseEvent->y();
                inNoMansZone = noMansZone(mouseEvent->globalPos()) ||
                  (4 < mouseEvent->y() && mouseEvent->y() < 30);
                //qDebug() << "inNoMansZone = " << inNoMansZone;
                //qDebug() << "mouse globalPos() = " << mouseEvent->globalPos()
                //         << " mouse pos() = " << mouseEvent->pos();
                atStartPosition = false;
                if (inNoMansZone)
                    QApplication::setOverrideCursor(QCursor(Qt::ForbiddenCursor));
           }
            if (inNoMansZone)
                return true;
         }
        else {
            inNoMansZone = false;   // reset this
            atStartPosition = true;
        }
    }
    QApplication::restoreOverrideCursor();
    return QMainWindow::eventFilter(target, event);
}

void Recorder::createCentralWidget() {
    centralWidget_ = new QMdiArea;
    setCentralWidget(centralWidget_);
    readSettings();
}

/// TODO Further complexities:
/** Tiling is really different from the initial setup.  If we are not
 using the Prucka windows manager, then opening and closing subwindows,
 tiling, cascading, etc. should be totally adlib.  The only exception
 is that you should not be able to ever open the realtimewindows
 if enable acquisition is not enabled.  We'll also limit review2 opening
 to having review1 present.  That means you can't close review1 if 
 review2 is open...

 With windows manager emulation, tiling will depend on the windows open.
 The only way to implement that is to manage the close buttons, the
 system menus and disabling and enabling the Window menu items.  We should
 allow closing the log and Review1 windows in emulation mode with 1 screen.
 Tiling in windows emulation mode will need to account for that.
*/ 

/// FIXME  Because of above, closing mdisubwindows doesn't work.

/// FIXME resizing screen forces extra setupInitialScreen(true) when
/// recorder is opened.  It should just do tiling.

/// TODO need to be able to save and restore windows arrangements.

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
    // The beginning and ending lines of this routine, and the 
    // need for bool tile are a KLUDGE, possibly only required
    // with X11, because of difficult X11 window manager features,
    // but not tested on the other platforms so who knows?
    // The problem is the first time the Recorder central widget
    // is set up, the value for height() is different from
    // subsequent times, so that the subwindows move slightly
    // between the initial appearance and any time retiling
    // occurs.  The heights are different too depending
    // on whether the status bar is shown or not.
    // SO... the sequence is:
    // call the Recorder constructor in Navigator
    // recorder->show() in Navigator
    // recorder->setupInitialScreen(false) in Navigator
    // Then setupInitialScreen() draws the subwindows
    // with the wrong height while the screen is hidden
    // and then calls itself with tile = true
    // to draw itself all over again with correct height.
    // This works, with some minor screen flashing; the
    // subwindows don't shift position.
    /// TODO test this function in Windows and Mac Os.
    if (!tile)
        hide();
    centralWidget_->update();
    centralWidget_->closeAllSubWindows();
    if (epOptions->screenFlags.testFlag(Options::TwoRecorderWindows)) {
        if (recorderWindow_ == Primary && allowAcquisition_) {
            realTimeWindowOpen(true);
        }
        else { // recorderWindow_ == Secondary or no Acquistion
            review1WindowOpen(true);
            logWindowOpen(true);
        }
    }
    else {  // only one screen, should just be the Primary screen
        review1WindowOpen(true);
        logWindowOpen(true);
        if (allowAcquisition_)
            realTimeWindowOpen(true);
        // trying to approximate a good layout for review1 and log windows.
        int w = centralWidget()->width();
        int h = centralWidget()->height();
        if (allowAcquisition_) {
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
        }   
        // logSubWindow_ always same position
        logSubWindow_->move(0, h * 2 / 3);
        logSubWindow_->resize(w, h / 3);
    }
    if (!tile) {
//         qDebug() << "Before setupInitialScreen()";
//         qDebug() << "centralWidget_->pos() = " << centralWidget_->pos() 
//              << " centralWidget_->width() = " << centralWidget_->width() 
//              << " centralWidget_->height() = " << centralWidget_->height(); 

        show();
        setupInitialScreen(true);   // run recursively first go-around
    }
    else {  // demonstrates unexplained change in central widget height 2nd time around.
//         qDebug() << "After setupInitialScreen()";
//         qDebug() << "centralWidget_->pos() = " << centralWidget_->pos() 
//              << " centralWidget_->width() = " << centralWidget_->width() 
//              << " centralWidget_->height() = " << centralWidget_->height(); 
    }
}

void Recorder::setManualSave(bool enable) {
    manualSaveAction_->setChecked(enable);
}

void Recorder::setEmergencySave(bool enable) {
    // save emergency save buffer to disk
    emit manualSave(enable);
}

void Recorder::updateAll() {
    //updateMenus();
    updateWindowTitle();
    updateSimulatorSettings();
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

void Recorder::openConsciousSedation() {
    openStudyInformation(StudyInformationDialog::ConsciousSedation);
}

void Recorder::openProcedures() {
    openStudyInformation(StudyInformationDialog::Procedures);
}

void Recorder::openStaff() {
    openStudyInformation(StudyInformationDialog::Staff);
}

void Recorder::openSupplies() {
    openStudyInformation(StudyInformationDialog::Supplies);
}

void Recorder::openMedications() {
    openStudyInformation(StudyInformationDialog::Medications);
}

void Recorder::openRadiology() {
    openStudyInformation(StudyInformationDialog::Radiology);
}

void Recorder::openOrders() {
    openStudyInformation(StudyInformationDialog::Orders);
}

void Recorder::openComplications() {
    openStudyInformation(StudyInformationDialog::Complications);
}

void Recorder::openContrast() {
    openStudyInformation(StudyInformationDialog::Contrast);
}

void Recorder::switchStudyConfiguration() {
    SelectStudyConfigDialog s(this);
    s.setStudyConfiguration(*study_->studyConfiguration());
    if (s.exec() == QDialog::Accepted) {
        const QString configName = s.config();
        // don't bother unless study configuration is changed
        if (configName != study_->studyConfiguration()->name()) {
            StudyConfigurations configList;
            if (configList.isPresent(configName)) {
                study_->setStudyConfiguration(
                    configList.studyConfiguration(configName));
                resetProtocolComboBox();
            }
        }
    }
}

void Recorder::setProtocol(int index) {
    study_->studyConfiguration()->setCurrentProtocolIndex(index);
}

void Recorder::winSave() {
    if (administrationAllowed()) {
        ItemList<WindowSetting> windows;
        WindowSetting w("test");
        windows[w.name()] = w;
        windows.update();
    }
}

void Recorder::winSaveAs() {
    if (administrationAllowed()) {


    }
}

void Recorder::winSwitch() {
    if (administrationAllowed())
        filler();
}

void Recorder::winDelete() {
    if (administrationAllowed())
        filler();
}

void Recorder::openStudyInformation(StudyInformationDialog::List list) {
    StudyInformationDialog d(this);
    d.openList(list);
    if (d.exec() == QDialog::Accepted) {
       filler();
    }
}

void Recorder::updateSystemSettings() {
    /// TODO change optical drive, whatever
    emit systemSettingsChanged();   // let Navigator know
}

void Recorder::updateSimulatorSettings() {
    QDockWidget* dockWidget =
        qobject_cast<QDockWidget*>(patientStatusBar_->parentWidget());
    dockWidget->setWindowTitle(epOptions->recorderFlags
                               .testFlag(Options::PatientStatusBarHasTitle)
        ? tr("Patient Status") : "");
    if (epOptions->recorderFlags.testFlag(Options::ImmovablePatientStatusBar))
        dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    else
        dockWidget->setFeatures(QDockWidget::DockWidgetClosable
            | QDockWidget::DockWidgetMovable 
            | QDockWidget::DockWidgetFloatable);
    updateMenus();
    //setupInitialScreen(true);
    // signal update simulator settings in Navigator
    emit simulatorSettingsChanged();
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
    openSubWindow(open, realTimeSubWindow_, realTimeWindow_, study_);
    // no system menu or close button if we are emulating Prucka
    if (open && epOptions->screenFlags.testFlag(Options::EmulateWindowsManager))
        realTimeSubWindow_->setWindowFlags(realTimeSubWindow_->windowFlags() 
            & ~Qt::WindowSystemMenuHint);
}

void Recorder::review1WindowOpen(bool open) {
     // if closing and review2 is open, close that too
    if (!open && subWindowIsOpen(review2SubWindow_))
        review2WindowOpen(false);
    openSubWindow(open, review1SubWindow_, review1Window_, study_, 1);
    if (open) {
        connectReviewWindows();
        review1Window_->makeWindowActive(true);
   }
} 

void Recorder::review2WindowOpen(bool open) {
    // must have a review1window open first
    if (open && !subWindowIsOpen(review1SubWindow_))
        return;
    openSubWindow(open, review2SubWindow_, review2Window_, study_, 2);
    if (open) {
        connectReviewWindows();
        review2Window_->makeWindowActive(false);
    }
} 

void Recorder::logWindowOpen(bool open) {
    openSubWindow(open, logSubWindow_, logWindow_, study_);
}

void Recorder::closeEvent(QCloseEvent *event) {
    if (recorderWindow_ == Secondary) {
        QMessageBox::information(this, tr("Close Primary Window First"),
                                 tr("Please close the RealTime window first."));
        event->ignore();
    }
    else if (closeStudy()) {
        study_->save();
        patient_->save();
        patientStatusBar_->stop();
        if (parentWidget())
            parentWidget()->show();        
        event->accept();
    }
    else
        event->ignore();
}

void Recorder::resizeEvent(QResizeEvent*) {
    if (epOptions->screenFlags.testFlag(Options::EmulateWindowsManager))
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
    int recorderNumber = (recorderWindow_ == Primary ? 1 : 2);
    settings.beginGroup(QString("recorder%1").arg(recorderNumber));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("size", size());
    settings.setValue("pos", pos()); 
    settings.setValue("state", saveState());
    // erase previously save list of open display windows
    settings.remove("subWindowList"); 
    // erase empty list of open display window names
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
        settings.setValue("geometry", subWindowList[i]->saveGeometry());
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
    int recorderNumber = (recorderWindow_ == Primary ? 1 : 2);
    settings.beginGroup(QString("recorder%1").arg(recorderNumber));
    QVariant size = settings.value("size");
    if (size.isNull()) {
        setWindowState(windowState() ^ Qt::WindowMaximized);
    }
    else {
#ifdef Q_OS_WIN32
        restoreGeometry(settings.value("geometry").toByteArray());
#else   // Mac or Linux
        // restoreGeometry doesn't work on X11
        // see Window Geometry section of Qt Reference Doc
        resize(size.toSize());
        move(settings.value("pos").toPoint());
#endif
        restoreState(settings.value("state").toByteArray());
    }
    settings.endGroup();
}

void Recorder::openStimulator() {
    Stimulator* stimulator = new TestStimulator(this);
    stimulator->show();
}

void Recorder::openSatMonitor() {
       SatMonitor* satMonitor = new SatMonitor(this);
       satMonitor->show();
}

void Recorder::tileSubWindows() {
    if (epOptions->screenFlags.testFlag(Options::EmulateWindowsManager) ||
        epOptions->screenFlags.testFlag(Options::EmulatePruckaTiling))
        // Prucka-like tiling
        setupInitialScreen(true);
    else
        // regular tiling        
        centralWidget_->tileSubWindows();
    // need to emit signals to other Recorder window
}

void Recorder::cascadeSubWindows() {
    centralWidget_->cascadeSubWindows();
    // emit cascade signal to other Recorder window
}

/**
 * Gets the statusbar going, with a blank message.
 * Temporary messages from menu functions, etc. will
 * now show up here.  The "Press F1 for Help" message
 * that appears on the Navigator statusbar doesn't work
 * well here, since a frame is drawn around any widget
 * inserted into the statusbar, which looks ugly adjacent
 * to the PatientStatusBar.
 */
void Recorder::createStatusBar() {
    if (epOptions->recorderFlags.testFlag(Options::RecorderHasStatusBar))
        statusBar()->showMessage(QString());
    else
        statusBar()->hide();
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
        tr("Conscious sedation list"),
        SLOT(openConsciousSedation()) ,
        tr("Alt+A"));
    complicationsAction_ = createAction(this, tr("Complications"), 
        tr("Complications list"),
        SLOT(openComplications()), tr("Alt+M"));
    radiologyAction_ = createAction(this, tr("Radiology"), 
        tr("Radiology list"),
        SLOT(openRadiology()), tr("Alt+N"));
    medicationAction_ = createAction(this, tr("Medications"), 
        tr("Medication list"),
        SLOT(openMedications()), tr("Alt+B"));
    suppliesAction_ = createAction(this, tr("Supplies"),
        tr("Supplies list"),
        SLOT(openSupplies()), tr("Alt+V"));
    staffAction_ = createAction(this, tr("Staff"), 
        tr("Staff list"),
        SLOT(openStaff()), tr("Alt+C"));
    clinicalProceduresAction_ = createAction(this, tr("Clinical Procedures"), 
        tr("Clinical procedures list"),
        SLOT(openProcedures()), tr("Alt+X"));
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
        tr("Switch study configuration"), SLOT(switchStudyConfiguration()),
        0, "hi32-switchwindowsettings.png");
    saveAction_ = createAction(this, tr("Save"),
        tr("Save study configuration"), SLOT(saveStudyConfiguration()));
    saveAsAction_ = createAction(this, tr("Save As..."), 
        tr("Save study configuration under different name"),
        SLOT(saveAsStudyConfiguration()));
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
        tr("Save window configuration"), SLOT(winSave()));
    winSaveAsAction_ = createAction(this, tr("Save As..."),
        tr("Save windows configuration as..."), SLOT(winSaveAs()));
    winSwitchAction_ = createAction(this, tr("Switch..."),
        tr("Switch windows configuration"), SLOT(winSwitch()));
    winDeleteAction_ = createAction(this, tr("Delete..."),
        tr("Delete windows configuration"), SLOT(winDelete()));
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
//     simulatorSettingsAction_ = createAction(this, tr("*Simulator Settings*"),
//         tr("Change simulator settings"), SLOT(simulatorSettings()));

    // Hardware menu -- NB No equivalent in Prucka system
    stimulatorAction_ = createAction(this, tr("Stimulator"),
        tr("Open stimulator"), SLOT(openStimulator()));
    satMonitorAction_ = createAction(this, tr("Sat Monitor"),
        tr("Open sat monitor"), SLOT(openSatMonitor()));

    // Help menu
    // in AbstractMainWindow

    // only on system toolbar
    manualSaveAction_ = createAction(this, tr("Manual Save"),
        tr("Activate manual saving of data"),
        SIGNAL(manualSave(bool)), tr("F11"), "hi32-savestudytype.png");
    manualSaveAction_->setCheckable(true);
    //connect(manualSaveAction_, SIGNAL(triggered(bool)),
     //   this, SIGNAL(manualSave(bool)));
    // explicitly connect signal/slot to indicate if "checked"
//     connect(manualSaveAction_, SIGNAL(triggered(bool checked)),
//         this, SLOT(manualSaveToggle(checked)));
    autoSaveAction_ = createAction(this, tr("Auto Save"),
        tr("Toggle Auto Save"), SIGNAL(autoSave(bool)),
         tr("Shift+F11"), "hi32-autosavetoggle.png");
    autoSaveAction_->setCheckable(true);
    //connect(autoSaveAction_, SIGNAL(triggered(bool)),
    //    this, SIGNAL(autoSave(bool)));
    emergencySaveAction_ = createAction(this, tr("Emergency Save"),
        tr("Toggle emergency save"), SIGNAL(emergencySave(bool)),
        tr("F12"));
    emergencySaveAction_->setCheckable(true);   // makes this toggle
    addAction(emergencySaveAction_); // added to Recorder, not on menu or toolbar
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
    studyConfigurationMenu_->addAction(intervalsAction());
    studyConfigurationMenu_->addAction(columnFormatsAction());
    studyConfigurationMenu_->addAction(protocolsAction());

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
    securitySubMenu_->addAction(loginAction());
    securitySubMenu_->addAction(logoutAction());
    securitySubMenu_->addAction(changePasswordAction());
    administrationMenu_->addMenu(securitySubMenu_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(systemSettingsAction());
    administrationMenu_->addAction(printSetupAction_);
    administrationMenu_->addAction(adminReportsAction_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(compressionRatioAction_);
    administrationMenu_->addAction(amplifierTestAction_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(ejectOpticalDiskAction_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(simulatorSettingsAction());

    hardwareMenu_ = menuBar()->addMenu(tr("&Hardware"));
    hardwareMenu_->addAction(stimulatorAction_);
    hardwareMenu_->addAction(satMonitorAction_);

    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(helpAction());
    helpMenu_->addAction(aboutAction());
    helpMenu_->addAction(aboutQtAction());
    
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
    protocolComboBox_ = new QComboBox;
    protocolComboBox_->setMinimumWidth(100);
    resetProtocolComboBox();
    connect(protocolComboBox_, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setProtocol(int)));
    systemToolBar->addWidget(protocolComboBox_);
  
    addToolBar(systemToolBar);
}

void Recorder::resetProtocolComboBox() {
    protocolComboBox_->clear();
    QList<Protocol> protocols = study_->studyConfiguration()->protocolList();
    QListIterator<Protocol> iter(protocols);
    while (iter.hasNext())
        protocolComboBox_->addItem(iter.next().name());
    protocolComboBox_->setCurrentIndex(study_->studyConfiguration()->
                                       currentProtocolIndex());
}

void Recorder::updateMenus() {
    simulatorSettingsAction()->setVisible(showSimulatorSettings());
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
    realTimeAction_->setEnabled(epOptions->
        filePathFlags.testFlag(Options::EnableAcquisition)  &&
        !epOptions->screenFlags.testFlag(Options::EmulateWindowsManager));
    // Tile and cascade menu items only appear if Prucka windows manager emulation is off
    tileAction_->setVisible(!epOptions->screenFlags.testFlag(Options::EmulateWindowsManager));
    cascadeAction_->setVisible(!epOptions->screenFlags.testFlag
                              (Options::EmulateWindowsManager));
}

void Recorder::saveStudyConfiguration() {
    if (!administrationAllowed())
        return;
    StudyConfigurations configList;
    configList.replace(*study_->studyConfiguration());
}

void Recorder::saveAsStudyConfiguration() {
    if (!administrationAllowed())
        return;
    QString configName = study_->studyConfiguration()->name();
    StudyConfigurations configList;
    bool ok;
    QString text =
            QInputDialog::getText(this,
                                  tr("Enter Study Configuration Name"),
                                  tr("Study configuration name:"),
                                  QLineEdit::Normal,
                                  configName, &ok);
    if (ok && !text.isEmpty()) {
        // search for duplicate study configuration name
        if (configList.isPresent(text)) {
            int result =
                QMessageBox::warning(this,
                                     tr("Duplicate Study Configuration Name"),
                                     tr("Study configuration name "
                                        "already exists.  Overwrite?"));
            if (result != QMessageBox::Ok)
                return;
            // remove study configuration with the same name
            configList.remove(text);
        }
        study_->studyConfiguration()->setName(text);
        configList.add(*study_->studyConfiguration());
    }
}


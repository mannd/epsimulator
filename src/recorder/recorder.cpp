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

#include "epfuns.h"
#include "versioninfo.h"

#include <qapplication.h>
#include <qlabel.h>
#include <q3mainwindow.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <q3popupmenu.h>
#include <qmenubar.h>
#include <QAction>
#include <qworkspace.h>		// this is a tmp central widget
//Added by qt3to4:
#include <QCloseEvent>


Recorder::Recorder(QWidget* parent, const char* name)
    : Q3MainWindow(parent, name, Qt::WDestructiveClose)
{
    workspace = new QWorkspace(this);
    setCentralWidget(workspace);

    createActions();
    createMenus();

    setCaption(tr("EP Simulator"));
    // Below not necessary, set in main.cpp by QApplication::setWindowIcon()
    // setWindowIcon(QIcon(":/hi16-app-epsimulator.png"));



    statusBar()->message(tr("EP Simulator (c) 2006 EP Studios, Inc."));

}

Recorder::~Recorder()
{
}

void Recorder::closeEvent(QCloseEvent *event) {
    // don't allow closing, as in Qt, closing a main window closes the
    // application.
    event->ignore();
    QMessageBox::information(this,
                             tr("Close Study?"),
                             tr("Please use the Close Study menu item to close"
                                " this study."));
}


void Recorder::closeStudy() {
    int ret = QMessageBox::question(this,
                                    tr("Close Study?"),
                                    tr("Select Yes to return to EP Simulator Navigator"),
                                    QMessageBox::Yes | QMessageBox::Default,
                                    QMessageBox::No | QMessageBox::Escape);
    if (ret == QMessageBox::Yes) {
            hide();     // can't close, or app will terminate
//         Navigator* navigator = new Navigator;
//         qApp->setMainWidget(navigator);
//         // Below is a work-around as showMaximized() alone doesn't always work.
//         navigator->showNormal();
//         navigator->showMaximized();
//         delete this;   
    }
}

void Recorder::help() {
    actions_.help(this);
}

 void Recorder::about() {
    actions_.about(this);
}

using EpFuns::createAction;

void Recorder::createActions()
{
    // It appears the Prucka does not have typical shortcut keys and accelerator keys
    // due to keyboard relabeling.  We'll provide some anyway.
//    patientInformationAct = new QAction(tr("Patient Information"), this);
     patientInformationAct = createAction(this, tr("Patient Information"), 
                        tr("Create and modify patient information"),
                        SLOT(patientInformation()));
    // Note that the keyboard hints are totally non-mnemotic.  Should
    // have a function that delivers the right QString, depending on whether
    // using Prucka compatible or simplified menu, or just have a separate
    // set of menus -- probably that is easier -- for each configuration
    consciousSedationAct = createAction(this, tr("Conscious Sedation"),
        tr("Conscious sedation list"), 0, tr("Alt+A"));
    complicationsAct = createAction(this, tr("Complications"), 
        tr("Complications list"), 0, tr("Alt+M"));
    radiologyAct = createAction(this, tr("Radiology"), 
        tr("Radiology list"), 0, tr("Alt+N"));
    medicationAct = createAction(this, tr("Medications"), 
        tr("Medication list"), 0, tr("Alt+B"));
    suppliesAct = createAction(this, tr("Supplies"),
        tr("Supplies list"), 0, tr("Alt+V"));
    staffAct = createAction(this, tr("Staff"), 
        tr("Staff list"), 0, tr("Alt+C"));
    clinicalProceduresAct = createAction(this, tr("Clinical Procedures"), 
        tr("Clinical procedures list"), 0, tr("Alt+X"));
    macrosAct = createAction(this, tr("Macros"), 
        tr("Macros list"), 0, tr("Alt+Z"));
    ordersAct = createAction(this, tr("Orders"), 
        tr("Enter orders"));
    reportsAct = createAction(this, tr("Reports..."), 
        tr("Create procedure reports"));
    exportDataAct = createAction(this, tr("Export Data"), 
        tr("Export data to external formats"));
    closeStudyAct = createAction(this, tr("Close Study"),
        tr("Close patient study"), SLOT(closeStudy()));
    // Study Configuration
    switchAct = createAction(this, tr("Switch..."), 
        tr("Switch study configuration"));
    saveAct = createAction(this, tr("Save"),
        tr("Save study configuration"));
    saveAsAct = createAction(this, tr("Save As..."), 
        tr("Save study configuration under different name"));
    intervalsAct = createAction(this, tr("Intervals"), 
        tr("Configure study intervals"));
    columnFormatsAct = createAction(this, tr("Column Formats"), 
        tr("Configure column formats"));
    protocolsAct = createAction(this, tr("Protocols"), 
        tr("Configure protocols"));
    // Measurements
    conductionIntervalsAct = createAction(this, tr("Conduction Intervals..."),
        tr("Measure conduction intervals"), 0, tr("Alt+Q"));
    snrtAct = createAction(this, tr("SNRT..."), 
        tr("Measure sinus node recover time"), 0, tr("Alt+W"));
    anteRPsAct = createAction(this, tr("Ante RPs..."), 
        tr("Measure anterograde refractory periods"), 0, tr("Alt+E"));
    retroRPsAct = createAction(this, tr("Retro RPs..."), 
        tr("Measure retrograde refractory periods"), 0, tr("Alt+R"));
    manualMeasurementsAct = createAction(this, tr("Manual Measurements"),
        tr("Make manual measurements"));
    statVitalsAct = createAction(this, tr("Stat Vitals"),
        tr("Record stat vital signs"));
    startStopNBPAct = createAction(this, tr("Start/Stop NBP"),
        tr("Start or stop noninvasive BP measurements"), 0, tr("F8"));
    autoLogVitalsAct = createAction(this, tr("Auto-Log Vitals"),
        tr("Auto-log vital signs"));
    autoLogVitalsAct->setToggleAction(true);
    autoLogNBPAct = createAction(this, tr("Auto-Log NBP"),
        tr("Auto-log noninvasive BP measurements"));
    autoLogNBPAct->setToggleAction(true);
    measurementConfigurationAct = createAction(this, 
        tr("Measurement Configuration"),
        tr("Configure measurements"));
    dataExtractionAct = createAction(this, tr("Data Extraction"),
        tr("Extract data"));
    // Windows Menu
    winSaveAct = createAction(this, tr("Save"),
        tr("Save window configuration"));
    winSaveAsAct = createAction(this, tr("Save As..."),
        tr("Save windows configuration as..."));
    winSwitchAct = createAction(this, tr("Switch..."),
        tr("Switch windows configuration"));
    winDeleteAct = createAction(this, tr("Delete..."),
        tr("Delete windows configuration"));
    timerAct = createAction(this, tr("Timer"),
        tr("Show timer"));
    timerAct->setToggleAction(true);
    stopwatchAct = createAction(this, tr("Stopwatch"),
        tr("Show stopwatch"));
    stopwatchAct->setToggleAction(true);
    realTimeAct = createAction(this, tr("Real-Time"),
       tr("Show real-time window"));
    realTimeAct->setToggleAction(true);
    realTimeAct->setOn(true);   // for now, default to on
    review1Act = createAction(this, tr("Review 1"),
       tr("Show review 1 window"));
    review1Act->setToggleAction(true);
    review2Act = createAction(this, tr("Review 2"),
       tr("Show review 2 window"));
    review2Act->setToggleAction(true);
    logAct = createAction(this, tr("Log"),
       tr("Show log window"));
    logAct->setToggleAction(true);
    ablationAct = createAction(this, tr("Ablation"),
       tr("Show ablation window"));
    ablationAct->setToggleAction(true);
    alignmentAct = createAction(this, tr("Alignment"),
       tr("Show alignment window"));
    alignmentAct->setToggleAction(true);
    mapAct = createAction(this, tr("Map"),
       tr("Show map window"));
    mapAct->setToggleAction(true);
    holterAct = createAction(this, tr("Holter"),
       tr("Show holter window"));
    holterAct->setToggleAction(true);
    plotAct = createAction(this, tr("Plot"),
       tr("Show plot window"));
    plotAct->setToggleAction(true);
    macroAct = createAction(this, tr("Macro"),
       tr("Show macro window"));
    macroAct->setToggleAction(true);
    image1Act = createAction(this, tr("Image 1"),
       tr("Show image 1 window"));
    image1Act->setToggleAction(true);
    image2Act = createAction(this, tr("Image 2"),
       tr("Show image 2 window"));
    image2Act->setToggleAction(true);
    imageLibraryAct = createAction(this, tr("Image Library"),
       tr("Show image library window"));
    imageLibraryAct->setToggleAction(true);
    securityAct = createAction(this, tr("Security"),
        tr("Security settings"));
    systemSettingsAct = createAction(this, tr("System Settings"),
        tr("Configure system settings"));
    printSetupAct = createAction(this, tr("Print Setup"),
        tr("Setup printer"));
    adminReportsAct = createAction(this, tr("Reports"),
        tr("Generate procedure reports"));
    compressionRatioAct = createAction(this, tr("Compression Ratio"),
        tr("Set compression ratio"));
    amplifierTestAct = createAction(this, tr("Amplifier Test..."),
        tr("Test amplifier"));
    ejectOpticalDiskAct = createAction(this, tr("Eject Optical Disk"),
        tr("Eject optical disk"));
    helpAct = createAction(this, tr("EP Simulator Help"),
        tr("EP Simulator help"), SLOT(help()));
    aboutAct = createAction(this, tr("&About EP Simulator"),
        tr("About EP Simulator"), SLOT(about()));
}

void Recorder::createMenus()
{
//    QPopupMenu *menuStudy = new QPopupMenu(this);

    menuStudy = new Q3PopupMenu(this);
    patientInformationAct->addTo(menuStudy);
    menuStudy->insertSeparator();
    consciousSedationAct->addTo(menuStudy);
    complicationsAct->addTo(menuStudy);
    radiologyAct->addTo(menuStudy);
    medicationAct->addTo(menuStudy);
    suppliesAct->addTo(menuStudy);
    staffAct->addTo(menuStudy);
    clinicalProceduresAct->addTo(menuStudy);
    macrosAct->addTo(menuStudy);
    ordersAct->addTo(menuStudy);
    menuStudy->insertSeparator();
    reportsAct->addTo(menuStudy);
    menuStudy->insertSeparator();
    exportDataAct->addTo(menuStudy);
    menuStudy->insertSeparator();
    closeStudyAct->addTo(menuStudy);

    menuStudyConfiguration = new Q3PopupMenu(this);
    switchAct->addTo(menuStudyConfiguration);
    saveAct->addTo(menuStudyConfiguration);
    saveAsAct->addTo(menuStudyConfiguration);
    menuStudyConfiguration->insertSeparator();
    intervalsAct->addTo(menuStudyConfiguration);
    columnFormatsAct->addTo(menuStudyConfiguration);
    protocolsAct->addTo(menuStudyConfiguration);

    menuMeasurements = new Q3PopupMenu(this);
    conductionIntervalsAct->addTo(menuMeasurements);
    snrtAct->addTo(menuMeasurements);
    anteRPsAct->addTo(menuMeasurements);
    retroRPsAct->addTo(menuMeasurements);
    manualMeasurementsAct->addTo(menuMeasurements);
    menuMeasurements->insertSeparator();
    statVitalsAct->addTo(menuMeasurements);
    startStopNBPAct->addTo(menuMeasurements);
    autoLogVitalsAct->addTo(menuMeasurements);
    autoLogNBPAct->addTo(menuMeasurements);
    menuMeasurements->insertSeparator();
    measurementConfigurationAct->addTo(menuMeasurements);
    menuMeasurements->insertSeparator();
    dataExtractionAct->addTo(menuMeasurements);

    menuWindows = new Q3PopupMenu(this);
    winSaveAct->addTo(menuWindows);
    winSaveAsAct->addTo(menuWindows);
    winSwitchAct->addTo(menuWindows);
    winDeleteAct->addTo(menuWindows);
    menuWindows->insertSeparator();
    timerAct->addTo(menuWindows);
    stopwatchAct->addTo(menuWindows);
    menuWindows->insertSeparator();
    realTimeAct->addTo(menuWindows);
    review1Act->addTo(menuWindows);
    review2Act->addTo(menuWindows);
    logAct->addTo(menuWindows);
    ablationAct->addTo(menuWindows);
    alignmentAct->addTo(menuWindows);
    mapAct->addTo(menuWindows);
    holterAct->addTo(menuWindows);
    plotAct->addTo(menuWindows);
    macroAct->addTo(menuWindows);
    image1Act->addTo(menuWindows);
    image2Act->addTo(menuWindows);
    imageLibraryAct->addTo(menuWindows);

    menuAdministration = new Q3PopupMenu(this);
    securityAct->addTo(menuAdministration);
    menuAdministration->insertSeparator();
    systemSettingsAct->addTo(menuAdministration);
    printSetupAct->addTo(menuAdministration);
    adminReportsAct->addTo(menuAdministration);
    menuAdministration->insertSeparator();
    compressionRatioAct->addTo(menuAdministration);
    amplifierTestAct->addTo(menuAdministration);
    menuAdministration->insertSeparator();
    ejectOpticalDiskAct->addTo(menuAdministration);

    menuHelp = new Q3PopupMenu(this);
    helpAct->addTo(menuHelp);
    aboutAct->addTo(menuHelp);
    
    menuBar()->insertItem(tr("Study"), menuStudy);
    menuBar()->insertItem(tr("Study Configuration"), menuStudyConfiguration);
    menuBar()->insertItem(tr("Measurements"), menuMeasurements);
    menuBar()->insertItem(tr("Windows"), menuWindows);
    menuBar()->insertItem(tr("Administration"), menuAdministration);
    menuBar()->insertSeparator();
    menuBar()->insertItem(tr("Help"), menuHelp);
}




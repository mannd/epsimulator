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
//#include "epfuns.h"
#include "patientdialog.h"
#include "study.h"
#include "versioninfo.h"

#include <qapplication.h>
#include <qlabel.h>
//#include <q3mainwindow.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <QMenu>
#include <QMenuBar>
//#include <qmenubar.h>
#include <QAction>
#include <QMdiArea>
//Added by qt3to4:
#include <QCloseEvent>


Recorder::Recorder(QWidget* parent)
    : QMainWindow(parent), study_(0) {
    workspace_ = new QMdiArea(this);
    setCentralWidget(workspace_);

    createActions();
    createMenus();

    setWindowTitle(tr("EP Simulator"));
    // Below not necessary, set in main.cpp by QApplication::setWindowIcon()
    // setWindowIcon(QIcon(":/hi16-app-epsimulator.png"));

    //statusBar()->message(tr("EP Simulator (c) 2006 EP Studios, Inc."));

}

Recorder::~Recorder() {
    // note study_ is deleted in closeStudy
}

void Recorder::setStudy(Study* s) {
    study_ = s;
}

void Recorder::patientInformation() {
    PatientDialog* patientDialog = new PatientDialog(this);
    patientDialog->setFields(study_);
    if (patientDialog->exec() == QDialog::Accepted) {
        patientDialog->getFields(study_);
        study_->save();
    }
    delete patientDialog;
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
        study_->save();
        // get rid of study_
        delete study_;
        if (QWidget* parentWidget = dynamic_cast<QWidget*>(parent())) {
            //parentWidget->showNormal();
            parentWidget->showMaximized();
        }
        hide();     // can't close, or app will terminate
    }
}

void Recorder::help() {
    EpGui::help(this);
}

 void Recorder::about() {
    EpGui::about(this);
}

using EpGui::createAction;

void Recorder::createActions()
{
    // It appears the Prucka does not have typical shortcut keys and accelerator keys
    // due to keyboard relabeling.  We'll provide some anyway.
//    patientInformationAct = new QAction(tr("Patient Information"), this);
     patientInformationAct_ = createAction(this, tr("Patient Information"), 
                        tr("Create and modify patient information"),
                        SLOT(patientInformation()));
    // Note that the keyboard hints are totally non-mnemotic.  Should
    // have a function that delivers the right QString, depending on whether
    // using Prucka compatible or simplified menu, or just have a separate
    // set of menus -- probably that is easier -- for each configuration
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
        tr("Close patient study"), SLOT(closeStudy()));
    // Study Configuration
    switchAct_ = createAction(this, tr("Switch..."), 
        tr("Switch study configuration"));
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
    realTimeAct_->setChecked(true);   // for now, default to on
    review1Act_ = createAction(this, tr("Review 1"),
       tr("Show review 1 window"));
    review1Act_->setCheckable(true);
    review2Act_ = createAction(this, tr("Review 2"),
       tr("Show review 2 window"));
    review2Act_->setCheckable(true);
    logAct_ = createAction(this, tr("Log"),
       tr("Show log window"));
    logAct_->setCheckable(true);
    ablationAct_ = createAction(this, tr("Ablation"),
       tr("Show ablation window"));
    ablationAct_->setCheckable(true);
    alignmentAct_ = createAction(this, tr("Alignment"),
       tr("Show alignment window"));
    alignmentAct_->setCheckable(true);
    mapAct_ = createAction(this, tr("Map"),
       tr("Show map window"));
    mapAct_->setCheckable(true);
    holterAct_ = createAction(this, tr("Holter"),
       tr("Show holter window"));
    holterAct_->setCheckable(true);
    plotAct_ = createAction(this, tr("Plot"),
       tr("Show plot window"));
    plotAct_->setCheckable(true);
    macroAct_ = createAction(this, tr("Macro"),
       tr("Show macro window"));
    macroAct_->setCheckable(true);
    image1Act_ = createAction(this, tr("Image 1"),
       tr("Show image 1 window"));
    image1Act_->setCheckable(true);
    image2Act_ = createAction(this, tr("Image 2"),
       tr("Show image 2 window"));
    image2Act_->setCheckable(true);
    imageLibraryAct_ = createAction(this, tr("Image Library"),
       tr("Show image library window"));
    imageLibraryAct_->setCheckable(true);
    securityAct_ = createAction(this, tr("Security"),
        tr("Security settings"));
    systemSettingsAct_ = createAction(this, tr("System Settings"),
        tr("Configure system settings"));
    printSetupAct_ = createAction(this, tr("Print Setup"),
        tr("Setup printer"));
    adminReportsAct_ = createAction(this, tr("Reports"),
        tr("Generate procedure reports"));
    compressionRatioAct_ = createAction(this, tr("Compression Ratio"),
        tr("Set compression ratio"));
    amplifierTestAct_ = createAction(this, tr("Amplifier Test..."),
        tr("Test amplifier"));
    ejectOpticalDiskAct_ = createAction(this, tr("Eject Optical Disk"),
        tr("Eject optical disk"));
    helpAct_ = createAction(this, tr("EP Simulator Help"),
        tr("EP Simulator help"), SLOT(help()));
    aboutAct_ = createAction(this, tr("&About EP Simulator"),
        tr("About EP Simulator"), SLOT(about()));
}

void Recorder::createMenus()
{
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

    administrationMenu_ = menuBar()->addMenu(tr("&Administration"));
    administrationMenu_->addAction(securityAct_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(systemSettingsAct_);
    administrationMenu_->addAction(printSetupAct_);
    administrationMenu_->addAction(adminReportsAct_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(compressionRatioAct_);
    administrationMenu_->addAction(amplifierTestAct_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(ejectOpticalDiskAct_);

    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(helpAct_);
    helpMenu_->addAction(aboutAct_);
}



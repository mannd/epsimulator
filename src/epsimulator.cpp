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

#include <qlabel.h>
#include <qmessagebox.h>

#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qworkspace.h>		// this is a tmp central widget

#include "epsimulator.h"
#include "patientinformationdialog.h"

Epsimulator::Epsimulator()
    : QMainWindow( 0, "epsimulator", WDestructiveClose )
{
    workspace = new QWorkspace(this);
    setCentralWidget(workspace);

    createActions();
    createMenus();

    setCaption(tr("EP Simulator"));
    setIcon(QPixmap::fromMimeSource("hi16-app-epsimulator.png"));



    statusBar()->message(tr("EP Simulator (c) 2006 EP Studios, Inc."));

}

Epsimulator::~Epsimulator()
{
}


void Epsimulator::closeStudy()
{
}

void Epsimulator::about(QWidget *parent)
{
    QMessageBox::about(parent, tr("About EP Simulator"),
		       tr("<h2>EP Simulator 0.1</h2>"
		          "<p>Copyright &copy; 2006 EP Studios, Inc."
			  "<p>EP Simulator simulates an EP recording "
			  "system."
                          "<p><a href=www.epstudiossoftware.com> www.epstudiossoftware.com</a>"));
///TODO make this an actual hyperlink that you can click on and go to
}

void Epsimulator::createActions()
{
    // It appears the Prucka does not have typical shortcut keys and accelerator keys
    // due to keyboard relabeling.  We'll provide some anyway.
    patientInformationAct = new QAction(tr("Patient Information"), 0 , this);
    patientInformationAct->setStatusTip(tr("Create and modify patient information"));
    connect(patientInformationAct, SIGNAL(activated()), this, SLOT(patientInformation()));

    // Note that the keyboard hints are totally non-mnemotic.  Should
    // have a function that delivers the right QString, depending on whether
    // using Prucka compatible or simplified menu, or just have a separate
    // set of menus -- probably that is easier -- for each configuration
    consciousSedationAct = new QAction(tr("Conscious Sedation"), tr("Alt+A"), this);
    consciousSedationAct->setStatusTip(tr("Conscious sedation list"));
    //connect signal slot here

    complicationsAct = new QAction(tr("Complications"), tr("Alt+M"), this);
    complicationsAct->setStatusTip(tr("Complications list"));

    radiologyAct = new QAction(tr("Radiology"), tr("Alt+N"), this);
    radiologyAct->setStatusTip(tr("Radiology list"));

    medicationAct = new QAction(tr("Medications"), tr("Alt+B"), this);
    medicationAct->setStatusTip(tr("Medication list"));

    suppliesAct = new QAction(tr("Supplies"), tr("Alt+V"), this);
    suppliesAct->setStatusTip(tr("Supplies list"));

    staffAct = new QAction(tr("Staff"), tr("Alt+C"), this);
    staffAct->setStatusTip(tr("Staff list"));

    clinicalProceduresAct = new QAction(tr("Clinical Procedures"), tr("Alt+X"), this);
    clinicalProceduresAct->setStatusTip(tr("Clinical procedures list"));

    macrosAct = new QAction(tr("Macros"), tr("Alt+Z"), this);
    macrosAct->setStatusTip(tr("Macros list"));    

    ordersAct = new QAction(tr("Orders"), 0, this);
    ordersAct->setStatusTip(tr("Enter orders"));

    reportsAct = new QAction(tr("Reports..."), 0, this);
    reportsAct->setStatusTip(tr("Create procedure reports"));

    exportDataAct = new QAction(tr("Export Data"), 0, this);
    exportDataAct->setStatusTip(tr("Export data to external formats"));

    closeStudyAct = new QAction(tr("Close Study"), 0, this);
    closeStudyAct->setStatusTip(tr("Close patient study"));
    connect(closeStudyAct, SIGNAL(activated()), this, SLOT(closeStudy()));


    // Study Configuration
    switchAct = new QAction(tr("Switch..."), 0, this);
    switchAct->setStatusTip(tr("Switch study configuration"));

    saveAct = new QAction(tr("Save"), 0, this);
    saveAct->setStatusTip(tr("Save study configuration"));

    saveAsAct = new QAction(tr("Save As..."), 0, this);
    saveAsAct->setStatusTip(tr("Save study configuration under different name"));

    intervalsAct = new QAction(tr("Intervals"), 0, this);
    intervalsAct->setStatusTip(tr("Configure study intervals"));

    columnFormatsAct = new QAction(tr("Column Formats"), 0, this);
    columnFormatsAct->setStatusTip(tr("Configure column formats"));

    protocolsAct = new QAction(tr("Protocols"), 0, this);
    protocolsAct->setStatusTip(tr("Configure protocols"));

    // Measurements
    conductionIntervalsAct = new QAction(tr("Conduction Intervals..."), tr("Alt+Q"), this);
    conductionIntervalsAct->setStatusTip(tr("Measure conduction intervals"));
   
    snrtAct = new QAction(tr("SNRT..."), tr("Alt+W"), this);
    snrtAct->setStatusTip(tr("Measure sinus node recovery time"));

    anteRPsAct = new QAction(tr("Ante RPs..."), tr("Alt+E"), this);
    anteRPsAct->setStatusTip(tr("Measure anterograde refractory periods"));

    retroRPsAct = new QAction(tr("Retro RPs..."), tr("Alt+R"), this);
    retroRPsAct->setStatusTip(tr("Measure retrograde refractory periods"));

    manualMeasurementsAct = new QAction(tr("Manual Measurements"), 0, this);
    manualMeasurementsAct->setStatusTip(tr("Make manual measurements"));

    statVitalsAct = new QAction(tr("Stat Vitals"), 0, this);
    statVitalsAct->setStatusTip(tr("Record stat vital signs"));

    startStopNBPAct = new QAction(tr("Start/Stop NBP"), tr("F8"), this);
    startStopNBPAct->setStatusTip(tr("Start or stop noninvasive BP measurements"));

    autoLogVitalsAct = new QAction(tr("Auto-Log Vitals"), 0, this);
    autoLogVitalsAct->setToggleAction(true);
    autoLogVitalsAct->setStatusTip(tr("Auto-log vital signs"));

    autoLogNBPAct = new QAction(tr("Auto-Log NBP"), 0, this);
    autoLogNBPAct->setToggleAction(true);
    autoLogNBPAct->setStatusTip(tr("Auto-log noninvasive BP measurements"));

    measurementConfigurationAct = new QAction(tr("Measurement Configuration"), 0, this);
    measurementConfigurationAct->setStatusTip(tr("Configure measurements"));

    dataExtractionAct = new QAction(tr("Data Extraction"), 0, this);
    dataExtractionAct->setStatusTip(tr("Extract data"));

    // Windows Menu
    winSaveAct = new QAction(tr("Save"), 0, this);
    winSaveAct->setStatusTip(tr("Save window configuration"));

    winSaveAsAct = new QAction(tr("Save As..."), 0, this);
    winSaveAct->setStatusTip(tr("Save windows configuration as..."));

    winSwitchAct = new QAction(tr("Switch..."), 0, this);
    winSwitchAct->setStatusTip(tr("Switch windows configuration"));

    winDeleteAct = new QAction(tr("Delete..."), 0, this);
    winDeleteAct->setStatusTip(tr("Delete windows configuration"));

    timerAct = new QAction(tr("Timer"), 0, this);
    timerAct->setToggleAction(true);
    timerAct->setStatusTip(tr("Show timer"));

    stopwatchAct = new QAction(tr("Stopwatch"), 0, this);
    stopwatchAct->setToggleAction(true);
    stopwatchAct->setStatusTip(tr("Show stopwatch"));

    realTimeAct = new QAction(tr("Real-Time"), 0, this);
    realTimeAct->setToggleAction(true);
    realTimeAct->setOn(true);   // for now, default to on
    realTimeAct->setStatusTip(tr("Show real-time window"));

    review1Act = new QAction(tr("Review 1"), 0, this);
    review1Act->setToggleAction(true);
    review1Act->setStatusTip(tr("Show review 1 window"));

    review2Act = new QAction(tr("Review 2"), 0, this);
    review2Act->setToggleAction(true);
    review2Act->setStatusTip(tr("Show review 2 window"));

    logAct = new QAction(tr("Log"), 0, this);
    logAct->setToggleAction(true);
    logAct->setStatusTip(tr("Show log window"));

    ablationAct = new QAction(tr("Ablation"), 0, this);
    ablationAct->setToggleAction(true);
    ablationAct->setStatusTip(tr("Show ablation window"));

    alignmentAct = new QAction(tr("Alignment"), 0, this);
    alignmentAct->setToggleAction(true);
    alignmentAct->setStatusTip(tr("Show alignment window"));

    mapAct = new QAction(tr("Map"), 0, this);
    mapAct->setToggleAction(true);
    mapAct->setStatusTip(tr("Show map window"));

    holterAct = new QAction(tr("Holter"), 0, this);
    holterAct->setToggleAction(true);
    holterAct->setStatusTip(tr("Show holter window"));

    plotAct = new QAction(tr("Plot"), 0, this);
    plotAct->setToggleAction(true);
    plotAct->setStatusTip(tr("Show plot window"));

    macroAct = new QAction(tr("Macro"), 0, this);
    macroAct->setToggleAction(true);
    macroAct->setStatusTip(tr("Show macro window"));

    image1Act = new QAction(tr("Image 1"), 0, this);
    image1Act->setToggleAction(true);
    image1Act->setStatusTip(tr("Show image 1 window"));

    image2Act = new QAction(tr("Image 2"), 0, this);
    image2Act->setToggleAction(true);
    image2Act->setStatusTip(tr("Show image 2 window"));

    imageLibraryAct = new QAction(tr("Image Library"), 0, this);
    imageLibraryAct->setToggleAction(true);
    imageLibraryAct->setStatusTip(tr("Show image library window"));

    securityAct = new QAction(tr("Security"), 0, this);
    securityAct->setStatusTip(tr("Security settings"));
    
    systemSettingsAct = new QAction(tr("System Settings"), 0, this);
    systemSettingsAct->setStatusTip(tr("Configure system settings"));
    
    printSetupAct = new QAction(tr("Print Setup"), 0, this);
    printSetupAct->setStatusTip(tr("Setup printer"));

    adminReportsAct = new QAction(tr("Reports"), 0, this);
    adminReportsAct->setStatusTip(tr("Generate procedure reports"));
    
    compressionRatioAct = new QAction(tr("Compression Ratio"), 0, this);
    compressionRatioAct->setStatusTip(tr("Set compression ratio"));

    amplifierTestAct = new QAction(tr("Amplifier Test..."), 0, this);
    amplifierTestAct->setStatusTip(tr("Test amplifier"));

    ejectOpticalDiskAct = new QAction(tr("Eject Optical Disk"), 0, this);
    ejectOpticalDiskAct->setStatusTip(tr("Eject optical disk"));
    
    
    
    

    aboutAct = new QAction(tr("&About EP Simulator"), 0, this);
    aboutAct->setStatusTip(tr("About EP Simulator"));
    connect(aboutAct, SIGNAL(activated()), this, SLOT(about(this)));
}

void Epsimulator::createMenus()
{
//    QPopupMenu *menuStudy = new QPopupMenu(this);

    menuStudy = new QPopupMenu(this);
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

    menuStudyConfiguration = new QPopupMenu(this);
    switchAct->addTo(menuStudyConfiguration);
    saveAct->addTo(menuStudyConfiguration);
    saveAsAct->addTo(menuStudyConfiguration);
    menuStudyConfiguration->insertSeparator();
    intervalsAct->addTo(menuStudyConfiguration);
    columnFormatsAct->addTo(menuStudyConfiguration);
    protocolsAct->addTo(menuStudyConfiguration);

    menuMeasurements = new QPopupMenu(this);
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

    menuWindows = new QPopupMenu(this);
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

    menuAdministration = new QPopupMenu(this);
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

    menuHelp = new QPopupMenu(this);
    aboutAct->addTo(menuHelp);
    
    menuBar()->insertItem(tr("Study"), menuStudy);
    menuBar()->insertItem(tr("Study Configuration"), menuStudyConfiguration);
    menuBar()->insertItem(tr("Measurements"), menuMeasurements);
    menuBar()->insertItem(tr("Windows"), menuWindows);
    menuBar()->insertItem(tr("Administration"), menuAdministration);
    menuBar()->insertSeparator();
    menuBar()->insertItem(tr("Help"), menuHelp);
}




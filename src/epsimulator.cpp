/***************************************************************************
 *   Copyright (C) 2006 by David Mann   *
 *   mannd@epstudiossoftware.com   *
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


#include "epsimulator.h"
#include "patientinformationdialog.h"

#include <qlabel.h>
#include <qmessagebox.h>

#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qtable.h>		// this is a tmp central widget

Epsimulator::Epsimulator()
    : QMainWindow( 0, "epsimulator", WDestructiveClose )
{
    qtable = new QTable(this);
    setCentralWidget(qtable);

    createActions();
    createMenus();

    setCaption(tr("EP Simulator"));



    statusBar()->message(tr("EP Simulator has started."));

}

Epsimulator::~Epsimulator()
{
}

void Epsimulator::patientInformation()
{
    PatientInformationDialog *patientInformationDialog = new PatientInformationDialog;
    patientInformationDialog->show();
}

void Epsimulator::closeStudy()
{
}

void Epsimulator::about()
{
    QMessageBox::about(this, tr("About EP Simulator"),
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

    exitAct = new QAction(tr("Exit"), 0, this);
    exitAct->setStatusTip(tr("Exit EP Simulator"));
    connect(exitAct, SIGNAL(activated()), this, SLOT(close()));

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

    startStopNBPAct = new QAction(tr("Start/Stop NBP"), 0, this);
    startStopNBPAct->setStatusTip(tr("Start or stop noninvasive BP measurements"));

    autoLogVitalsAct = new QAction(tr("Auto-Log Vitals"), 0, this);
    autoLogVitalsAct->setStatusTip(tr("Auto-log vital signs"));

    autoLogNBPAct = new QAction(tr("Auto-Log NBP"), 0, this);
    autoLogNBPAct->setStatusTip(tr("Auto-log noninvasive BP measurements"));

    measurementConfigurationAct = new QAction(tr("Measurement Configuration"), 0, this);
    measurementConfigurationAct->setStatusTip(tr("Configure measurements"));

    dataExtractionAct = new QAction(tr("Data Extraction"), 0, this);
    dataExtractionAct->setStatusTip(tr("Extract data"));


    aboutAct = new QAction(tr("&About EP Simulator"), 0, this);
    aboutAct->setStatusTip(tr("About EP Simulator"));
    connect(aboutAct, SIGNAL(activated()), this, SLOT(about()));
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
    exitAct->addTo(menuStudy);

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
    menuAdministration = new QPopupMenu(this);
    menuHelp = new QPopupMenu(this);
    aboutAct->addTo(menuHelp);
    
    menuBar()->insertItem(tr("&Study"), menuStudy);
    menuBar()->insertItem(tr("Study &Configuration"), menuStudyConfiguration);
    menuBar()->insertItem(tr("&Measurements"), menuMeasurements);
    menuBar()->insertItem(tr("&Windows"), menuWindows);
    menuBar()->insertItem(tr("&Administration"), menuAdministration);
    menuBar()->insertSeparator();
    menuBar()->insertItem(tr("&Help"), menuHelp);
}

void Epsimulator::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::question(
            this,
            tr("Really quit?"),
            tr("Quit EP Simulator?"),
            QMessageBox::Yes | QMessageBox::Default,
            QMessageBox::No,
            QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}


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


#ifndef RECORDER_H
#define RECORDER_H

#include "patient.h"

#include <QMainWindow>

class PatientStatusBar;
class QAction;
class QCloseEvent;
class QComboBox;
class QDockWidget;
class QMdiArea;
class QMenu;
class Study;

class Recorder : public QMainWindow {
    Q_OBJECT

public:
    Recorder(QWidget* parent = 0);
    
    void setStudy(Study*);

    ~Recorder();

protected:
    void closeEvent(QCloseEvent * event);
    //    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    void patientInformation();
    void about();
    void help(); 
    void closeStudy();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createPatientStatusBar();
    void updateWindowTitle();
    void readSettings();
    void saveSettings();

    Study* study_;
    Patient patient_;

    // central widget
    QMdiArea* workspace_;
    // patient status bar
    PatientStatusBar* patientStatusBar_;
    
    

    // Study Menu
    QAction* patientInformationAct_;
    QAction* consciousSedationAct_;
    QAction* complicationsAct_;
    QAction* radiologyAct_;
    QAction* medicationAct_;
    QAction* suppliesAct_;
    QAction* staffAct_;
    QAction* clinicalProceduresAct_;
    QAction* macrosAct_;
    QAction* ordersAct_;
    QAction* reportsAct_;
    QAction* exportDataAct_;
    QAction* closeStudyAct_;

    // Study Configuration
    QAction* switchAct_;
    QAction* saveAct_;
    QAction* saveAsAct_;
    QAction* intervalsAct_;
    QAction* columnFormatsAct_;
    QAction* protocolsAct_;

    // Measurements
    QAction* conductionIntervalsAct_;
    QAction* snrtAct_;
    QAction* anteRPsAct_;
    QAction* retroRPsAct_;
    QAction* manualMeasurementsAct_;
    QAction* statVitalsAct_;
    QAction* startStopNBPAct_;
    QAction* autoLogVitalsAct_;
    QAction* autoLogNBPAct_;
    QAction* measurementConfigurationAct_;
    QAction* dataExtractionAct_;

    // Windows
    QAction* winSaveAct_;   // note that Study Config menu has duplicate names
    QAction* winSaveAsAct_;
    QAction* winSwitchAct_;
    QAction* winDeleteAct_;
    QAction* timerAct_;
    QAction* stopwatchAct_;
    QAction* realTimeAct_;
    QAction* review1Act_;
    QAction* review2Act_;
    QAction* logAct_;
    QAction* ablationAct_;
    QAction* alignmentAct_;
    QAction* mapAct_;
    QAction* holterAct_;
    QAction* plotAct_;
    QAction* macroAct_;
    QAction* image1Act_;
    QAction* image2Act_;
    QAction* imageLibraryAct_;

    // Administration Menu
    QAction* securityAct_;
    QAction* systemSettingsAct_;
    QAction* printSetupAct_;
    QAction* adminReportsAct_;   // there is a Report action in the Study menu too
    QAction* compressionRatioAct_;
    QAction* amplifierTestAct_;
    QAction* ejectOpticalDiskAct_;
    
    QAction* helpAct_;
    QAction* aboutAct_;

    // System Toolbar
    QAction* autoSaveAct_;
    QComboBox* switchedVideoComboBox_;
    QComboBox* protocolComboBox_;

    // Main Menu Bar
    QMenu* studyMenu_;
    QMenu* studyConfigurationMenu_;
    QMenu* measurementsMenu_;
    QMenu* windowsMenu_;
    QMenu* administrationMenu_;
    QMenu* helpMenu_;
};

#endif

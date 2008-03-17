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
#include "recorderdefs.h"

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>

class DisplayWindow;
class LogWindow;
class OpticalDisk;
class Options;
class PatientStatusBar;
class QAction;
class QCloseEvent;
class QComboBox;
class QDockWidget;
class QMenu;
class QSplitter;
class RealTimeWindow;
class ReviewWindow;
class Review2Window;
class SatMonitor;
class Settings;
class Study;
class User;

namespace EpRecorder {


class Recorder : public QMainWindow {
    Q_OBJECT
public:
    Recorder(QWidget* parent, 
        Study* study, OpticalDisk* currentDisk);
    
    void updateAll();

    ~Recorder();

public slots:
//    void manualSave();  // starts manual saving, depresses Save button

protected:
    void closeEvent(QCloseEvent * event);
    //    void contextMenuEvent(QContextMenuEvent * event);

signals:
    void manualSave(bool);  // emitted if Save toolbar button changed
    void autoSave(bool);    // emitted if AutoSave toolbar button changed
    void updateSimulatorSettings(); // emitted after simulator settings updated

private slots:
    void patientInformation();
    //void autoSaveToggle(bool on);
    void login();
    void logout();
    void changePassword();
    void about();
    void help(); 
    void systemSettings();
    void simulatorSettings();
    void openStimulator();
    void openSatMonitor();
    void setManualSave(bool);
    void updateMenus();

    void realTimeWindowOpen(bool);
    void review1WindowOpen(bool);
    void review2WindowOpen(bool);
    void logWindowOpen(bool);
    void tileSubWindows();
    void cascadeSubWindows();

private:
 
    std::vector<bool> openDisplayWindowList_;

    void updateOpenDisplayWindowList();  
    void restoreDisplayWindow(const QString& key, 
        const Settings& settings, QMdiSubWindow*, DisplayWindow*);

    void createActions();
    void createCentralWidget();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createPatientStatusBar();
    void updateWindowTitle();
    void readSettings();
    void readSettings(const Settings&);
    void writeSettings();
    void writeSettings(Settings&);
    bool administrationAllowed();
    bool closeStudy();
    
    bool subWindowIsOpen(QMdiSubWindow*);

    template<typename T>
    void openSubWindow(bool, QMdiSubWindow*&, T*&, int number = 0);

    Study* study_;
    Patient* patient_;
    User* user_;
    Options* options_;

    // hardware
    OpticalDisk* currentDisk_;

    // central widget
    QMdiArea* centralWidget_;
    RealTimeWindow* realTimeWindow_;
    ReviewWindow* review1Window_;
    ReviewWindow* review2Window_;
    LogWindow* logWindow_;
    QMdiSubWindow* realTimeSubWindow_;
    QMdiSubWindow* review1SubWindow_;
    QMdiSubWindow* review2SubWindow_;
    QMdiSubWindow* logSubWindow_;

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
    QAction* tileAct_;
    QAction* cascadeAct_;

    // Administration Menu
    QAction* loginAct_;
    QAction* logoutAct_;
    QAction* changePasswordAct_;
    QAction* systemSettingsAct_;
    QAction* printSetupAct_;
    QAction* adminReportsAct_;   // there is a Report action in the Study menu too
    QAction* compressionRatioAct_;
    QAction* amplifierTestAct_;
    QAction* ejectOpticalDiskAct_;
    QAction* simulatorSettingsAct_;

    // Hardware Menu
    QAction* stimulatorAct_;
    QAction* satMonitorAct_;
    
    // Help Menu
    QAction* helpAct_;
    QAction* aboutAct_;

    // System Toolbar
    QAction* manualSaveAct_;
    QAction* autoSaveAct_;
    QComboBox* switchedVideoComboBox_;
    QComboBox* protocolComboBox_;

    // Main Menu Bar
    QMenu* studyMenu_;
    QMenu* studyConfigurationMenu_;
    QMenu* measurementsMenu_;
    QMenu* windowsMenu_;
    QMenu* administrationMenu_;
    QMenu* securitySubMenu_;
    QMenu* hardwareMenu_;
    QMenu* helpMenu_;
};

/**
 * Opens a QMdiSubWindow with specific internal widget of type T.
 * This needs to be a template, because even though all DisplayWindows
 * are derived from DisplayWindow, specific subclasses must be instantiated.
 * An alternative here might be a factory method determined by the
 * DisplayWindowType enum.
 * @param open Open or close (remove) the subWindow
 * @param subWindow pointer to subWindow being manipulated
 * @param internalWidget pointer to a specific type of DisplayWindow
 * @param number window number, 0 if none, 1 or 2 (e.g. review1Window)
 */
template<typename T> 
void Recorder::openSubWindow(bool open, QMdiSubWindow*& subWindow, 
    T*& internalWidget, int number) {
    bool alreadyOpen = subWindowIsOpen(subWindow);
    if (open && alreadyOpen)
        return;
    if (!open && !alreadyOpen)
        return;
    if (open) {
        internalWidget = new T(number);
        subWindow = centralWidget_->addSubWindow(internalWidget);
        subWindow->show();
    }
    else {
        centralWidget_->removeSubWindow(subWindow);
        subWindow->close();
        internalWidget = 0;
    }
    updateMenus();
}

}

#endif

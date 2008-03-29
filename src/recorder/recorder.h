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
    void updateSettings();

    void realTimeWindowOpen(bool);
    void review1WindowOpen(bool);
    void review2WindowOpen(bool);
    void logWindowOpen(bool);
    void tileSubWindows();
    void cascadeSubWindows();

private:
 
    std::vector<bool> openDisplayWindowList_;

    void updateOpenDisplayWindowList();  
    void connectReviewWindows();
    
    void restoreDisplayWindow(const QString& key,
        Settings& settings, const QString& currentWindowKey,
        QMdiSubWindow*, DisplayWindow*, QMdiSubWindow*& currentSubWindow);

    void createActions();
    void createCentralWidget();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createPatientStatusBar();
    void updateWindowTitle();
    void readSettings();
    void readSettings(Settings&);
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
    QAction* patientInformationAction_;
    QAction* consciousSedationAction_;
    QAction* complicationsAction_;
    QAction* radiologyAction_;
    QAction* medicationAction_;
    QAction* suppliesAction_;
    QAction* staffAction_;
    QAction* clinicalProceduresAction_;
    QAction* macrosAction_;
    QAction* ordersAction_;
    QAction* reportsAction_;
    QAction* exportDataAction_;
    QAction* closeStudyAction_;

    // Study Configuration
    QAction* switchAction_;
    QAction* saveAction_;
    QAction* saveAsAction_;
    QAction* intervalsAction_;
    QAction* columnFormatsAction_;
    QAction* protocolsAction_;

    // Measurements
    QAction* conductionIntervalsAction_;
    QAction* snrtAction_;
    QAction* anteRPsAction_;
    QAction* retroRPsAction_;
    QAction* manualMeasurementsAction_;
    QAction* statVitalsAction_;
    QAction* startStopNBPAction_;
    QAction* autoLogVitalsAction_;
    QAction* autoLogNBPAction_;
    QAction* measurementConfigurationAction_;
    QAction* dataExtractionAction_;

    // Windows
    QAction* winSaveAction_;   // note that Study Config menu has duplicate names
    QAction* winSaveAsAction_;
    QAction* winSwitchAction_;
    QAction* winDeleteAction_;
    QAction* timerAction_;
    QAction* stopwatchAction_;
    QAction* realTimeAction_;
    QAction* review1Action_;
    QAction* review2Action_;
    QAction* logAction_;
    QAction* ablationAction_;
    QAction* alignmentAction_;
    QAction* mapAction_;
    QAction* holterAction_;
    QAction* plotAction_;
    QAction* macroAction_;
    QAction* image1Action_;
    QAction* image2Action_;
    QAction* imageLibraryAction_;
    QAction* tileAction_;
    QAction* cascadeAction_;

    // Administration Menu
    QAction* loginAction_;
    QAction* logoutAction_;
    QAction* changePasswordAction_;
    QAction* systemSettingsAction_;
    QAction* printSetupAction_;
    QAction* adminReportsAction_;   // there is a Report action in the Study menu too
    QAction* compressionRatioAction_;
    QAction* amplifierTestAction_;
    QAction* ejectOpticalDiskAction_;
    QAction* simulatorSettingsAction_;

    // Hardware Menu
    QAction* stimulatorAction_;
    QAction* satMonitorAction_;
    
    // Help Menu
    QAction* helpAction_;
    QAction* aboutAction_;

    // System Toolbar
    QAction* manualSaveAction_;
    QAction* autoSaveAction_;
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

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

#include "abstractmainwindow.h"
#include "options.h"
#include "recorderdefs.h"
#include "studyinformationdialog.h"

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>

class QAction;
class QCloseEvent;
class QComboBox;
class QDockWidget;
class QSettings;

namespace EpCore {
    class User;
}

namespace EpHardware { 
    namespace EpOpticalDisk {
        class OpticalDisk;
    }
    namespace EpAmplifier {
        class Amplifier;
    }
    class SatMonitor;
}

namespace EpPatient {
    class Patient;
}

namespace EpStudy {
    class Study;
    class StudyConfiguration;
}

namespace EpRecorder {

class DisplayWindow;
class LogWindow;
class PatientStatusBar;
class RealTimeWindow;
class ReviewWindow;
class Review2Window;

class Recorder : public EpGui::AbstractMainWindow {
    Q_OBJECT
public:
    Recorder(QWidget* parent, EpStudy::Study* study,
             EpHardware::EpOpticalDisk::OpticalDisk* currentDisk,
             EpCore::User* user,
             EpCore::Options* options,
             bool allowAcquisition = true,
             RecorderWindow = Primary);
    ~Recorder();

    RecorderWindow recorderWindow() const {return recorderWindow_;}
    
    void setupInitialScreen(bool tile = false);  // the default screen setup
    void updateAll();

public slots:
    void changeOpticalDisk(EpHardware::EpOpticalDisk::OpticalDisk* disk) {
        currentDisk_ = disk;}

protected:
    // these are definitions of abstract virtual functions in EpGui::AbstractMainWindow
    virtual EpCore::User* user() const {return user_;}
    virtual EpHardware::EpOpticalDisk::OpticalDisk* currentDisk() const {return currentDisk_;}

    bool eventFilter(QObject*, QEvent*);
    void closeEvent(QCloseEvent*);
    void resizeEvent(QResizeEvent*);
    virtual EpCore::Options* options() const {return options_;}

signals:
    void manualSave(bool);  // emitted if Save toolbar button changed
    void autoSave(bool);    // emitted if AutoSave toolbar button changed
    void emergencySave(bool);   // emitted if Emergency Save key activated
    void patientInformationClosed(); // emitted when Patient Info Dialog closed
    void studyDataChanged(const EpStudy::Study*);

    // these signals let Navigator and any other Recorder windows know
    // that important settings have changed.
    void simulatorSettingsChanged(); // emitted after simulator settings changed
    void systemSettingsChanged();   // emitted after system settings changed
    void displayWindowResized(QWidget*); // emitted after
                                         // display windows resized

private slots:
    // these are redefinitions of abstract virtual functions
    // in EpGui::AbstractMainWindow.
    virtual void updateSimulatorSettings();
    virtual void updateSystemSettings();

    void saveStudyConfiguration();
    void saveAsStudyConfiguration();

    void patientInformation();
    void openConsciousSedation();
    void openProcedures();
    void openStaff();
    void openSupplies();
    void openMedications();
    void openRadiology();
    void openOrders();
    void openComplications();
    void openContrast();
    void switchStudyConfiguration();
    void winSave();
    void winSaveAs();
    void winSwitch();
    void winDelete();
    void openStimulator();
    void openSatMonitor();
    void setManualSave(bool);
    void setEmergencySave(bool);
    void updateMenus();
    void setProtocol(int);
    void resetProtocolComboBox();

    void realTimeWindowOpen(bool);
    void review1WindowOpen(bool);
    void review2WindowOpen(bool);
    void logWindowOpen(bool);
    void tileSubWindows();
    void cascadeSubWindows();
    void resizeDisplayWindows(QWidget*);

private:
    Recorder(const Recorder&);
    Recorder& operator=(const Recorder&);

    static const int edgeWidth = 80; // width of edges that are ignored by mouse
    bool noMansZone(const QPoint& p);
 
    void loadPatient();
    void loadAmplifier();
    void loadStudyConfiguration();

    void updateOpenDisplayWindowList();  
    void connectReviewWindows();
    
    void restoreDisplayWindow(const QString& key,
        QSettings& settings, const QString& currentWindowKey,
        QMdiSubWindow*, DisplayWindow*, QMdiSubWindow*& currentSubWindow);

    void openStudyInformation(EpStudy::StudyInformationDialog::List);

    void arrangeSubWindows();

    void createActions();
    void createCentralWidget();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createPatientStatusBar();
    virtual void readSettings();
    void writeSettings();
    bool closeStudy();
    
    bool subWindowIsOpen(QMdiSubWindow*);

    template<typename T>
    void openSubWindow(bool, QMdiSubWindow*&,
                       T*&, 
		       int number = 0);

    std::vector<bool> openDisplayWindowList_;
    EpStudy::Study* study_;
    EpStudy::StudyConfiguration* studyConfig_;
    EpPatient::Patient* patient_;
    EpCore::User* user_;            // owned by Navigator
    EpCore::Options* options_;      // owned by Navigator

    // hardware
    EpHardware::EpOpticalDisk::OpticalDisk* currentDisk_;
    EpHardware::EpAmplifier::Amplifier* amplifier_;

    // types of Recorder window
    bool allowAcquisition_;
    RecorderWindow recorderWindow_;

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
    QAction* emergencySaveAction_;

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
    QAction* winSaveAction_;   // note Study Config menu has duplicate names
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
    QAction* printSetupAction_;
    QAction* adminReportsAction_;   // Report action has a Study menu too
    QAction* compressionRatioAction_;
    QAction* amplifierTestAction_;
    QAction* ejectOpticalDiskAction_;

    // Hardware Menu
    QAction* stimulatorAction_;
    QAction* satMonitorAction_;
    
    // Help Menu
    // Actions are in EpGui::AbstractMainWindow

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
        internalWidget = new T(study_, options_, number);
        subWindow = centralWidget_->addSubWindow(internalWidget, 
            Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
        QMenu* systemMenu = new QMenu(subWindow);
        QAction* closeAct = new QAction(tr("Close"), subWindow);
        closeAct->setShortcut(tr("Ctrl+W"));
        connect(closeAct, SIGNAL(triggered()), subWindow, SLOT(close()));
        systemMenu->addAction(closeAct);
        subWindow->setSystemMenu(systemMenu);
        subWindow->setOption(QMdiSubWindow::RubberBandResize, options_->
            screenFlags.testFlag(EpCore::Options::EmulateWindowsManager));
        subWindow->installEventFilter(this);
        subWindow->setMouseTracking(true);
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

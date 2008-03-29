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

/** 
 *  @file header file for the Navigator classs,
 *  which is the main window for the program.
 */

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QMainWindow>

class AbstractButtonFrame;
class Catalog;
class Catalogs;
class CatalogComboBox;
class FilterCatalogDialog;
class MoveCopyStudyDialog;
class OpticalDisk;
class Options;
class QAction;
class QCloseEvent;
class QMenu;
class QSplitter;
class QToolBar;
class StatusBar;
class Study;
class TableListView;
class User;

/**
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Navigator : public QMainWindow {
    Q_OBJECT

public:
    Navigator(QWidget* parent = 0);

    void updateAll();   // called by Recorder

    ~Navigator();

public slots:
    void regenerateCatalogs();
    void updateSimulatorSettings();

protected:
    void closeEvent(QCloseEvent*);

private slots:
    void newStudy();
    void continueStudy();
    void reviewStudy();
    void preregisterPatient();
    void reports();
    void copyStudy();
    void moveStudy();
    void deleteStudy();
    void filterStudies();
    void unfilterStudies();
    void refreshCatalogs();        
    void changeCatalog();
    void ejectDisk();
    void relabelDisk();
    void exportCatalog();
    void login();
    void logout();
    void changePassword();
    void setIntervals();
    void setColumnFormats();
    void setProtocols();
    void setStudyConfigurations();
    
    void systemSettings();
    void simulatorSettings();    

    void help();
    void about();

    // these must be separate due to Qt Signal/Slot mechanism.  Can't pass
    // a parameter to a common slot.
    void setCatalogNetwork();
    void setCatalogSystem();
    void setCatalogOptical();
    void setCatalogOther();

    // This is a slot, but so far I am just using it as a regular function,
    // as it only affects the simulator settings dialog so far.  
    // This may change in the future.
    /// updates menus (hides/disables, etc.) as needed.
    void updateMenus();

private:
    // Functions to set up the Navigator main window.
    void createOpticalDrive();
    void createCentralWidget();
    void createButtonFrame();
    void createTableListView();
    void createStatusBar();
    void setupAction(QAction* action, QString statusTip,
                     const char* slotName, const char* iconName = 0);
    void createActions();
    void createMenus();
    void createToolBars();
    void writeSettings();
    void readSettings();
    void updateStatusBarUserLabel();
    void updateWindowTitle();

    // Optical disk related
    void labelDisk(bool reLabel, OpticalDisk* disk);

    // Filtering
    void processFilter();

    // Study related
    void startStudy(Study*);
    void reviewStudy(Study*);
    void reports(Study*);
    bool getStudyInformation(Study*);
    Study* getSelectedStudy();
    Study* getNewStudy();   // returns selected study or a new study
    void addStudyToCatalogs();  // add study to appropriate catalogs
    void createDataFiles(); // make study data files on disk
    void moveCopyStudyMessageBox(bool move = false);
    void copyStudy(bool);
    void doStudyCopy(MoveCopyStudyDialog&, bool move);
    bool studyOnDisk(const Study*) const; // make sure study is on current disk
    void studyNotOnDiskError();  // report study not on current disk

    // Administration related
    bool administrationAllowed();

    // misc
    void noStudySelectedError();

    // Data members
    Options* options_;
    FilterCatalogDialog* filterCatalogDialog_;  // FilterCatalog Dialog box
    Catalogs* catalogs_;
    StatusBar* statusBar_;
    OpticalDisk* currentDisk_;
    User* user_;

    // central widget stuff
    QSplitter* centralWidget_;
    AbstractButtonFrame* buttonFrame_;
    TableListView* tableListView_;

    // Study menu
    QAction* newAction_;
    QAction* continueAction_;
    QAction* reviewAction_;
    QAction* preregisterAction_;
    QAction* reportsAction_;
    QAction* copyAction_;
    QAction* moveAction_;
    QAction* deleteAction_;
    QAction* exportAction_;
    QAction* exitAction_;

    // Catalog menu
    QAction* networkSwitchAction_;
    QAction* systemSwitchAction_;
    QAction* opticalSwitchAction_;
    QAction* browseSwitchAction_;
    QAction* filterStudiesAction_;
    QAction* removeStudiesFilterAction_;
    QAction* refreshViewAction_;
    QAction* regenerateAction_;
    QAction* relabelDiskAction_;
    QAction* mergeStudiesAction_;

    // Utilities menu
    QAction* exportListsAction_;
    QAction* exportReportFormatsAction_;
    QAction* importListsAction_;
    QAction* importReportFormatsAction_;
    QAction* ejectOpticalDiskAction_;

    // Administration menu
    QAction* loginAction_;
    QAction* logoutAction_;
    QAction* changePasswordAction_;
    QAction* intervalsAction_;
    QAction* columnFormatsAction_;
    QAction* protocolsAction_;
    QAction* studyConfigurationsAction_;
    QAction* systemSettingsAction_;
    // Not part of the Prucka menu system!
    // Simulator options are set here.
    QAction* simulatorSettingsAction_;

    // Help menu
    QAction* epsimulatorHelpAction_;
    QAction* aboutAction_;

    // menus
    QMenu* studyMenu_;
    QMenu* catalogMenu_;
    QMenu* switchSubMenu_;
    QMenu* utilitiesMenu_;
    QMenu* administrationMenu_;
    QMenu* securitySubMenu_;
    QMenu* helpMenu_;

    // toolbars
    QToolBar* navigatorToolBar_;
    CatalogComboBox* catalogComboBox_;

};

#endif

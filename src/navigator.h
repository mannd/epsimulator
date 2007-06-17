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

#include "actions.h"

#include <qmainwindow.h>

class Catalog;
class Catalogs;
class CatalogComboBox;
class FilterCatalog;
class OpticalDisk;
class Options;
class StatusBar;
class Study;
class TableListView;
class User;

class QAction;
class QLabel;
class QPopupMenu;
class QPushButton;
class QSplitter;
class QToolBar;

/**
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Navigator : public QMainWindow {
    Q_OBJECT

public:
    Navigator(QWidget* parent = 0, const char *name = "navigator");
    ~Navigator();

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
    void refreshCatalog();        
    void regenerateCatalog();
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
    void saveSettings();
    void readSettings();

    // Filtering
    void processFilter();

    // Study related
    void startStudy(Study*);
    bool getStudyInformation(Study*);
    Study* getSelectedStudy();
    Study* getNewStudy();   // returns selected study or a new study
    QString studyPath(const Study*) const;    // returns path to that study
    void addStudyToCatalogs();  // add study to appropriate catalogs
    void createDataFiles(); // make study data files on disk
    void deleteDataFiles(); // delete all study data on disk
    bool studyOnDisk(const Study*) const; // make sure study is on current disk
    void studyNotOnDiskError();  // report study not on current disk
    QString createLocation() const; // creates study location from label and side
    /// FIXME below are not needed, delete
//     void deleteDataFile();  // delete data file associated with current study
//     void createDataFile();  // create a data file for a new study
//     void openDataFile();    // open a  data file for review or appending data

    // Administration related
    bool administrationAllowed();

    // misc
    void noStudySelectedError();

    // Data members
    Options* options_;
    FilterCatalog* filterCatalog_;  // FilterCatalog Dialog box
    Catalogs* catalogs_;
    StatusBar* statusBar_;
    OpticalDisk* currentDisk_;
    User* user_;

//    OpticalDiskDrive* opticalDiskDrive_;

    // central widget stuff
    /// TODO Make all these end in underscore
    QSplitter* horizontalSplitter_;
    TableListView* tableListView_;

    // Actions
    Actions actions_;
    // Study menu
    QAction* newAct_;
    QAction* continueAct_;
    QAction* reviewAct_;
    QAction* preregisterAct_;
    QAction* reportsAct_;
    QAction* copyAct_;
    QAction* moveAct_;
    QAction* deleteAct_;
    QAction* exportAct_;
    QAction* exitAct_;

    // Catalog menu
//    QAction* switchAct_;
    QAction* networkSwitchAct_;
    QAction* systemSwitchAct_;
    QAction* opticalSwitchAct_;
    QAction* browseSwitchAct_;
    QAction* filterStudiesAct_;
    QAction* removeStudiesFilterAct_;
    QAction* refreshViewAct_;
    QAction* regenerateAct_;
    QAction* relabelDiskAct_;
    QAction* mergeStudiesAct_;

    // Utilities menu
    QAction* exportListsAct_;
    QAction* exportReportFormatsAct_;
    QAction* importListsAct_;
    QAction* importReportFormatsAct_;
    QAction* ejectOpticalDiskAct_;

    // Administration menu
    QAction* loginAct_;
    QAction* logoutAct_;
    QAction* changePasswordAct_;
    QAction* intervalsAct_;
    QAction* columnFormatsAct_;
    QAction* protocolsAct_;
    QAction* studyConfigurationsAct_;
    QAction* systemSettingsAct_;
    
    // Not part of the Prucka menu system!
    // Simulator options are set here.
    QAction* simulatorOptionsAct_;

    // Help menu
    QAction* epsimulatorHelpAct_;
    QAction* aboutAct_;

    // menus
    QPopupMenu *studyMenu_;
    QPopupMenu *catalogMenu_;
    QPopupMenu* switchSubMenu_;
    QPopupMenu *utilitiesMenu_;
    QPopupMenu *administrationMenu_;
    QPopupMenu *securitySubMenu_;
    QPopupMenu *helpMenu_;

    // toolbars
    QToolBar* navigatorToolBar_;
    CatalogComboBox* catalogComboBox_;

};

#endif

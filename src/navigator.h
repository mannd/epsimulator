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

#include "options.h"
#include "study.h"

#include <qmainwindow.h>

class Catalog;
class Catalogs;
class CatalogComboBox;
class FilterCatalog;
class OpticalDisk;
class OpticalDiskDrive;
class Options;
class TableListView;

class QAction;
class QFrame;
class QGridLayout;
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
  
protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void newStudy();
    void continueStudy();
    void reviewStudy();
    void preregisterPatient();
    void reports();
    void deleteStudy();
    void filterStudies();
    void unfilterStudies();
    void refreshCatalog();        
    void regenerateCatalog();
    void changeCatalog();
    void ejectDisk();
    void relabelDisk();
    
    // these must be separate due to Qt Signal/Slot mechanism.  Can't pass
    // a parameter to a common slot.
    void setCatalogNetwork();
    void setCatalogSystem();
    void setCatalogOptical();
    void setCatalogOther();

    void systemSettings();

    void help();
    void about();

private:

    static const int buttonSize = 70;   // size of square buttons in blue panel

    // Functions to set up the Navigator main window.
    void createCentralWidget();
    void createButtonFrame();
    void setupButton(QPushButton* button, QString pixmapName, 
                     QLabel* label, const char* slotName, 
		     bool lastButton = false);
    void createTableListView();
    void createStatusBar();
    void updateSourceLabel();
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
    void startStudy();
    bool getStudyInformation();
    bool studySelected();
    void prepareStudy();    // clears study_ if no study selected
    void deleteDataFile();  // delete data file associated with current study
    void createDataFile();  // create a data file for a new study
    void openDataFile();    // open a  data file for review or appending data

    // Data members
    /// TODO study_ should be Study*
    Study study_;           // current study
    Options* options_;
    FilterCatalog* filterCatalog_;  // FilterCatalog Dialog box
    Catalogs* catalogs_;
    
    OpticalDisk* currentDisk_;
//    OpticalDiskDrive* opticalDiskDrive_;

    // StatusBar labels
    QLabel* messageLabel_;
    QLabel* userLabel_;
    QLabel* sourceLabel_;
    QLabel* filterLabel_;

    // central widget stuff
    /// TODO Make all these end in underscore
    QSplitter* horizontalSplitter_;
    QFrame* buttonFrame_;
    QGridLayout* buttonFrameLayout_;
    TableListView* tableListView_;

    // Study menu
    QAction *newAct;
    QAction *continueAct;
    QAction *reviewAct;
    QAction *preregisterAct;
    QAction *reportsAct;
    QAction *copyAct;
    QAction *deleteAct;
    QAction* exportAct_;
    QAction *exitAct;

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
    QAction *loginAct;
    QAction *logoutAct;
    QAction *changePasswordAct;
    QAction *intervalsAct;
    QAction *columnFormatsAct;
    QAction *protocolsAct;
    QAction *studyConfigurationsAct;
    QAction *systemSettingsAct;

    // Help menu
    QAction* epsimulatorHelpAct_;
    QAction *aboutAct;

    // menus
    QPopupMenu *studyMenu;
    QPopupMenu *catalogMenu;
    QPopupMenu* switchSubMenu_;
    QPopupMenu *utilitiesMenu;
    QPopupMenu *administrationMenu;
    QPopupMenu *securitySubMenu;
    QPopupMenu *helpMenu;

    // toolbars
    QToolBar* navigatorToolBar_;
    CatalogComboBox* catalogComboBox_;

};

#endif

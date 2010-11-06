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

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "abstractmainwindow.h"
#include "catalog.h"

class QAction;
class QCloseEvent;
class QMenu;
class QSplitter;
class QToolBar;

namespace EpCore {
    class User;
    class Options;
}

namespace EpHardware { 
    namespace EpOpticalDisk { 
	class OpticalDisk; }
}

namespace EpStudy { 
    class Study; 
}

namespace EpNavigator {

class AbstractButtonFrame;
class CatalogComboBox;
class FilterCatalogDialog;
class MoveCopyStudyDialog;
class StatusBar;
class TableListView;

/**
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class Navigator : public EpGui::AbstractMainWindow {
    Q_OBJECT

public:
    Navigator(QWidget* parent = 0);
    ~Navigator();

public slots:
    void regenerateCatalogs();
    virtual void updateSimulatorSettings();
    virtual void updateSystemSettings();

protected:
    void closeEvent(QCloseEvent*);
    virtual EpCore::User* user() const {return user_;}
    virtual EpHardware::EpOpticalDisk::OpticalDisk* currentDisk() const {
        return currentDisk_;}
    virtual EpCore::Options* options() const {return options_;}

private slots:
    void newStudy();
    void continueStudy();
    void reviewStudy();
    void preregisterPatient();
    void reports();
    void copyStudy();
    void moveStudy();
    void deleteStudy();
    void exportCatalog();
    void filterStudies();
    void unfilterStudies();
    void refreshCatalogs();
    void changeCatalog();
    void exportLists();
    void exportReportFormats();
    void importLists();
    void importReportFormats();
    void ejectDisk();
    void relabelDisk();
    void pacingSites();
    void phases();
    void arrhythmiaTypes();
    void arrhythmiaTolerances();
    void blockDescriptions();
    void refractoryLocations();
    void setStudyConfigurations();
    void manageSections();
    void manageFormats();
    void clearSelection();
    void editStudyInCatalogs(const EpStudy::Study*);

    // these must be separate due to Qt Signal/Slot mechanism.  Can't pass
    // a parameter to a common slot.
    void setCatalogNetwork();
    void setCatalogSystem();
    void setCatalogOptical();
    void setCatalogOther();

    void updateAll();

private:
    Q_DISABLE_COPY(Navigator)

    enum DataFlow {Import, Export};
    enum DataType {Lists, ReportFormats};
    enum MoveType {Move, Copy};

    // Functions to set up the Navigator main window.
    void initializeOpticalDisk();
    void createOpticalDrive();
    void createCatalogs();
    void createCentralWidget();
    void createButtonFrame();
    void createTableListView();
    void createStatusBar();
    void createActions();
    void createMenus();
    void createToolBars();
    void createLists();
    void writeSettings();
    virtual void readSettings();
    void updateMenus();
    void updateStatusBarUserLabel();
    void updateWindowTitle();

    // Optical disk related
    void createDefaultDataDir();    // creates ~/MyStudies if it doesn't exist
    void labelDisk(bool reLabel, EpHardware::EpOpticalDisk::OpticalDisk* disk);

    // Filtering
    void processFilter();
    void applyFilter();

    // Catalog related
    void setCatalog(Catalog::Source);

    // List related
    void editEpList(const QString& table,
                    const QString& title,
                    const QString& label);

    // Study related
    void startStudy(EpStudy::Study*, bool review = false);
    void reviewStudy(EpStudy::Study*);
    void reports(EpStudy::Study*);
    bool getStudyInformation(EpStudy::Study*);
    EpStudy::Study* getSelectedStudy();
    EpStudy::Study* getNewStudy();   // returns selected study or a new study
    void addStudyToCatalogs();  // add study to appropriate catalogs
    void createDataFiles(); // make study data files on disk
    void moveStudyMessageBox(MoveType);
    bool moveDataMessageBox(DataFlow, DataType);
    void moveStudy(MoveType);
    void moveStudyData(MoveCopyStudyDialog&, MoveType);
    void moveData(DataFlow, DataType);
    bool studyOnDisk(const EpStudy::Study*) const; // make sure study on current disk
    void studyNotOnDiskError();  // report study not on current disk

    // misc
    void noStudySelectedError();

    // Data members
    FilterCatalogDialog* filterCatalogDialog_;  // FilterCatalog Dialog box
    Catalogs* catalogs_;
    StatusBar* statusBar_;
    EpHardware::EpOpticalDisk::OpticalDisk* currentDisk_;
    EpCore::User* user_;
    EpCore::Options* options_;

    // central widget stuff
    QSplitter* centralWidget_;
    AbstractButtonFrame* buttonFrame_;
    TableListView* tableListView_;

    // Actions

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
    QAction* proceduresAction_;
    QAction* staffAction_;
    QAction* suppliesAction_;
    QAction* medicationsAction_;
    QAction* complicationsAction_;
    QAction* contrastAction_;
    QAction* pacingSitesAction_;
    QAction* phasesAction_;
    QAction* arrhythmiaTypesAction_;
    QAction* arrhythmiaTolerancesAction_;
    QAction* blockDescriptionsAction_;
    QAction* refractoryLocationsAction_;
    QAction* studyConfigurationsAction_;
    QAction* manageSectionsAction_;
    QAction* manageFormatsAction_;
    QAction* templatesAction_;
#ifndef NDEBUG
    QAction* testWidgetAction_;
#endif

    // Help menu
    // Actions returned by AbstractMainWindow

    // Non-menu actions
    QAction* clearSelectionAction_;

    // top-level menus
    QMenu* studyMenu_;
    QMenu* catalogMenu_;
    QMenu* utilitiesMenu_;
    QMenu* administrationMenu_;
    QMenu* helpMenu_;

    // toolbars
    QToolBar* navigatorToolBar_;
    CatalogComboBox* catalogComboBox_;
};


}

#endif

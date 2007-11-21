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

/** @file
 * Basically the navigator window is the main window.  When switching to
 * the epsimulator window, we will actually just be changing the menus
 * and the central widget
 */
#include "navigator.h"

#include "actions.h"
#include "buttonframe.h"
#include "catalog.h"
#include "catalogcombobox.h"
#include "changepassworddialog.h"
#include "disklabeldialog.h"
#include "epfuns.h"
#include "error.h"
#include "filtercatalogdialog.h"
#include "movecopystudydialog.h"    // temporary
#include "movecopystudywizard.h"
#include "opticaldisk.h"
#include "options.h"
#include "passworddialog.h"
#include "patientdialog.h"
#include "recorder.h"
#include "simulatorsettingsdialog.h"
#include "settings.h"
#include "statusbar.h"
#include "study.h"
#include "selectstudyconfigdialog.h"
//#include "studycopywizard.h"
#include "systemdialog.h"
#include "tablelistview.h"
#include "user.h"
#include "utilities.h"
#include "versioninfo.h"

#include <QAction>
#include <QDateTime>
#include <QFileDialog>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QRegExp>
#include <QSplitter>
#include <QTextIStream>
#include <QTextOStream>
#include <QToolBar>

#include <algorithm>

#ifndef NDEBUG
#include <iostream> // for debugging
#endif

/**
 * Navigator constructor
 */
Navigator::Navigator(QWidget* parent)
    : QMainWindow( parent, Qt::WDestructiveClose ),
                   options_(Options::instance()), filterCatalogDialog_(0),
                   catalogs_(0), currentDisk_(0), user_(User::instance()),
                   recorder_(0) {
    do {
        createOpticalDrive();
    } while (!currentDisk_);
    catalogs_ = new Catalogs(options_, currentDisk_->fullPath());
    createActions();
    createMenus();
    createToolBars();
    createCentralWidget();
    createStatusBar();

    connect(catalogComboBox_, SIGNAL(activated(int)),
        this, SLOT(changeCatalog()));

    setCaption(tr("%1 Navigator").arg(VersionInfo::instance()->programName()));
}

// protected


// private slots

// Blue bar actions
void Navigator::newStudy() {
    if (!currentDisk_->isLabeled()) 
        relabelDisk();
    // if after all the above we finally have a label...
    if (currentDisk_->isLabeled()) {
        Study* study = getNewStudy();
        SelectStudyConfigDialog* selectStudyConfigDialog  = 
            new SelectStudyConfigDialog(this);
        if (selectStudyConfigDialog->exec() == QDialog::Accepted) {
            study->setConfig(selectStudyConfigDialog->config());
            if (getStudyInformation(study)) {
                catalogs_->addStudy(study, currentDisk_->label(),
                                    currentDisk_->translatedSide(),
                                    options_->labName(), user_->machineName());
                refreshCatalogs();
                startStudy(study);
        /// FIXME bug: startStudy deletes Navigator, then returns, and the 2 
        /// deletes below happen, deleting an already deleted pointer.  That
        /// can't work.
            }
        }
        delete selectStudyConfigDialog;
    }
}

void Navigator::continueStudy() {
    Study* study = getSelectedStudy();
    if (!study) {
        noStudySelectedError();
        return;
    }
    if (study->isPreregisterStudy()) {
        /// TODO make this a new study, but use original time/date?    
        SelectStudyConfigDialog* selectStudyConfigDialog  = 
            new SelectStudyConfigDialog(this);
        if (selectStudyConfigDialog->exec() == QDialog::Accepted) {
            study->setConfig(selectStudyConfigDialog->config());
            catalogs_->deleteStudy(study);
            catalogs_->addStudy(study, currentDisk_->label(),
                    currentDisk_->translatedSide(),
                    options_->labName(), user_->machineName());
            refreshCatalogs();
            startStudy(study);
        }
        delete selectStudyConfigDialog;
    }
    else if (!studyOnDisk(study)) 
        studyNotOnDiskError();
    else
        startStudy(study);
}

void Navigator::reviewStudy() {
    Study* study = getSelectedStudy();
    if ((study) && !study->isPreregisterStudy()) {
        // if study not on this optical disk change disk and return
        if (!studyOnDisk(study)) {
            studyNotOnDiskError();
            delete study;
            return;
        }
        reviewStudy(study);
    }
    else
        noStudySelectedError();
}

void Navigator::preregisterPatient() {
    Study* study = getNewStudy();
    study->makePreregisterStudy();
    if (getStudyInformation(study)) {
        catalogs_->addStudy(study);
        refreshCatalogs();
        // refreshCatalogs() catches IO errors, so should 
        // be no resource leak
    }
    delete study;
}

void Navigator::reports()  {
    Study* study = getSelectedStudy();
    if ((study) && !study->isPreregisterStudy()) {
        // if study not on this optical disk change disk and return
        if (!studyOnDisk(study)) {
            studyNotOnDiskError();
            delete study;
            return;
        }
        reports(study);
    }
    else
        noStudySelectedError();
}

void Navigator::copyStudy() {
    CopyStudyDialog* w = new CopyStudyDialog(this);
    //w->setCurrentIndex(0);
    w->show();
/*    if (w->exec())
        ;
    delete w;*/
}
//     StudyCopyWizard* wizard = new StudyCopyWizard(this);
//     wizard->setSourcePathName(currentDisk_->fullPath());
//     if (wizard->exec()) {
//         OpticalDisk* disk = new OpticalDisk(wizard->destinationPathName());
//         disk->readLabel();
//         if (!disk->isLabeled())
//             labelDisk(false, disk);
//         Catalog* catalog = new Catalog(wizard->destinationPathName());
//         wizard->copy();
//         // for each study in studiesList
//         //		copy study form source folder to destination folder
//         // 		throw something if any copy fails
//         // after successful copying, don't update catalog
//         // make a catalog.dat file in the destination folder
//         // data on source is not erased
//         ;
//         delete catalog;
//         delete disk;
//     }
//     delete wizard;
//}

void Navigator::moveStudy() {
//     if (administrationAllowed()) {
//         StudyMoveWizard* wizard = new StudyMoveWizard(this);
//         wizard->setSourcePathName(currentDisk_->fullPath());
//         if (wizard->exec()) {
//             OpticalDisk* disk = new OpticalDisk(wizard->destinationPathName());
//             disk->readLabel();
//             if (!disk->isLabeled())
//                 labelDisk(false, disk);
//             Catalog* catalog = new Catalog(wizard->destinationPathName());
//             wizard->move();
//         // for each study in studiesList
//         //		copy study form source folder to destination folder
//         // 		throw something if any copy fails
//         // after successful copying, 
//         // make a catalog.dat file in the destination folder
//         // now update system catalogs
//         // now erase data on source
//             ;
//             delete catalog;
//             delete disk;
//         }
//         delete wizard;
//     }
}

void Navigator::deleteStudy() {
    try {
        if (Study* study = getSelectedStudy()) {
            if (!study->isPreregisterStudy() && !catalogs_->studyPresentOnOpticalDisk(study)) {
                QMessageBox::information(this, tr("Study Not On Optical Disk"),
                    tr("Insert the disk containing this study and try again."));
                return;
            }
            int ret = QMessageBox::warning(
                this, tr("Delete Study?"),
                tr("The selected study will be permanently "
                "deleted.  Do you wish to continue?"),
                QMessageBox::Yes ,
                QMessageBox::No | QMessageBox::Default, // default is NO!
                QMessageBox::Cancel | QMessageBox::Escape);
            if (ret == QMessageBox::Yes) {
                catalogs_->deleteStudy(study);
                refreshCatalogs();
                // delete item;
                if (!study->isPreregisterStudy())
                    deleteDataFiles(currentDisk_->studiesPath() + study->studyDirName());
            }
            delete study;
        } 
        else 
            noStudySelectedError();
    }
    catch (EpSim::FileNotFoundError& e) {
        QMessageBox::warning(this, tr("Problem Deleting Study"),
            tr("Could not find study file %1").arg(e.fileName()));
    }
    catch (EpSim::DeleteError&) {
        QMessageBox::warning(this, tr("Problem Deleting Study"),
            tr("Errors occurred while trying to delete study data."));
    }
}

void Navigator::filterStudies() {
    if (!filterCatalogDialog_)
        filterCatalogDialog_ = new FilterCatalogDialog(this);
    if (filterCatalogDialog_->exec() == QDialog::Accepted) 
        processFilter();
}

void Navigator::unfilterStudies() {
    // do the unfiltering here
    tableListView_->removeFilter();
    statusBar_->updateFilterLabel(false);
    removeStudiesFilterAct_->setEnabled(false);
    filterStudiesAct_->setEnabled(true);
//    refreshViewAct_->setEnabled(true);
    regenerateAct_->setEnabled(true);
}

void Navigator::refreshCatalogs() {
    catalogComboBox_->refresh();
    catalogs_->refresh();
    catalogs_->setCurrentCatalog(catalogComboBox_->source());
    tableListView_->load(catalogs_->currentCatalog());
    // reapply filter if present
    if (tableListView_->filtered())
        processFilter();
}

void Navigator::regenerateCatalogs() {
    catalogs_->regenerate(currentDisk_->label(), currentDisk_->side(), 
                          options_->labName(), user_->machineName());
    refreshCatalogs();
}

void Navigator::changeCatalog() {
     catalogs_->setCurrentCatalog(catalogComboBox_->source());
     tableListView_->load(catalogs_->currentCatalog());
     // reapply filter if present
     if (tableListView_->filtered())
         processFilter();
    statusBar_->updateSourceLabel(catalogs_->currentCatalog()->path());
}

void Navigator::ejectDisk() {
    // make sure there is a label, to avoid blank label
    // in the label list box.
    if (!currentDisk_->isLabeled())
        relabelDisk();
    if (!currentDisk_->isLabeled())
        return;     // give up if they haven't relabeled it
    currentDisk_->eject(this);
    createOpticalDrive();
    if (!currentDisk_->isLabeled())
        labelDisk(false, currentDisk_);
    delete catalogs_;
    catalogs_ = new Catalogs(options_, currentDisk_->fullPath());
    refreshCatalogs();
    statusBar_->updateSourceLabel(catalogs_->currentCatalog()->path());
}

/** Here is the logic:
            Not an emulated disk:   Allow side changes any time.  All
                three buttons are always available. 
            Emulated disk:
                New disk: Only allow side change if isTwoSided()
                Flipping a disk: allow side change (don't allow flipping 1 sided disks)
                Relabeling: don't allow any side changes at all
            OpticalDisk::allowSideChange() will set itself appropriately
 */
/**
 * 
 * @param reLabel true if relabeling already labeled disk
 * @param disk OpticalDisk that is to be labeled
 */
void Navigator::labelDisk(bool reLabel, OpticalDisk* disk) {
    DiskLabelDialog* diskLabelDialog = new DiskLabelDialog(this);
    QString oldLabel = disk->label();
    diskLabelDialog->setLabel(oldLabel);
    // Disabled buttons can't be set, so do this first.
    diskLabelDialog->setSide(disk->side());
 
    diskLabelDialog->enableNoneButton(disk->showAllSideButtons() || 
        !disk->isTwoSided());    
    diskLabelDialog->enableSideButtons(disk->allowSideChange());
    if (diskLabelDialog->exec() == QDialog::Accepted) {
        disk->setLabel(diskLabelDialog->label());
        disk->setSide(diskLabelDialog->side());
        disk->writeLabel();
        disk->setIsLabeled(true);
        if (reLabel)
            catalogs_->relabel(diskLabelDialog->label(), diskLabelDialog->side());
        refreshCatalogs();
    }
    delete diskLabelDialog;
}

void Navigator::relabelDisk() {
    labelDisk(true, currentDisk_);
}

void Navigator::login() {
    if (!user_->isAdministrator()) {
        PasswordDialog* pwDialog = 
            new PasswordDialog(options_,this);
        if (pwDialog->exec() == QDialog::Accepted) {
            user_->makeAdministrator(true);
            updateStatusBarUserLabel();
            updateMenus();
        }
        delete pwDialog;
    }
}

void Navigator::updateStatusBarUserLabel() {
    statusBar_->updateUserLabel(options_->oldStyleNavigator() ?
        user_->role() : user_->name());
}

void Navigator::logout() {
    user_->makeAdministrator(false);
    updateStatusBarUserLabel();
    updateMenus();
}

void Navigator::changePassword() {
    ChangePasswordDialog* cpDialog = new ChangePasswordDialog(options_, this);
    if (cpDialog->exec() == QDialog::Accepted) {
        cpDialog->changePassword();
    }
    delete cpDialog;   
}

/// Checks to see if administrator access if required, if it is,
/// and not logged in as administrator, will do a login, then
/// will allow adminstration if user successfully logged in.
bool Navigator::administrationAllowed() {
    if (!options_->administratorAccountRequired())
        return true;
    login();
    return user_->isAdministrator();
}



void Navigator::noStudySelectedError() {
    QMessageBox::warning(this, tr("No Study Selected"),
                         tr("You must select a study first to do this."));
}

void Navigator::setIntervals() {
    if (administrationAllowed())
        EpSim::filler(this);
}

void Navigator::setColumnFormats() {
    if (administrationAllowed())
        EpSim::filler(this);
}

void Navigator::setProtocols() {
    if (administrationAllowed())
        EpSim::filler(this);
}

void Navigator::setStudyConfigurations() {
    if (administrationAllowed())
        EpSim::filler(this);
}

void Navigator::setCatalogNetwork() {
    catalogComboBox_->setSource(Catalog::Network);
    changeCatalog();
}

void Navigator::setCatalogSystem() {
    catalogComboBox_->setSource(Catalog::System);
    changeCatalog();
}

void Navigator::setCatalogOptical() {
    catalogComboBox_->setSource(Catalog::Optical);
    changeCatalog();
}

void Navigator::setCatalogOther() {
    QFileDialog* fd = new QFileDialog(this, tr("Select Catalog"),
        options_->systemCatalogPath(), Catalog::defaultFileName());
    if (fd->exec() == QDialog::Accepted) {
        catalogs_->setCatalogPath(Catalog::Other, fd->directory().path());
        catalogs_->refresh();   // to reload Other catalog
        catalogComboBox_->setSource(Catalog::Other);
        changeCatalog();
    }
    delete fd;
}

void Navigator::exportCatalog() {
    QFileDialog* fd = new QFileDialog(this, tr("Export Catalog"),
        QDir::homeDirPath(), tr("Comma-delimited (*.csv)"));
    fd->setMode(QFileDialog::AnyFile);
    fd->setAcceptMode(QFileDialog::AcceptSave);
    if (fd->exec() == QDialog::Accepted) {
        QStringList files = fd->selectedFiles();
        QString fileName = QString();
        if (!files.isEmpty())
            fileName = files[0];
        if (!fileName.isEmpty()) {
            try {
                tableListView_->exportCSV(fileName);
            }
            catch (EpSim::IoError& e) {
                std::cerr << e.what() << std::endl;
                QMessageBox::warning(this, tr("Error"),
                    tr("Could not export Catalog to %1").arg(e.fileName()));
            }
        }
    }
    delete fd;
}

void Navigator::simulatorSettings() {
    if (administrationAllowed()) {
        SimulatorSettingsDialog* simDialog = 
            new SimulatorSettingsDialog(options_, this);
        if (simDialog->exec() == QDialog::Accepted) {
            simDialog->setOptions();
            updateMenus();
            // change type of optical disk if needed
            do {
                createOpticalDrive();
            } while (!currentDisk_);
            delete catalogs_;
            catalogs_ = new Catalogs(options_, currentDisk_->fullPath());
            tableListView_->setOldStyle(options_->oldStyleNavigator());
            tableListView_->adjustColumns(true);
            refreshCatalogs();   // This repopulates the TableListView.
            // Need to do below to make sure user label
            // matches Navigator style.
            statusBar_->updateSourceLabel(catalogs_->currentCatalog()->path());
            updateStatusBarUserLabel();
            /// TODO other effects of changing simulator settings below
        }
        delete simDialog;
    }
}

void Navigator::systemSettings() {
    if (administrationAllowed()) {
        /// FIXME Need disk space-time function.
        SystemDialog* systemDialog = new SystemDialog(options_, 
            currentDisk_->studiesPath(), currentDisk_->label(),
            currentDisk_->translatedSide(), this);
        if (systemDialog->exec() == QDialog::Accepted) {
            systemDialog->setOptions();
            // menu is changed
            networkSwitchAct_->setEnabled(options_->enableNetworkStorage());
            // optical disk, status bar and catalog might be changed 
            do {
                createOpticalDrive();
            } while (!currentDisk_);
            delete catalogs_;
            /// TODO change current disk here
            catalogs_ = new Catalogs(options_, currentDisk_->fullPath());
            refreshCatalogs();
            statusBar_->updateSourceLabel(catalogs_->currentCatalog()->path());
        }
        delete systemDialog;
    }
}

void Navigator::help() {
    EpUi::help(this);
}

void Navigator::about() {
    EpUi::about(this);
}

// private

void Navigator::createOpticalDrive() {
    try {
        // make sure any old disk info is saved and then delete disk
 //       if (currentDisk_)
 //           currentDisk_->writeLabel();
        delete currentDisk_;
        currentDisk_ = 0;
        if (options_->emulateOpticalDrive()) {
            currentDisk_ = EmulatedOpticalDisk::getLastDisk(
                options_->opticalStudyPath());
            if (!currentDisk_) {
                currentDisk_ = new EmulatedOpticalDisk(
                    options_->opticalStudyPath(),
                    options_->dualSidedDrive());
                currentDisk_->saveLastDisk();
            }
        }
        else
            currentDisk_ = new OpticalDisk(options_->opticalStudyPath());
        currentDisk_->readLabel();
    }
    catch (EpSim::IoError& e) { 
        int ret = QMessageBox::warning(this, tr("Error"),
                             tr("Could not find disk %1. "
                                "Enter the correct path to your "
                                "optical drive or Exit").arg(e.fileName()), 
                                tr("Continue"),
                                tr("Exit Program"), "", 0, 0);
        if (ret == 1)
            exit(1);
        else {
            QFileDialog* fd = new QFileDialog(this);
            fd->setMode(QFileDialog::Directory);
            if (fd->exec() == QDialog::Accepted) {
                QStringList files = fd->selectedFiles();
                QString fileName = QString();
                if (!files.isEmpty()) {
                    fileName = files[0];
                    if (!fileName.isEmpty()) {
                        options_->setOpticalStudyPath(fileName);
                        options_->writeSettings();
                    }
                }
            }
        }
    }
}

void Navigator::createCentralWidget() {
    horizontalSplitter_ = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(horizontalSplitter_);
    createButtonFrame();
    createTableListView();
    readSettings(); 
    refreshCatalogs();
}

/**
 * Create the "blue bar" to the side of the Navigator window.  Uses
 * setupButton to make each button.  The parent of the buttonFrame is
 * the horizontalSplitter_.  This is also the parent of the 
 * tableListView_.
 */
void Navigator::createButtonFrame() {
    if (Options::instance()->newStyleBlueBar()) // set up flat buttons
        buttonFrame_ = new NewStyleButtonFrame(horizontalSplitter_);
    else
        buttonFrame_ = new OldStyleButtonFrame(horizontalSplitter_);
    buttonFrame_->addButton("New Study", "hi64-newstudy", SLOT(newStudy()));
    buttonFrame_->addButton("Continue Study", "hi64-continuestudy",
        SLOT(continueStudy()));
    buttonFrame_->addButton("Review Study", "hi64-reviewstudy",
        SLOT(reviewStudy()));
    buttonFrame_->addButton("Pre-Register", "hi64-preregister",
        SLOT(preregisterPatient()));
    buttonFrame_->addButton("Reports", "hi64-reports", 
        SLOT(reports()), true); 
}

/** @brief Creates the TableListView object.
 *
 * The tableListView_ contains the list of studies, and reflects whatever
 * source is current in the catalogComboBox_.
 */
void Navigator::createTableListView() {
    tableListView_ = new TableListView(horizontalSplitter_,
        options_->oldStyleNavigator());
    connect(tableListView_, SIGNAL(doubleClicked(Q3ListViewItem*, 
        const QPoint&, int)), this, SLOT(newStudy()));
}

void Navigator::createStatusBar() {
    statusBar_ = new StatusBar(catalogs_->currentCatalog()->path(), this);
    setStatusBar(statusBar_);
    updateStatusBarUserLabel();
}

void Navigator::updateMenus() {
    bool showSimulatorSettings = !options_->hideSimulatorMenu() ||
        user_->isAdministrator();
    simulatorOptionsAct_->setVisible(showSimulatorSettings);
}

using EpCore::createAction;

void Navigator::createActions() {
    // Study menu
    newAct_ = createAction(this, tr("&New..."), tr("New study"),
        SLOT(newStudy()), QKeySequence(tr("Ctrl+N")), "hi32-newstudy.png");
    continueAct_ = createAction(this, tr("&Continue"),tr ("Continue study"),
        SLOT(continueStudy()), 0, "hi32-continuestudy.png");
    reviewAct_ = createAction(this, tr("&Review"),
        tr("Review study"), SLOT(reviewStudy()), 0, "hi32-reviewstudy.png");
    preregisterAct_= createAction(this, tr("&Pre-Register"),
        tr("Pre-register patient"), SLOT(preregisterPatient()), 
        0, "hi32-preregister.png");
    reportsAct_= createAction(this, tr("R&eports..."),
        tr("Procedure reports"), SLOT(reports()), 0,
        "hi32-reports.png" );
    copyAct_= createAction(this, tr("Copy..."), tr("Copy study"),
        SLOT(copyStudy()));
    moveAct_ = createAction(this, tr("Move..."), tr("Move study"),
        SLOT(moveStudy()));
    deleteAct_= createAction(this, tr("Delete..."), tr("Delete study"),
        SLOT(deleteStudy()));
    exportAct_ = createAction(this, tr("Export..."), tr("Export study"),
        SLOT(exportCatalog()), 0, "hi32-exportstudy.png");
    exitAct_= createAction(this, tr("E&xit"), tr("Exit EP Simulator"),
        SLOT(close()), tr("Ctrl+Q"));
    // Catalog menu
    // Submenu of Switch...
    // an action "Achive Server" is skipped here, but is present on Prucka
    networkSwitchAct_ = createAction(this, tr("Network"),
        tr("Switch to network catalog"), SLOT(setCatalogNetwork()));
    // networkSwitchAct_ only enabled if set in options
    networkSwitchAct_->setEnabled(options_->enableNetworkStorage());
    systemSwitchAct_ = createAction(this, tr("System"),
        tr("Switch to system catalog"), SLOT(setCatalogSystem()));
    opticalSwitchAct_ = createAction(this, tr("Optical"),
        tr("Switch to optical catalog"), SLOT(setCatalogOptical()));
    // back to main menu items
    browseSwitchAct_ = createAction(this, tr("Browse..."),
        tr("Browse for catalog files"), SLOT(setCatalogOther()));
    filterStudiesAct_ = createAction(this, tr("Filter Studies..."),
        tr("Filter studies"), SLOT(filterStudies()),
        0, "hi32-filterstudies.png");
    removeStudiesFilterAct_ = createAction(this, tr("Remove Studies Filter"),
        tr("Remove studies filter"), SLOT(unfilterStudies()),
        0, "hi32-removefilter.png");
    // inactivate removeStudiesFilterAct_ by default
    removeStudiesFilterAct_->setEnabled(false);
    refreshViewAct_ = createAction(this, tr("Refresh"),
        tr("Refresh the catalog"), SLOT(refreshCatalogs()),
        0, "hi32-refreshcatalog.png");
    regenerateAct_ = createAction(this, tr("Regenerate"),
        tr("Regenerate the catalog"), SLOT(regenerateCatalogs()));
    relabelDiskAct_ = createAction(this, tr("Re-Label Disk..."),
        tr("Re-label the optical disk"), SLOT(relabelDisk()));
    mergeStudiesAct_ = createAction(this, tr("Merge Studies..."),
        tr("Merge studies together"));

    // Utilities menu
    exportListsAct_ = createAction(this, tr("Export Lists..."),
        tr("Export lists"));
    exportReportFormatsAct_ = createAction(this, tr("Export Report Formats..."),
        tr("Export report formats"));
    importListsAct_ = createAction(this, tr("Import Lists..."),
        tr("Import lists"));
    importReportFormatsAct_ = createAction(this, tr("Import Report Formats..."),
        tr("Import report formats"));
    ejectOpticalDiskAct_ = createAction(this, tr("Eject Optical Disk"),
        tr("Eject optical disk"), SLOT(ejectDisk()));

    // Administration menu
    loginAct_= createAction(this, tr("Login..."),
        tr("Login as administrator"), SLOT(login()));
    logoutAct_= createAction(this, tr("Logout"),
        tr("Logout from administrator"), SLOT(logout()));
    changePasswordAct_= createAction(this, tr("Change Password..."),
        tr("Change administrator password"), SLOT(changePassword()));
    intervalsAct_= createAction(this, tr("Intervals"),
        tr("Intervals"), SLOT(setIntervals()));
    columnFormatsAct_= createAction(this, tr("Column Formats"),
        tr("Column formats"), SLOT(setColumnFormats()));
    protocolsAct_= createAction(this, tr("Protocols"),
        tr("Protocols"), SLOT(setProtocols()));
    studyConfigurationsAct_= createAction(this, tr("Study Configurations"),
        tr("Study configurations"), SLOT(setStudyConfigurations()));
    systemSettingsAct_= createAction(this, tr("System Settings"),
        tr("Change system settings"), SLOT(systemSettings()));
    simulatorOptionsAct_ = createAction(this, tr("*Simulator Settings*"),
        tr("Change simulator settings"), SLOT(simulatorSettings()));

    // Help menu
    epsimulatorHelpAct_ = createAction(this, tr("EP Simulator Help..."),
        tr("Get help for EP Simulator"), SLOT(help()), tr("F1"));
    aboutAct_= createAction(this, tr("&About EP Simulator"),
        tr("About EP Simulator"), SLOT(about()));
}

void Navigator::createToolBars() {
    navigatorToolBar_ = new QToolBar(tr("Navigator")); 
    navigatorToolBar_->setAutoFillBackground(true);
    catalogComboBox_ = new CatalogComboBox();
    navigatorToolBar_->addWidget(catalogComboBox_);
    navigatorToolBar_->addSeparator();
    navigatorToolBar_->addAction(filterStudiesAct_);
    navigatorToolBar_->addAction(removeStudiesFilterAct_);
    navigatorToolBar_->addSeparator();
    navigatorToolBar_->addAction(refreshViewAct_);
    navigatorToolBar_->addSeparator();
    navigatorToolBar_->addAction(exportAct_);
    addToolBar(navigatorToolBar_);
}

void Navigator::createMenus() {

    studyMenu_ = menuBar()->addMenu(tr("&Study"));
    studyMenu_->addAction(newAct_);
    studyMenu_->addAction(continueAct_);
    studyMenu_->addAction(reviewAct_);
    studyMenu_->addAction(preregisterAct_);
    studyMenu_->addAction(reportsAct_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(copyAct_);
    studyMenu_->addAction(moveAct_);
    studyMenu_->addAction(deleteAct_);
    studyMenu_->addAction(exportAct_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(exitAct_);

    catalogMenu_ = menuBar()->addMenu(tr("&Catalog"));
    switchSubMenu_ = new QMenu(tr("Switch"));
    switchSubMenu_->addAction(networkSwitchAct_);
    switchSubMenu_->addAction(systemSwitchAct_);
    switchSubMenu_->addAction(opticalSwitchAct_);
    switchSubMenu_->addAction(browseSwitchAct_);
    catalogMenu_->addMenu(switchSubMenu_);
    catalogMenu_->addAction(filterStudiesAct_);
    catalogMenu_->addAction(removeStudiesFilterAct_);
    catalogMenu_->addSeparator();
    catalogMenu_->addAction(refreshViewAct_);
    catalogMenu_->addAction(regenerateAct_);
    catalogMenu_->addAction(relabelDiskAct_);
    catalogMenu_->addAction(mergeStudiesAct_);

    utilitiesMenu_ = menuBar()->addMenu(tr("&Utilities"));
    utilitiesMenu_->addAction(exportListsAct_);
    utilitiesMenu_->addAction(exportReportFormatsAct_);
    utilitiesMenu_->addSeparator();
    utilitiesMenu_->addAction(importListsAct_);
    utilitiesMenu_->addAction(importReportFormatsAct_);
    utilitiesMenu_->addSeparator();
    utilitiesMenu_->addAction(ejectOpticalDiskAct_);

    administrationMenu_ = menuBar()->addMenu(tr("&Administration"));
    securitySubMenu_ = new QMenu(tr("Security"));
    securitySubMenu_->addAction(loginAct_);
    securitySubMenu_->addAction(logoutAct_);
    securitySubMenu_->addAction(changePasswordAct_);
    administrationMenu_->addMenu(securitySubMenu_);
    //insert Lists submenu here
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(intervalsAct_);
    administrationMenu_->addAction(columnFormatsAct_);
    administrationMenu_->addAction(protocolsAct_);
    administrationMenu_->addAction(studyConfigurationsAct_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(systemSettingsAct_);
    administrationMenu_->addAction(simulatorOptionsAct_);
    // insert reports submenu here

    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(epsimulatorHelpAct_);
    helpMenu_->addAction(aboutAct_);

    updateMenus();

}

void Navigator::saveSettings() {
    Settings settings;
    QString str;
    QTextOStream out(&str);
    out << *horizontalSplitter_;
    settings.writeEntry("/horizontalSplitter", str);
}

/**
 * Read the current settings, including location of the horizontalSplitter_.
 * This will also read the last "disk" used if optical disk emulation is
 * on.
 */
void Navigator::readSettings() {
    Settings settings;
    QString str = settings.readEntry("/horizontalSplitter");
    QTextIStream in(&str);
    in >> *horizontalSplitter_;
}

void Navigator::processFilter() {
        QRegExp lastNameRegExp(filterCatalogDialog_->lastNameFilter(), 
            false, true);
	QRegExp firstNameRegExp(filterCatalogDialog_->firstNameFilter(), 
            false, true);
	QRegExp mrnRegExp(filterCatalogDialog_->mrnFilter(), false, true);
	QRegExp studyConfigRegExp(filterCatalogDialog_->studyConfigFilter(),
            false, true);
	QRegExp studyNumberRegExp(filterCatalogDialog_->studyNumberFilter(),
            false, true);
	QRegExp studyLocationRegExp(filterCatalogDialog_->studyLocationFilter(),
            false, true);
	// date stuff next
	QDate today = QDate::currentDate();
	QDate startDate = today, endDate = today;
	bool anyDate = false;
	switch (filterCatalogDialog_->dateFilter()) {
	    case FilterCatalogDialog::AnyDate : 
		anyDate = true;
		break;
            // today, default settings are true
	    case FilterCatalogDialog::Today : 
		break;
            // last week's studies
	    case FilterCatalogDialog::LastWeek : 
		startDate = endDate.addDays(-7);
		break; 
            // specific dates selected
	    case FilterCatalogDialog::SpecificDates :   
		startDate = filterCatalogDialog_->beginDate();
		endDate = filterCatalogDialog_->endDate();
		break;
	}	
        TableListView::FilterStudyType filterStudyType =
            filterCatalogDialog_->filterStudyType();
 
        tableListView_->applyFilter(filterStudyType, lastNameRegExp,
            firstNameRegExp, mrnRegExp, studyConfigRegExp, 
            studyNumberRegExp, studyLocationRegExp, 
            anyDate, startDate, endDate);
        statusBar_->updateFilterLabel(true);
        filterStudiesAct_->setEnabled(false);
        removeStudiesFilterAct_->setEnabled(true);
        // only disallow regenerating when catalog is filtered
        /// TODO This may not be necessary.  
        /// Catalog will be refreshed automatically
        /// after it is regenerated.
        regenerateAct_->setEnabled(false);
}

void Navigator::startStudy(Study* s) {
    // write study files
    QString studiesPath = currentDisk_->studiesPath();
    QDir studiesDir(studiesPath);
    if (!studiesDir.exists()) {
        if (!studiesDir.mkdir(studiesPath))
            throw EpSim::IoError(studiesPath, "could not create studiesPath");
    }
    // create study directory and write study.dat file
    QString studyPath = studiesPath + s->studyDirName();
    QDir studyDir(studyPath);
    if (!studyDir.exists()) {
        if (!studyDir.mkdir(studyPath))
            throw EpSim::IoError(studyPath, "could not create studyPath");
    }
    s->setPath(studyPath);
    s->save();
    // recorder saves pointer to navigator and unhides it when
    // it "closes" (actually it hides itself too)
    // We also reuse the same recorder all the time, 
    // but use lazy initialization
    Recorder *recorder = getRecorder();
    recorder->setStudy(s);
    recorder->showNormal();
    recorder->showMaximized();
    //recorder->setStudy(s);
    // looks better to show new window first, then hide this one,
    // and vice versa
    hide();

}

void Navigator::reviewStudy(Study* s) {
    QMessageBox::information(this, tr("Starting Review Simulation"),
        tr("The Review simulation is not implemented yet.\n"
           "Will return to Navigator."));
    delete s;
}

void Navigator::reports(Study* s) {
    QMessageBox::information(this, tr("Starting Report Simulation"),
        tr("The Report simulation is not implemented yet.\n"
           "Will return to Navigator."));
    delete s;
}

// returns true if PatientDialog is saved, false if cancelled
bool Navigator::getStudyInformation(Study* study) {
    PatientDialog* patientDialog = new PatientDialog(this);
    patientDialog->setFields(study);
    if (patientDialog->exec() == QDialog::Accepted) {
        patientDialog->getFields(study);
        delete patientDialog;
        return true;
    }
    delete patientDialog;
    return false;
}

/// This returns a pointer to a study selected from the catalog, 
/// returns 0 if no study selected
Study* Navigator::getSelectedStudy() {
    return tableListView_->study();
}

/// Returns study selected in the catalog, or, if none selected, a new study.
Study* Navigator::getNewStudy() {
    Study* study = getSelectedStudy();
    if (study) {
        // A new study must have a current date time.
        study->setDateTime(QDateTime::currentDateTime());
        // study number will be set to blank also.
        study->setNumber(QString());
        // need a new key for a new study.
        study->resetKey(); 
        return study;
    }
    else
        // Current date time already set with new study.
        return new Study;
}

Recorder* Navigator::getRecorder() {
    if (!recorder_)
        recorder_ = new Recorder(this);
    return recorder_;
}

QString Navigator::studyPath(const Study* study) const {
    return QDir::cleanDirPath(currentDisk_->fullPath() + "/" + study->key());
}

void Navigator::deleteDataFiles(const QString& path) {
    if (!options_->permanentDelete()) {
#ifndef NDEBUG
        std::cerr << "Path is " << path.toAscii().constData()
            << std::endl;
#endif
        return;
    }
    try {
        QDir d(path);
        if (!d.exists())
            throw EpSim::FileNotFoundError(path);
        QString item;
        QFileInfoList list = d.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            d.remove(fileInfo.fileName());
        }
        d.rmdir(path);
    }
    catch (EpSim::FileNotFoundError& e) {
        throw e;
    }
    catch (EpSim::IoError&) {
        throw EpSim::DeleteError();
    }
}

/// This checks to make sure the selected study is on the optical disk catalog.
/// If something is wrong with the catalog and the study is not physically present
/// on disk, despite being in the catalog, the actual disk processing should raise
/// and exception.
bool Navigator::studyOnDisk(const Study* s) const {
    if (catalogs_->currentCatalogIsOptical())
        return true; 
        // by definition if you are selecting from the optical catalog
        // the study is there.
    if (catalogs_->studyPresentOnOpticalDisk(s))
        return true;
    return false;
}

// Below reports the error and changes the disk.  Non-const function
// since disk is changed.
void Navigator::studyNotOnDiskError() {
 
    ejectDisk();
}


Navigator::~Navigator() {
    saveSettings();
    delete catalogs_;
    delete currentDisk_;
    user_->destroy();
}

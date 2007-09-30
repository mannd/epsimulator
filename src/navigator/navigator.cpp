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

#include "buttonframe.h"
#include "catalog.h"
#include "changepassworddialog.h"
#include "disklabeldialog.h"
#include "catalogcombobox.h"
#include "error.h"
#include "filtercatalog.h"
#include "opticaldisk.h"
#include "options.h"
#include "navigator.h"
#include "patientdialog.h"
#include "passworddialog.h"
#include "recorder.h"
#include "simulatorsettingsdialog.h"
#include "settings.h"
#include "statusbar.h"
#include "study.h"
#include "studyconfigdialog.h"
#include "studycopywizard.h"
#include "systemdialog.h"
#include "tablelistview.h"
#include "user.h"
#include "utilities.h"
#include "versioninfo.h"

#include <qaction.h>
#include <qapplication.h>
#include <qdatetime.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qregexp.h>
#include <qsplitter.h>
#include <qtoolbar.h>

#include <algorithm>

#ifndef NDEBUG
#include <iostream> // for debugging
#endif

/**
 * Navigator constructor
 */
Navigator::Navigator(QWidget* parent, const char* name)
    : QMainWindow( parent, name, WDestructiveClose ),
                   options_(Options::instance()), filterCatalog_(0),
                   catalogs_(0), currentDisk_(0), user_(User::instance()) {
    do {
        createOpticalDrive();
    } while (!currentDisk_);
    catalogs_ = new Catalogs(options_, currentDisk_->fullPath());
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createCentralWidget();

    connect(catalogComboBox_, SIGNAL(activated(int)),
        this, SLOT(changeCatalog()));

    setCaption(tr("%1 Navigator").arg(VersionInfo::instance()->programName()));
    setIcon(QPixmap::fromMimeSource("hi32-app-epsimulator.png"));
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
        StudyConfigDialog* studyConfigDialog  = new StudyConfigDialog(this);
        if (studyConfigDialog->exec() == QDialog::Accepted) {
            study->setConfig(studyConfigDialog->config());
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
        delete studyConfigDialog;
        delete study;
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
        StudyConfigDialog* studyConfigDialog  = new StudyConfigDialog(this);
        if (studyConfigDialog->exec() == QDialog::Accepted) {
            study->setConfig(studyConfigDialog->config());
            catalogs_->deleteStudy(study);
            catalogs_->addStudy(study, currentDisk_->label(),
                    currentDisk_->translatedSide(),
                    options_->labName(), user_->machineName());
            refreshCatalogs();
            startStudy(study);
        }
        delete studyConfigDialog;
    }
    else if (!studyOnDisk(study)) 
        studyNotOnDiskError();
    else
        startStudy(study);
    delete study;
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
    delete study;
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
    delete study;
}

void Navigator::copyStudy() {
    StudyCopyWizard* wizard = new StudyCopyWizard(this);
    wizard->setSourcePathName(currentDisk_->fullPath());
    if (wizard->exec()) {
        OpticalDisk* disk = new OpticalDisk(wizard->destinationPathName());
        disk->readLabel();
        if (!disk->isLabeled())
            labelDisk(false, disk);
        Catalog* catalog = new Catalog(wizard->destinationPathName());
        wizard->copy();
        // for each study in studiesList
        //		copy study form source folder to destination folder
        // 		throw something if any copy fails
        // after successful copying, don't update catalog
        // make a catalog.dat file in the destination folder
        // data on source is not erased
        ;
        delete catalog;
        delete disk;
    }
    delete wizard;
}

void Navigator::moveStudy() {
    if (administrationAllowed()) {
        StudyMoveWizard* wizard = new StudyMoveWizard(this);
        wizard->setSourcePathName(currentDisk_->fullPath());
        if (wizard->exec()) {
            OpticalDisk* disk = new OpticalDisk(wizard->destinationPathName());
            disk->readLabel();
            if (!disk->isLabeled())
                labelDisk(false, disk);
            Catalog* catalog = new Catalog(wizard->destinationPathName());
            wizard->move();
        // for each study in studiesList
        //		copy study form source folder to destination folder
        // 		throw something if any copy fails
        // after successful copying, 
        // make a catalog.dat file in the destination folder
        // now update system catalogs
        // now erase data on source
            ;
            delete catalog;
            delete disk;
        }
        delete wizard;
    }
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
    if (!filterCatalog_)
        filterCatalog_ = new FilterCatalog(this);
    if (filterCatalog_->exec() == QDialog::Accepted) 
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
        PasswordDialog* pwDialog = new PasswordDialog(options_,this);
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
    QFileDialog *fd = new QFileDialog(options_->systemCatalogPath(), 
                                      Catalog::defaultFileName(), this, 0, true);
    if (fd->exec() == QDialog::Accepted) {
        catalogs_->setCatalogPath(Catalog::Other, fd->dirPath());
        catalogs_->refresh();   // to reload Other catalog
        catalogComboBox_->setSource(Catalog::Other);
        changeCatalog();
    }
    delete fd;
}

void Navigator::exportCatalog() {
    QFileDialog *fd = new QFileDialog(QDir::homeDirPath(), "Comma-delimited (*.csv)", this, 0, true);
    fd->setMode(QFileDialog::AnyFile);
    if (fd->exec() == QDialog::Accepted) {
        QString fileName = fd->selectedFile();
        if (!fileName.isEmpty()) {
            int ret = 0;
            if (QFile::exists(fileName))
                ret = QMessageBox::warning(this,
                                           tr("Overwrite File"),
                                           tr("Overwrite\n\'%1\'?").
                                                arg(fileName),
                                           tr("&Yes"), tr("&No"),
                                           QString::null, 1, 1);
            if (ret == 0) {
                try {
                    tableListView_->exportCSV(fd->selectedFile());
                }
                catch (EpSim::IoError& e) {
                    std::cerr << e.what() << std::endl;
                    QMessageBox::warning(this, tr("Error"),
                        tr("Could not export Catalog to %1").arg(e.fileName()));
                }
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
    actions_.help(this);
}

void Navigator::about() {
    actions_.about(this);
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
            currentDisk_ = EmulatedOpticalDisk::getLastDisk(options_->opticalStudyPath());
            if (!currentDisk_) {
                currentDisk_ = new EmulatedOpticalDisk(options_->opticalStudyPath(),
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
            QFileDialog *fd = new QFileDialog(this, 0, true);
            fd->setMode(QFileDialog::Directory);
            if (fd->exec() == QDialog::Accepted) {
                options_->setOpticalStudyPath(fd->selectedFile());
                options_->writeSettings();
            }
        }
    }
}

void Navigator::createCentralWidget() {
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
    horizontalSplitter_ = new QSplitter(Horizontal, this);
    setCentralWidget(horizontalSplitter_);

    ButtonFrame* buttonFrame = new ButtonFrame(horizontalSplitter_);

    buttonFrame->addButton("New Study", "hi64-newstudy.png", SLOT(newStudy()));
    buttonFrame->addButton("Continue Study", "hi64-continuestudy.png", SLOT(continueStudy()));
    buttonFrame->addButton("Review Study", "hi64-reviewstudy.png", SLOT(reviewStudy()));
    buttonFrame->addButton("Pre-Register", "hi64-preregister.png", SLOT(preregisterPatient()));
    buttonFrame->addButton("Reports", "hi64-reports.png", SLOT(reports()), true); 
}

/** @brief Creates the TableListView object.
 *
 * The tableListView_ contains the list of studies, and reflects whatever
 * source is current in the catalogComboBox_.
 */
void Navigator::createTableListView() {
    tableListView_ = new TableListView(horizontalSplitter_, options_->oldStyleNavigator());
    connect(tableListView_, SIGNAL(doubleClicked(QListViewItem*, 
        const QPoint&, int)), this, SLOT(newStudy()));
}

void Navigator::createStatusBar() {
    statusBar_ = new StatusBar(catalogs_->currentCatalog()->path(), this, 
        "StatusBar");
    updateStatusBarUserLabel();
}

void Navigator::updateMenus() {
    bool showSimulatorSettings = !options_->hideSimulatorMenu() ||
        user_->isAdministrator();
    simulatorOptionsAct_->setVisible(showSimulatorSettings);
}

/**
 * Sets up icon, status tip, and slot for an action.
 */
void Navigator::setupAction(QAction* action, QString statusTip, 
                            const char* slotName, const char* iconName) {
    if (iconName)
        action->setIconSet(QPixmap::fromMimeSource(iconName));
    action->setStatusTip(tr(statusTip));
    if (slotName)
        connect(action, SIGNAL(activated()), this, slotName);
} 

void Navigator::createActions() {
    // Study menu
    newAct_ = new QAction(tr("&New..."), tr("Ctrl+N"), this);
    setupAction(newAct_, "New study", SLOT(newStudy()), "hi32-newstudy.png");
    continueAct_ = new QAction(tr("&Continue"), 0, this);
    setupAction(continueAct_, "Continue study", SLOT(continueStudy()), 
                "hi32-continuestudy.png");
    reviewAct_ = new QAction(tr("&Review"), 0, this);
    setupAction(reviewAct_, "Review study", SLOT(reviewStudy()), 
                "hi32-reviewstudy.png");
    preregisterAct_= new QAction(tr("&Pre-Register"), 0, this);
    setupAction(preregisterAct_, "Pre-register patient", 
        SLOT(preregisterPatient()), "hi32-preregister.png");
    reportsAct_= new QAction(tr("R&eports..."), 0, this);
    setupAction(reportsAct_, "Procedure reports", SLOT(reports()), 
                "hi32-reports.png" );
    copyAct_= new QAction(tr("Copy..."), 0, this);
    setupAction(copyAct_, "Copy study", SLOT(copyStudy()));
    moveAct_ = new QAction(tr("Move..."), 0, this);
    setupAction(moveAct_, "Move study", SLOT(moveStudy()));
    deleteAct_= new QAction(tr("Delete..."), 0, this);
    setupAction(deleteAct_, "Delete study", SLOT(deleteStudy()));
    exportAct_ = new QAction(tr("Export..."), 0, this);
    setupAction(exportAct_, "Export study", SLOT(exportCatalog()), "hi32-exportstudy.png");
    exitAct_= new QAction(tr("E&xit"), tr("Ctrl+Q"), this);
    setupAction(exitAct_, "Exit EP Simulator", SLOT(close()));

    // Catalog menu
    // Submenu of Switch...
    // an action "Achive Server" is skipped here, but is present on Prucka
    networkSwitchAct_ = new QAction(tr("Network"), 0, this);
    setupAction(networkSwitchAct_, "Switch to network catalog", 
        SLOT(setCatalogNetwork()));
    // networkSwitchAct_ only enabled if set in options
    networkSwitchAct_->setEnabled(options_->enableNetworkStorage());
    systemSwitchAct_ = new QAction(tr("System"), 0, this);
    setupAction(systemSwitchAct_, "Switch to system catalog", 
        SLOT(setCatalogSystem()));
    opticalSwitchAct_ = new QAction(tr("Optical"), 0, this);
    setupAction(opticalSwitchAct_, "Switch to optical catalog", 
        SLOT(setCatalogOptical()));
    // back to main menu items
    browseSwitchAct_ = new QAction(tr("Browse..."), 0, this);
    setupAction(browseSwitchAct_, "Browse for catalog files", 
        SLOT(setCatalogOther()));
    filterStudiesAct_ = new QAction(tr("Filter Studies..."), 0, this);
    setupAction(filterStudiesAct_, "Filter studies", 
        SLOT(filterStudies()), "hi32-filterstudies.png");
    removeStudiesFilterAct_ = new QAction(tr("Remove Studies Filter"), 0, this);
    setupAction(removeStudiesFilterAct_, "Remove studies filter",
	SLOT(unfilterStudies()), "hi32-removefilter.png");
    // inactivate removeStudiesFilterAct_ by default
    removeStudiesFilterAct_->setEnabled(false);
    refreshViewAct_ = new QAction(tr("Refresh"), 0, this);
    setupAction(refreshViewAct_, "Refresh the catalog", 
        SLOT(refreshCatalogs()), "hi32-refreshcatalog.png");
    regenerateAct_ = new QAction(tr("Regenerate"), 0, this);
    setupAction(regenerateAct_, "Regenerate the catalog",
	SLOT(regenerateCatalogs()));
    relabelDiskAct_ = new QAction(tr("Re-Label Disk..."), 0 ,this);
    setupAction(relabelDiskAct_, "Re-label the optical disk", SLOT(relabelDisk()));
    mergeStudiesAct_ = new QAction(tr("Merge Studies..."), 0, this);
    setupAction(mergeStudiesAct_, "Merge studies together", 0);

    // Utilities menu
    exportListsAct_ = new QAction(tr("Export Lists..."), 0, this);
    setupAction(exportListsAct_, "Export lists", 0);
    exportReportFormatsAct_ = new QAction(tr("Export Report Formats..."), 0, this);
    setupAction(exportReportFormatsAct_, "Export report formats", 0);
    importListsAct_ = new QAction(tr("Import Lists..."), 0, this);
    setupAction(importListsAct_, "Import lists", 0);
    importReportFormatsAct_ = new QAction(tr("Import Report Formats..."), 0, this);
    setupAction(importReportFormatsAct_, "Import report formats", 0);
    ejectOpticalDiskAct_ = new QAction(tr("Eject Optical Disk"), 0, this);
    setupAction(ejectOpticalDiskAct_, "Eject optical disk", SLOT(ejectDisk()));

    // Administration menu
    loginAct_= new QAction(tr("Login..."), 0, this);
    setupAction(loginAct_, "Login as administrator", SLOT(login()));
    logoutAct_= new QAction(tr("Logout"), 0, this);
    setupAction(logoutAct_, "Logout from administrator", SLOT(logout()));
    changePasswordAct_= new QAction(tr("Change Password..."), 0, this);
    setupAction(changePasswordAct_, "Change administrator password", SLOT(changePassword()));
    intervalsAct_= new QAction(tr("Intervals"), 0, this);
    setupAction(intervalsAct_, "Intervals", SLOT(setIntervals()));
    columnFormatsAct_= new QAction(tr("Column Formats"), 0, this);
    setupAction(columnFormatsAct_, "Column formats", SLOT(setColumnFormats()));
    protocolsAct_= new QAction(tr("Protocols"), 0, this);
    setupAction(protocolsAct_, "Protocols", SLOT(setProtocols()));
    studyConfigurationsAct_= new QAction(tr("Study Configurations"), 0, this);
    setupAction(studyConfigurationsAct_, "Study configurations", SLOT(setStudyConfigurations()));
    systemSettingsAct_= new QAction(tr("System Settings"), 0, this);
    setupAction(systemSettingsAct_, "Change system settings",
                SLOT(systemSettings()));
    simulatorOptionsAct_ = new QAction(tr("*Simulator Settings*"), 0, this);
    setupAction(simulatorOptionsAct_, "Change simulator settings", 
                SLOT(simulatorSettings()));

    // Help menu
    epsimulatorHelpAct_ = new QAction(tr("EP Simulator Help..."), tr("F1"), this);
    setupAction(epsimulatorHelpAct_, "Get help for EP Simulator", SLOT(help()));
    aboutAct_= new QAction(tr("&About EP Simulator"), 0, this);
    setupAction(aboutAct_, "About EP Simulator", SLOT(about()));
}

void Navigator::createToolBars() {
    navigatorToolBar_ = new QToolBar(tr("Navigator"), this);
    catalogComboBox_ = new CatalogComboBox(navigatorToolBar_, "catalogComboBox");
    navigatorToolBar_->addSeparator();
    filterStudiesAct_->addTo(navigatorToolBar_);
    removeStudiesFilterAct_->addTo(navigatorToolBar_);
    navigatorToolBar_->addSeparator();
    refreshViewAct_->addTo(navigatorToolBar_);
    navigatorToolBar_->addSeparator();
    exportAct_->addTo(navigatorToolBar_);
}

void Navigator::createMenus() {

    studyMenu_ = new QPopupMenu(this);
    newAct_->addTo(studyMenu_);
    continueAct_->addTo(studyMenu_);
    reviewAct_->addTo(studyMenu_);
    preregisterAct_->addTo(studyMenu_);
    reportsAct_->addTo(studyMenu_);
    studyMenu_->insertSeparator();
    copyAct_->addTo(studyMenu_);
    moveAct_->addTo(studyMenu_);
    deleteAct_->addTo(studyMenu_);
    exportAct_->addTo(studyMenu_);
    studyMenu_->insertSeparator();
    exitAct_->addTo(studyMenu_);

    catalogMenu_ = new QPopupMenu(this);
    switchSubMenu_ = new QPopupMenu(this);
    networkSwitchAct_->addTo(switchSubMenu_);
    systemSwitchAct_->addTo(switchSubMenu_);
    opticalSwitchAct_->addTo(switchSubMenu_);
    browseSwitchAct_->addTo(switchSubMenu_);
    catalogMenu_->insertItem(tr("Switch"), switchSubMenu_);
    filterStudiesAct_->addTo(catalogMenu_);
    removeStudiesFilterAct_->addTo(catalogMenu_);
    catalogMenu_->insertSeparator();
    refreshViewAct_->addTo(catalogMenu_);
    regenerateAct_->addTo(catalogMenu_);
    relabelDiskAct_->addTo(catalogMenu_);
    mergeStudiesAct_->addTo(catalogMenu_);

    utilitiesMenu_ = new QPopupMenu(this);
    exportListsAct_->addTo(utilitiesMenu_);
    exportReportFormatsAct_->addTo(utilitiesMenu_);
    utilitiesMenu_->insertSeparator();
    importListsAct_->addTo(utilitiesMenu_);
    importReportFormatsAct_->addTo(utilitiesMenu_);
    utilitiesMenu_->insertSeparator();
    ejectOpticalDiskAct_->addTo(utilitiesMenu_);

    administrationMenu_ = new QPopupMenu(this);
    securitySubMenu_ = new QPopupMenu(this);
    loginAct_->addTo(securitySubMenu_);
    logoutAct_->addTo(securitySubMenu_);
    changePasswordAct_->addTo(securitySubMenu_);
    administrationMenu_->insertItem(tr("Security"), securitySubMenu_);
    //insert Lists submenu here
    administrationMenu_->insertSeparator();
    intervalsAct_->addTo(administrationMenu_);
    columnFormatsAct_->addTo(administrationMenu_);
    protocolsAct_->addTo(administrationMenu_);
    studyConfigurationsAct_->addTo(administrationMenu_);
    administrationMenu_->insertSeparator();
    systemSettingsAct_->addTo(administrationMenu_);
    simulatorOptionsAct_->addTo(administrationMenu_);
    // insert reports submenu here

    helpMenu_ = new QPopupMenu(this);
    epsimulatorHelpAct_->addTo(helpMenu_);
    aboutAct_->addTo(helpMenu_);

    menuBar()->insertItem(tr("&Study"), studyMenu_);
    menuBar()->insertItem(tr("&Catalog"), catalogMenu_);
    menuBar()->insertItem(tr("&Utilities"), utilitiesMenu_);
    menuBar()->insertItem(tr("&Administration"), administrationMenu_);
    menuBar()->insertItem(tr("&Help"), helpMenu_);

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
        QRegExp lastNameRegExp(filterCatalog_->lastNameFilter(), false, true);
	QRegExp firstNameRegExp(filterCatalog_->firstNameFilter(), false, true);
	QRegExp mrnRegExp(filterCatalog_->mrnFilter(), false, true);
	QRegExp studyConfigRegExp(filterCatalog_->studyConfigFilter(), false, true);
	QRegExp studyNumberRegExp(filterCatalog_->studyNumberFilter(), false, true);
	QRegExp studyLocationRegExp(filterCatalog_->studyLocationFilter(), false, true);
	// date stuff next
	QDate today = QDate::currentDate();
	QDate startDate = today, endDate = today;
	bool anyDate = false;
	switch (filterCatalog_->dateFilter()) {
	    case FilterCatalog::AnyDate : 
		anyDate = true;
		break;

	    case FilterCatalog::Today : // today, default settings are true
		break;

	    case FilterCatalog::LastWeek : 
		startDate = endDate.addDays(-7);
		break; // i.e. last week's studies

	    case FilterCatalog::SpecificDates :   // specific dates selected
		startDate = filterCatalog_->beginDate();
		endDate = filterCatalog_->endDate();
		break;
	}	
        TableListView::FilterStudyType filterStudyType = filterCatalog_->filterStudyType();
 
        tableListView_->applyFilter(filterStudyType, lastNameRegExp,
            firstNameRegExp, mrnRegExp, studyConfigRegExp, 
            studyNumberRegExp, studyLocationRegExp, 
            anyDate, startDate, endDate);
        statusBar_->updateFilterLabel(true);
        filterStudiesAct_->setEnabled(false);
        removeStudiesFilterAct_->setEnabled(true);
        // only disallow regenerating when catalog is filtered
        /// TODO This may not be necessary.  Catalog will be refreshed automatically
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
    Recorder *recorder = new Recorder();
    qApp->setMainWidget(recorder);
    // Below is a work-around as showMaximized() alone doesn't always work.
    recorder->showNormal();
    recorder->showMaximized();
    //delete this;   
}

void Navigator::reviewStudy(Study* s) {
    s = 0;  /// TODO only to avoid compiler warning now, s will be passed
            /// to Review module.
    QMessageBox::information(this, tr("Starting Review Simulation"),
        tr("The Review simulation is not implemented yet.\n"
           "Will return to Navigator."));
}

void Navigator::reports(Study* s) {
    s = 0;
    QMessageBox::information(this, tr("Starting Report Simulation"),
        tr("The Report simulation is not implemented yet.\n"
           "Will return to Navigator."));
}

// returns true if PatientDialog is saved, false if cancelled
bool Navigator::getStudyInformation(Study* study) {
    PatientDialog* patientDialog = new PatientDialog(this);
    patientDialog->setFields(study);
    if (patientDialog->exec() == QDialog::Accepted) {
        patientDialog->getFields(study);
        return true;
    }
    return false;
}

/// This returns a pointer to a study selected from the catalog, 
/// returns 0 if no study selected
Study* Navigator::getSelectedStudy() {
    return tableListView_->study();
}

/// Returns study selected in the catalog, or, if none selected, a new study.
Study* Navigator::getNewStudy() {
    Study* study= getSelectedStudy();
    if (study) {
        // A new study must have a current date time.
        study->setDateTime(QDateTime::currentDateTime());
        // study number will be set to blank also.
        study->setNumber(QString::null);
        // need a new key for a new study.
        study->resetKey(); 
        return study;
    }
    else
        // Current date time already set with new study.
        return new Study;
}

QString Navigator::studyPath(const Study* study) const {
    return QDir::cleanDirPath(currentDisk_->fullPath() + "/" + study->key());
}

void Navigator::deleteDataFiles(const QString& path) {
    if (!options_->permanentDelete()) {
#ifndef NDEBUG
        std::cerr << "Path is " << path << std::endl;
#endif
        return;
    }
    try {
        QDir d(path);
        if (!d.exists())
            throw EpSim::FileNotFoundError(path);
        QString item;
        const QFileInfoList *list = d.entryInfoList();
        QFileInfoListIterator it(*list);
        QFileInfo *fi;
        
        while((fi = it.current())){
            item = fi->fileName();
            d.remove(item);
            ++it;
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
        return true; // by definition is you are selecting from the optical catalog
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

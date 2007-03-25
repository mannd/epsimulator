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
#include "epsimdefs.h"
#include "epsimulator.h"
#include "filtercatalog.h"
#include "opticaldisk.h"
#include "options.h"
#include "navigator.h"
#include "patientdialog.h"
#include "passworddialog.h"
#include "simulatorsettingsdialog.h"
#include "settings.h"
#include "statusbar.h"
#include "study.h"
#include "studyconfigdialog.h"
#include "systemdialog.h"
#include "tablelistview.h"

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
#include <stdlib.h>

#ifndef NDEBUG
#include <iostream> // for debugging
#endif

class QListViewItem;

/**
 * Navigator constructor
 */
Navigator::Navigator(QWidget* parent, const char* name)
    : QMainWindow( parent, name, WDestructiveClose ),
    options_(Options::instance()), filterCatalog_(0),
    userIsAdministrator_(false) {

    catalogs_ = new Catalogs(options_);

    currentDisk_ = new OpticalDisk(options_->opticalStudyPath());

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createCentralWidget();

    connect(catalogComboBox_, SIGNAL(activated(int)),
        this, SLOT(changeCatalog()));

    setCaption(tr("%1 Navigator").arg(PROGRAM_NAME));
    setIcon(QPixmap::fromMimeSource("hi32-app-epsimulator.png"));
}

// protected


// private slots

void Navigator::newStudy() {
    // Must have an optical disk ready to go.
    if (!currentDisk_) 
        QMessageBox::warning(this, tr("No Optical Disk"),
                             tr("Insert an optical disk and try again."));
    else {
        // study_ must be "blank" unless a study is selected in the catalog.
        // Same thing for preregister and continue study.
        prepareStudy();
        // We will leave this dialog with this name, even though it
        // probably should have been called StudyConfigSelectDialog.
        StudyConfigDialog* studyConfigDialog  = new StudyConfigDialog(this);
        if (studyConfigDialog->exec()) {
            study_.setConfig(studyConfigDialog->config());
            if (getStudyInformation()) {
                startStudy();
            }
        }
        delete studyConfigDialog;
    }
}

void Navigator::continueStudy() {
    prepareStudy();
    /// TODO Rest of processing 
}

void Navigator::reviewStudy() {
}

void Navigator::preregisterPatient() {
    prepareStudy();
    study_.setConfig("");   // preregistered study has no config info
    getStudyInformation();
    // write study to catalogs -- also called from newStudy
}

void Navigator::reports() {
}

void Navigator::deleteStudy() {
    if (tableListView_->selectedItem()) {
    //if (QListViewItem* item = tableListView_->selectedItem()) {
        int ret = QMessageBox::warning(
            this, tr("Delete Study?"),
            tr("The selected study will be permanently "
               "deleted.  Do you wish to continue?"),
            QMessageBox::Yes ,
            QMessageBox::No | QMessageBox::Default, // default is NO!
            QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes) {
            removeStudyFromCatalogs();
            // delete item;
            deleteDataFiles();
        }
    } 
    else {
        QMessageBox::information(this, tr("No Study Selected"),
            tr("You must first select a study to delete it."),
            QMessageBox::Ok);
    }
}

void Navigator::removeStudyFromCatalogs() {
}

void Navigator::filterStudies() {
    if (!filterCatalog_)
        filterCatalog_ = new FilterCatalog(this);
    if (filterCatalog_->exec()) 
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

void Navigator::refreshCatalog() {
    catalogComboBox_->refresh();
    catalogs_->setCurrentCatalog(catalogComboBox_->source());
    tableListView_->load(catalogs_->currentCatalog()->filePath());
    // reapply filter if present
    if (tableListView_->filtered())
        processFilter();
}

void Navigator::regenerateCatalog() {
    ///TODO this will read through the studies in the directory 
    /// and recreate the catalog.
    /// ? filter has to be cleared for this to work.
}

void Navigator::changeCatalog() {
    catalogs_->setCurrentCatalog(catalogComboBox_->source());
    tableListView_->load(catalogs_->currentCatalog()->filePath());
    // reapply filter if present
    if (tableListView_->filtered())
        processFilter();
    statusBar_->updateSourceLabel(catalogs_->currentCatalog()->path());
}

void Navigator::ejectDisk() {
    currentDisk_->eject();  // Currently does nothing, 
                            // is supposed to mechanically eject the disk.
    QMessageBox::information( this, tr("Eject Disk"),
    "Change Disk and select OK when done." );
    if (!currentDisk_->hasLabel())
        relabelDisk();
    
/// TODO something like below
//     if (opticalDiskDrive_->changeDisk()) {
//         //currentDisk_ = opticalDiskDrive_->loadedDisk();
}
/*
    if (opticalDiskDrive_->setup()) {
        if (opticalDiskDrive_->diskLoaded()) 
            opticalDiskDrive_->eject(currentDisk_);
        else
            currentDisk_ = opticalDiskDrive_->load();
*/

void Navigator::relabelDisk() {
    DiskLabelDialog* diskLabelDialog = new DiskLabelDialog(this);
    diskLabelDialog->setLabel(currentDisk_->label());
    diskLabelDialog->enableSideButtons(currentDisk_->isTwoSided());
    if (currentDisk_->isTwoSided()) 
        diskLabelDialog->setSide(currentDisk_->side(), tr("A"));
    if (diskLabelDialog->exec()) {
        currentDisk_->setLabel(diskLabelDialog->label());
        if (currentDisk_->isTwoSided())
            currentDisk_->setSide(tr(diskLabelDialog->side()));
    }
    delete diskLabelDialog;
}

void Navigator::login() {
    if (!userIsAdministrator_) {
        PasswordDialog* pwDialog = new PasswordDialog(options_,this);
        if (pwDialog->exec()) {
            userIsAdministrator_ = true;
            statusBar_->updateUserLabel(userIsAdministrator_);
            updateMenus();
        }
        delete pwDialog;
    }
}

void Navigator::logout() {
    userIsAdministrator_ = false;
    statusBar_->updateUserLabel(userIsAdministrator_);
    updateMenus();
}

void Navigator::changePassword() {
    ChangePasswordDialog* cpDialog = new ChangePasswordDialog(options_, this);
    if (cpDialog->exec()) {
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
    return userIsAdministrator_;
}

/// This is not for final production, just during development.
void Navigator::filler() {
    QMessageBox::information(this, tr("FYI"),
                             tr("This function is not implemented yet."));
}

void Navigator::setIntervals() {
    if (administrationAllowed())
        filler();
}

void Navigator::setColumnFormats() {
    if (administrationAllowed())
        filler();
}

void Navigator::setProtocols() {
    if (administrationAllowed())
        filler();
}

void Navigator::setStudyConfigurations() {
    if (administrationAllowed())
        filler();
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
                                      options_->catalogFileName(), this, 0, true);
    if (fd->exec() == QDialog::Accepted) {
        catalogs_->setCatalogPath(Catalog::Other, fd->dirPath());
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
            if (ret == 0)
                tableListView_->exportCSV(fd->selectedFile());
        }
    }
    delete fd;
}

void Navigator::simulatorSettings() {
    if (administrationAllowed()) {
        SimulatorSettingsDialog* simDialog = 
            new SimulatorSettingsDialog(options_, this);
        if (simDialog->exec()) {
            simDialog->setOptions();
            updateMenus();
        }
        delete simDialog;
    }
}

void Navigator::systemSettings() {
    if (administrationAllowed()) {
        SystemDialog* systemDialog = new SystemDialog(options_, this);
        if (systemDialog->exec()) {
            systemDialog->setOptions();
            // menu is changed
            networkSwitchAct_->setEnabled(options_->enableNetworkStorage());
            // status bar and catalog might be changed 
            delete catalogs_;
            catalogs_ = new Catalogs(options_);
            refreshCatalog();
            statusBar_->updateSourceLabel(catalogs_->currentCatalog()->path());
        }
        delete systemDialog;
    }
}

void Navigator::help() {
    QMessageBox::information(this, tr("%1 Help").arg(PROGRAM_NAME),
        tr("Help is available from www.epstudiossoftware.com"),
        QMessageBox::Ok);
}

void Navigator::about() {
    Epsimulator::about(this);
}

// private

void Navigator::createCentralWidget() {
    createButtonFrame();
    createTableListView();
    readSettings(); 
    refreshCatalog();
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

//    buttonFrame_ = new QFrame(horizontalSplitter_);
    ButtonFrame* buttonFrame = new ButtonFrame(horizontalSplitter_);

    buttonFrame->addButton("New Study", "hi64-newstudy.png", SLOT(newStudy()));
    buttonFrame->addButton("Continue Study", "hi64-continuestudy.png", SLOT(continueStudy()));
    buttonFrame->addButton("Review Study", "hi64-reviewstudy.png", 0);
    buttonFrame->addButton("Pre-Register", "hi64-preregister.png", SLOT(preregisterPatient()));
    buttonFrame->addButton("Reports", "hi64-reports.png", 0, true); 
}

/** @brief Creates the TableListView object.
 *
 * The tableListView_ contains the list of studies, and reflects whatever
 * source is current in the catalogComboBox_.
 */
void Navigator::createTableListView() {
    tableListView_ = new TableListView(horizontalSplitter_, options_);
    connect(tableListView_, SIGNAL(doubleClicked(QListViewItem*, 
        const QPoint&, int)), this, SLOT(newStudy()));


}

void Navigator::createStatusBar() {
    statusBar_ = new StatusBar(catalogs_->currentCatalog()->path(), this, 
        options_->oldStyleNavigator(), "StatusBar");
}

void Navigator::updateMenus() {
    bool showSimulatorSettings = !options_->hideSimulatorMenu() ||
        userIsAdministrator_;
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
    newAct = new QAction(tr("&New..."), tr("Ctrl+N"), this);
    setupAction(newAct, "New study", SLOT(newStudy()), "hi32-newstudy.png");
    continueAct = new QAction(tr("&Continue"), 0, this);
    setupAction(continueAct, "Continue study", 0, "hi32-continuestudy.png");
    reviewAct = new QAction(tr("&Review"), 0, this);
    setupAction(reviewAct, "Review study", 0, "hi32-reviewstudy.png");
    preregisterAct = new QAction(tr("&Pre-Register"), 0, this);
    setupAction(preregisterAct, "Pre-register patient", 
        SLOT(preregisterPatient()), "hi32-preregister.png");
    reportsAct = new QAction(tr("R&eports..."), 0, this);
    setupAction(reportsAct, "Procedure reports", 0, "hi32-reports.png" );
    copyAct = new QAction(tr("Copy..."), 0, this);
    setupAction(copyAct, "Copy study", 0);
    deleteAct = new QAction(tr("Delete..."), 0, this);
    setupAction(deleteAct, "Delete study", SLOT(deleteStudy()));
    exportAct_ = new QAction(tr("Export..."), 0, this);
    setupAction(exportAct_, "Export study", SLOT(exportCatalog()), "hi32-exportstudy.png");
    exitAct = new QAction(tr("E&xit"), tr("Ctrl+Q"), this);
    setupAction(exitAct, "Exit EP Simulator", SLOT(close()));

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
        SLOT(refreshCatalog()), "hi32-refreshcatalog.png");
    regenerateAct_ = new QAction(tr("Regenerate"), 0, this);
    setupAction(regenerateAct_, "Regenerate the catalog",
	SLOT(regenerateCatalog()));
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
    loginAct = new QAction(tr("Login..."), 0, this);
    setupAction(loginAct, "Login as administrator", SLOT(login()));
    logoutAct = new QAction(tr("Logout"), 0, this);
    setupAction(logoutAct, "Logout from administrator", SLOT(logout()));
    changePasswordAct = new QAction(tr("Change Password..."), 0, this);
    setupAction(changePasswordAct, "Change administrator password", SLOT(changePassword()));
    intervalsAct = new QAction(tr("Intervals"), 0, this);
    setupAction(intervalsAct, "Intervals", SLOT(setIntervals()));
    columnFormatsAct = new QAction(tr("Column Formats"), 0, this);
    setupAction(columnFormatsAct, "Column formats", SLOT(setColumnFormats()));
    protocolsAct = new QAction(tr("Protocols"), 0, this);
    setupAction(protocolsAct, "Protocols", SLOT(setProtocols()));
    studyConfigurationsAct = new QAction(tr("Study Configurations"), 0, this);
    setupAction(studyConfigurationsAct, "Study configurations", SLOT(setStudyConfigurations()));
    systemSettingsAct = new QAction(tr("System Settings"), 0, this);
    setupAction(systemSettingsAct, "Change system settings",
                SLOT(systemSettings()));
    simulatorOptionsAct_ = new QAction(tr("*Simulator Settings*"), 0, this);
    setupAction(simulatorOptionsAct_, "Change simulator settings", 
                SLOT(simulatorSettings()));

    // Help menu
    epsimulatorHelpAct_ = new QAction(tr("EP Simulator Help..."), tr("F1"), this);
    setupAction(epsimulatorHelpAct_, "Get help for EP Simulator", SLOT(help()));
    aboutAct = new QAction(tr("&About EP Simulator"), 0, this);
    setupAction(aboutAct, "About EP Simulator", SLOT(about()));
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

    studyMenu = new QPopupMenu(this);
    newAct->addTo(studyMenu);
    continueAct->addTo(studyMenu);
    reviewAct->addTo(studyMenu);
    preregisterAct->addTo(studyMenu);
    reportsAct->addTo(studyMenu);
    studyMenu->insertSeparator();
    copyAct->addTo(studyMenu);
    deleteAct->addTo(studyMenu);
    exportAct_->addTo(studyMenu);
    studyMenu->insertSeparator();
    exitAct->addTo(studyMenu);

    catalogMenu = new QPopupMenu(this);
    switchSubMenu_ = new QPopupMenu(this);
    networkSwitchAct_->addTo(switchSubMenu_);
    systemSwitchAct_->addTo(switchSubMenu_);
    opticalSwitchAct_->addTo(switchSubMenu_);
    browseSwitchAct_->addTo(switchSubMenu_);
    catalogMenu->insertItem(tr("Switch"), switchSubMenu_);
    filterStudiesAct_->addTo(catalogMenu);
    removeStudiesFilterAct_->addTo(catalogMenu);
    catalogMenu->insertSeparator();
    refreshViewAct_->addTo(catalogMenu);
    regenerateAct_->addTo(catalogMenu);
    relabelDiskAct_->addTo(catalogMenu);
    mergeStudiesAct_->addTo(catalogMenu);

    utilitiesMenu = new QPopupMenu(this);
    exportListsAct_->addTo(utilitiesMenu);
    exportReportFormatsAct_->addTo(utilitiesMenu);
    utilitiesMenu->insertSeparator();
    importListsAct_->addTo(utilitiesMenu);
    importReportFormatsAct_->addTo(utilitiesMenu);
    utilitiesMenu->insertSeparator();
    ejectOpticalDiskAct_->addTo(utilitiesMenu);

    administrationMenu = new QPopupMenu(this);
    securitySubMenu = new QPopupMenu(this);
    loginAct->addTo(securitySubMenu);
    logoutAct->addTo(securitySubMenu);
    changePasswordAct->addTo(securitySubMenu);
    administrationMenu->insertItem(tr("Security"), securitySubMenu);
    //insert Lists submenu here
    administrationMenu->insertSeparator();
    intervalsAct->addTo(administrationMenu);
    columnFormatsAct->addTo(administrationMenu);
    protocolsAct->addTo(administrationMenu);
    studyConfigurationsAct->addTo(administrationMenu);
    administrationMenu->insertSeparator();
    systemSettingsAct->addTo(administrationMenu);
    simulatorOptionsAct_->addTo(administrationMenu);
    // insert reports submenu here

    helpMenu = new QPopupMenu(this);
    epsimulatorHelpAct_->addTo(helpMenu);
    aboutAct->addTo(helpMenu);

    menuBar()->insertItem(tr("&Study"), studyMenu);
    menuBar()->insertItem(tr("&Catalog"), catalogMenu);
    menuBar()->insertItem(tr("&Utilities"), utilitiesMenu);
    menuBar()->insertItem(tr("&Administration"), administrationMenu);
    menuBar()->insertItem(tr("&Help"), helpMenu);

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
        // also disallow refreshing while filtered
//        refreshViewAct_->setEnabled(false);
// only disallow regenerating when catalog is filtered
        regenerateAct_->setEnabled(false);
}

void Navigator::startStudy() {
    // write study to catalog
    // write study files
    // load study window
    ///TODO need to pass study_ to eps
 /*   
    Epsimulator* eps = new Epsimulator(this);
    eps->showMaximized();*/
}

// returns true if PatientDialog is saved, false if cancelled
bool Navigator::getStudyInformation() {
    Study newStudy(study_);
    newStudy.setDateTime(QDateTime::currentDateTime());
    PatientDialog* patientDialog = new PatientDialog(this);
    patientDialog->setFields(newStudy);
    if (patientDialog->exec()) {
        patientDialog->getFields(newStudy);
        study_ = newStudy;
        /// FIXME this depends on the catalogComboBox
        //study_.setPath(options_->opticalStudyPath());
        //study_.setFile(study_.fileName());
        if (!study_.isPreregisterStudy()) 
            study_.setLocation(currentDisk_->label());
        /// FIXME this doesn't below here:
/*        tableListView_->addStudy(study_);
        // write the study to the catalog now in case user decides to refresh later
        tableListView_->save(catalogs_->filePaths());*/
        return true;
    }
    return false;
}

/// returns true and switches to study highlighted in the catalog; otherwise returns /// false if no study highlighted and leaves study_ unchanged
bool Navigator::studySelected() {
    if (QListViewItem* item = tableListView_->selectedItem()) {
        // must cast item to a TableListViewItem* to get study from it.
        study_ = dynamic_cast<TableListViewItem*>(item)->study();
        return true;
    }
    return false;
}

void Navigator::prepareStudy() {
    if (!studySelected()) {
        Study newStudy;
        study_ = newStudy;
    }
}

void Navigator::deleteDataFiles() {
}

Navigator::~Navigator() {
    saveSettings();
    tableListView_->save(catalogs_->filePaths());
    delete catalogs_;
    delete currentDisk_;
}

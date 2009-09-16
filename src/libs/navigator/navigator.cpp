/***************************************************************************
 *   Copyright (C) 2006 by David Mann                                      *
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

/** @file
 * Basically the navigator window is the main window.  When switching to
 * the epsimulator window, we will actually just be changing the menus
 * and the central widget
 */

#include "navigator.h"

#include "actions.h"
#include "buttonframe.h"
#include "catalogcombobox.h"
#include "disklabeldialog.h"
#include "error.h"
#include "fileutilities.h"
#include "filtercatalogdialog.h"
#include "movecopystudydialog.h"
#include "opticaldisk.h"
#include "options.h"
#include "patientdialog.h"
#include "recorder.h"
#include "selectstudyconfigdialog.h"
#include "statusbar.h"
#include "study.h"
#include "studyconfiguration.h"
#include "tablelistview.h"
#include "user.h"
#include "versioninfo.h"

#include <QAction>
#include <QCloseEvent>
#include <QDateTime>
#include <QFileDialog>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QRegExp>
#include <QSettings>
#include <QSplitter>
#include <QToolBar>
#include <QVariant>

#include <algorithm>
#include <memory>

using EpCore::Options;
using EpCore::User;
using EpCore::VersionInfo;
using EpGui::PatientDialog;
using EpStudy::Study;
using EpStudy::StudyConfiguration;
using EpNavigator::Navigator;
using EpNavigator::StatusBar;

using namespace EpHardware::EpOpticalDisk;

// note that user_ is not a Singleton, but the single instance is owned
// by Navigator, passed to Recorder.
Navigator::Navigator(QWidget* parent) : AbstractMainWindow(parent), 
                                        options_(Options::instance()),
                                        filterCatalogDialog_(0),
                                        currentDisk_(0),
                                        user_(User::instance()) {
    setAttribute(Qt::WA_DeleteOnClose);

    createDefaultDataDir();
    createOpticalDrive();
    createCatalogs();    
    createActions();
    createMenus();
    createToolBars();
    createCentralWidget();
    createStatusBar();

    // NB: activated(), not currentIndexChanged() is required here,
    // as currentIndexChanged() is sent when the combobox is 
    // programmatically changed as well as changed by the user and
    // that will cause duplicate entries in the combobox when the
    // Other catalog is selected.
    connect(catalogComboBox_, SIGNAL(activated(int)),
        this, SLOT(changeCatalog()));

    updateWindowTitle();
    //readSettings();
}

Navigator::~Navigator() {
    delete catalogs_;
    delete currentDisk_;
    delete user_;
    Options::destroy();
    VersionInfo::destroy();
}

// protected

void Navigator::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

// private slots

bool Navigator::acquisitionIsEnabled() {
    if (!options_->filePathFlags.testFlag(Options::EnableAcquisition)) {
        QMessageBox::information(this, tr("Acquisition Not Enabled"),
            tr("This workstation is not set up for acquisition. "
            "Select the System Settings menu item to enable acquisition."));
        return false;
    }
    return true;
}


// Blue bar actions
void Navigator::newStudy() {
    if (!acquisitionIsEnabled())
	return;
    if (!currentDisk_->isLabeled()) 
        relabelDisk();
    // if after all the above we finally have a label...
    if (currentDisk_->isLabeled()) {
        Study* study = getNewStudy();
        SelectStudyConfigDialog* selectStudyConfigDialog  =
            new SelectStudyConfigDialog(this);
        if (selectStudyConfigDialog->exec() == QDialog::Accepted) {
            study->setConfig(selectStudyConfigDialog->config().name());
            if (getStudyInformation(study)) {
                catalogs_->addStudy(study, currentDisk_->label(),
                                    currentDisk_->translatedSide(),
                                    options_->labName, user_->machineName());
                refreshCatalogs();
                startStudy(study);
            }
        }
        delete selectStudyConfigDialog;
    }
}

void Navigator::continueStudy() {
    if (!acquisitionIsEnabled())
	return;
    if (!options_->filePathFlags.testFlag(Options::EnableAcquisition)) {
        QMessageBox::information(this, tr("Acquisition Not Enabled"),
            tr("This workstation is not set up for acquisition. "
            "Select the System Settings menu item to enable acquisition."));
        return;
    }
    Study* study = getSelectedStudy();
    if (!study) {
        noStudySelectedError();
        return;
    }
    if (study->isPreregisterStudy()) {
        SelectStudyConfigDialog* selectStudyConfigDialog  = 
            new SelectStudyConfigDialog(this);
        if (selectStudyConfigDialog->exec() == QDialog::Accepted) {
            study->setConfig(selectStudyConfigDialog->config().name());
            catalogs_->deleteStudy(study);
            catalogs_->addStudy(study, currentDisk_->label(),
                    currentDisk_->translatedSide(),
                    options_->labName, user_->machineName());
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

void Navigator::moveCopyStudyMessageBox(bool move) {
    QString typeOfCopy = move ? tr("move") : tr("copy");
    QString uCaseTypeOfCopy = move ? tr("Move") : tr("Copy");
    QMessageBox::information(this, tr("Study %1 Wizard").arg(uCaseTypeOfCopy),
        tr("This wizard will enable you to %1 patient studies" 
           " from one location to another.  You will need to provide"
           " the location of the source and destination"
           " folders to complete the %1.").arg(typeOfCopy));
}

void Navigator::doStudyCopy(MoveCopyStudyDialog& dialog, bool move) {
    if (dialog.exec()) {
        // handle badness first: source and destination can't be the same
        // UNLESS they both are the optical disk.
        if (!currentDisk_->isOpticalDiskPath(dialog.sourcePath()) &&
            dialog.destinationPath() == dialog.sourcePath())
            throw EpCore::SourceDestinationSameError(dialog.sourcePath());
        // copy studies to temp dir, pretend it is an optical disk
        QList<QString> list = dialog.selectedItems();
        QDir tmpDir = QDir::temp(); 
        // delete the old versions in tmp, otherwise copy will not copy
        EpCore::deleteDir(OpticalDisk::makeStudiesPath(tmpDir.absolutePath()));
        if (!tmpDir.mkdir(OpticalDisk::studiesDirName()))
            throw EpCore::IoError();
        tmpDir.cd(OpticalDisk::studiesDirName());
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            //EpCore::deleteDir(destinationPath);
            EpCore::copyDir(fileInfo.filePath(), tmpDir.absolutePath()); 
        }
        // if source and destination are optical disks, 
        // eject disk and check labels
        if (currentDisk_->isOpticalDiskPath(dialog.destinationPath())
            && dialog.destinationPath() == dialog.sourcePath()) {
            QString sourceLabel = currentDisk_->label(); 
            ejectDisk();
            // if disk isn't labelled, label it
            if (!currentDisk_->isLabeled())
                labelDisk(false, currentDisk_);
            // if labels the same, throw sourcedestinationsameerror
            if (sourceLabel == currentDisk_->label())
                throw EpCore::SourceDestinationSameError(dialog.sourcePath());
            // now copy from the tmp dir to the destination 
            // (optical disk or hard drive)
            EpCore::copyDir(tmpDir.absolutePath(), currentDisk_->labelPath());
            // make/update catalog on destination, don't update system catalogs
            OpticalCatalog c(currentDisk_->labelPath());
            // move is exactly the same, except update system catalogs
            if (move)
                regenerateCatalogs();
            else
                c.create(currentDisk_->label(), currentDisk_->side(), 
                          options_->labName, user_->machineName());
        }
        else {    // we are copying from disk or dir to dir
            EpCore::copyDir(tmpDir.absolutePath(), dialog.destinationPath());
            OpticalCatalog c(dialog.destinationPath());
            c.create(dialog.destinationPath(), QString(), options_->labName,
                user_->machineName());   
        }
    }
}

void Navigator::copyStudy(bool move) {
    try {    
        moveCopyStudyMessageBox(move);
        if (move) {
            MoveStudyDialog dialog(this, currentDisk_);
            doStudyCopy(dialog, move);
        }
        else {
            CopyStudyDialog dialog(this, currentDisk_);
            doStudyCopy(dialog, move);
        }
    }
    catch (EpCore::SourceDestinationSameError& e) {    
        QMessageBox::warning(this, 
            tr("Source and destination directories are the same"),
            tr("Source and destination directories must be different"));
    }
    catch (EpCore::IoError& e) {
        QMessageBox::warning(this,
            tr("Error copying study"),
            tr("Study could not be copied"));
    }    
}

void Navigator::copyStudy() {
    copyStudy(false);
}

void Navigator::moveStudy() {
    copyStudy(true);
}

void Navigator::deleteStudy() {
    Study* study = getSelectedStudy();
    try {
        if (study) {
            if (!study->isPreregisterStudy() 
                && !catalogs_->studyPresentOnOpticalDisk(study)) {
                QMessageBox::information(this, 
                    tr("Study Not On Optical Disk"),
                    tr("Insert the disk containing this study "
                    "and try again."));
                delete study;
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
                if (!study->isPreregisterStudy() 
                    && options_->permanentDelete)
                    EpCore::deleteDir(currentDisk_->studiesPath() 
                        + study->dirName());
            }
            delete study;
        } 
        else 
            noStudySelectedError();
    }
    catch (EpCore::FileNotFoundError& e) {
        QMessageBox::warning(this, tr("Problem Deleting Study"),
            tr("Could not find study file %1").arg(e.fileName()));
        delete study;
    }
    catch (EpCore::DeleteError&) {
        QMessageBox::warning(this, tr("Problem Deleting Study"),
            tr("Errors occurred while trying to delete study data."));
        delete study;
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
    removeStudiesFilterAction_->setEnabled(false);
    filterStudiesAction_->setEnabled(true);
//    refreshViewAction_->setEnabled(true);
    regenerateAction_->setEnabled(true);
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
                          options_->labName, user_->machineName());
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
    createCatalogs();
    refreshCatalogs();
    statusBar_->updateSourceLabel(catalogs_->currentCatalog()->path());
}

/**
 * When the program starts for the first time, there needs to be a default
 * location for data storage. This function creates a "MyStudies"
 * directory in the user home directory for this purpose,
 * unless it already exists. It will not overwrite it if the
 * directory already exists.
 * 
 * TODO An alternative design would be to 
 * defer this until an optical disk is chosen, in other words, default
 * to no optical disk until studies are created.
 */
void Navigator::createDefaultDataDir() {
    QString defaultDataDirName = "MyStudies";
    if (!QDir::home().exists(defaultDataDirName)) {
        if (!QDir::home().mkdir(defaultDataDirName)) {
            QMessageBox::warning(this, 
                                 tr("Could Not Create Data Directory"),
                                 tr("The default data directory, %1"
                                    " could not be created. Setting"
                                    " default data directory to %2.")
                                 .arg(QDir::home()
                                 .filePath(defaultDataDirName))
                                 .arg(QDir::homePath()));
            options_->opticalStudyPath = QDir::homePath();
        }
        else {
            QMessageBox::information(this,
                                     tr("Default Data Directory Created"),
                                     tr("A default data directory, %1"
                                        " has been created.  Change this"
                                        " in the System Options dialog to"
                                        " the path to your optical disk.")
                                     .arg(QDir::home()
                                     .filePath(defaultDataDirName)));
        }
    }
}

/** 
 * Label the optical disk. Opens a dialog to get the label and side,
 * then writes both to the label.dat file in the catalog directory
 * of the disk.  Not that the catalog directory is NOT necessarily the
 * root directory of the disk; with emulated optical disks they are
 * different, but the disk itself handles this.  
 * Here is the logic:
 *           Not an emulated disk:   Allow side changes any time.  All
 *               three buttons are always available. 
 *           Emulated disk:
 *               New disk: Only allow side change if isTwoSided()
 *               Flipping a disk: allow side change (don't allow 
 *                  flipping 1 sided disks)
 *               Relabeling: don't allow any side changes at all
 *           OpticalDisk::allowSideChange() will set itself appropriately
 * 
 * @param reLabel true if relabeling already labeled disk
 * @param disk OpticalDisk that is to be labeled
 */
void Navigator::labelDisk(bool reLabel, OpticalDisk* disk) {
    DiskLabelDialog* diskLabelDialog = new DiskLabelDialog(this);
    QString oldLabel = disk->label();
    // Disabled buttons can't be set, so do this first.
    diskLabelDialog->setLabelSide(oldLabel, disk->side());
    diskLabelDialog->enableNoneButton(disk->showAllSideButtons() || 
        !disk->isTwoSided());    
    diskLabelDialog->enableSideButtons(disk->allowSideChange());
    if (diskLabelDialog->exec() == QDialog::Accepted) {
        disk->setLabelSide(diskLabelDialog->label(), 
            diskLabelDialog->side());
        disk->writeLabel();
        disk->setIsLabeled(true);
        if (reLabel)
            catalogs_->relabel(diskLabelDialog->label(), 
                diskLabelDialog->side());
        refreshCatalogs();
    }
    delete diskLabelDialog;
}

void Navigator::relabelDisk() {
    labelDisk(true, currentDisk_);
}

void Navigator::updateWindowTitle() {
    AbstractMainWindow::updateWindowTitle(tr("Navigator"));
}

void Navigator::updateStatusBarUserLabel() {
    statusBar_->updateUserLabel(options_->oldStyleNavigator ?
        user_->role() : user_->name());
}

void Navigator::updateAll() {
    updateStatusBarUserLabel();
    updateWindowTitle();
    updateMenus();
}

void Navigator::noStudySelectedError() {
    QMessageBox::warning(this, tr("No Study Selected"),
                         tr("You must select a study first to do this."));
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

void Navigator::setCatalog(Catalog::Source source) {
    catalogComboBox_->setSource(source);
    changeCatalog();
}

void Navigator::setCatalogNetwork() {
    setCatalog(Catalog::Network);
}

void Navigator::setCatalogSystem() {
    setCatalog(Catalog::System);
}

void Navigator::setCatalogOptical() {
    setCatalog(Catalog::Optical);
}

void Navigator::setCatalogOther() {
    QFileDialog fd(this, tr("Select Catalog"),
        options_->systemCatalogPath, Catalog::defaultFileName());
    if (fd.exec() == QDialog::Accepted) {
        catalogs_->setCatalogPath(Catalog::Other, fd.directory().path());
        catalogs_->refresh();   // to reload Other catalog
        catalogComboBox_->setSource(Catalog::Other);
        changeCatalog();
    }
}

void Navigator::exportCatalog() {
    QFileDialog fd(this, tr("Export Catalog"),
        QDir::homePath(), tr("Comma-delimited (*.csv)"));
    fd.setFileMode(QFileDialog::AnyFile);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    if (fd.exec() == QDialog::Accepted) {
        QStringList files = fd.selectedFiles();
        QString fileName = QString();
        if (!files.isEmpty())
            fileName = files[0];
        if (!fileName.isEmpty()) {
            try {
                tableListView_->exportCSV(fileName);
            }
            catch (EpCore::IoError& e) {
                qDebug(e.what());
                QMessageBox::warning(this, tr("Error"),
                    tr("Could not export Catalog to %1").arg(e.fileName()));
            }
        }
    }
}

void Navigator::updateSimulatorSettings() {
    try {
        writeSettings();    // preserve window status
        setUpdatesEnabled(false);
        // change type of optical disk if needed
        createOpticalDrive();
        delete centralWidget_;
        createCentralWidget();
        delete catalogs_;
        createCatalogs();
        tableListView_->setOldStyle(options_->oldStyleNavigator);
        tableListView_->adjustColumns();
        refreshCatalogs();   // This repopulates the TableListView.
        // Need to do below to make sure user label
        // matches Navigator style.
        updateMenus();
        statusBar_->updateSourceLabel(catalogs_->currentCatalog()->path());
        updateStatusBarUserLabel();
        readSettings(); // restore the window settings, splitter position
        /// TODO other effects of changing simulator settings below
        setUpdatesEnabled(true);
    }
    catch (std::exception&) {
        setUpdatesEnabled(true);
        throw;
    }
}

void Navigator::updateSystemSettings() {
    networkSwitchAction_->setEnabled(options_
        ->filePathFlags.testFlag(Options::EnableNetworkStorage));
    // optical disk, status bar and catalog might be changed 
    createOpticalDrive();
    delete catalogs_;
    createCatalogs();
    refreshCatalogs();
    statusBar_->updateSourceLabel(catalogs_
        ->currentCatalog()->path());
}

// private

void Navigator::initializeOpticalDisk() {
   try {
        delete currentDisk_;
        currentDisk_ = 0;
        if (options_->opticalDiskFlags.testFlag(Options::Emulation)) {
            currentDisk_ = EmulatedOpticalDisk::getLastDisk(
                options_->opticalStudyPath);
            if (!currentDisk_) {
                currentDisk_ = new EmulatedOpticalDisk(
                    options_->opticalStudyPath,
                    options_->opticalDiskFlags.testFlag(Options::DualSided));
                currentDisk_->saveLastDisk();
            }
        }
        else
            currentDisk_ = new OpticalDisk(options_->opticalStudyPath);
        currentDisk_->readLabel();
    }
    catch (EpCore::IoError& e) { 
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
            fd->setFileMode(QFileDialog::Directory);
            if (fd->exec() == QDialog::Accepted) {
                QStringList files = fd->selectedFiles();
                QString fileName = QString();
                if (!files.isEmpty()) {
                    fileName = files[0];
                    if (!fileName.isEmpty()) {
                        options_->opticalStudyPath = fileName;
                        options_->writeSettings();
                    }
                }
            }
        }
    }
}

void Navigator::createOpticalDrive() {
    do {
        initializeOpticalDisk();
    } while (!currentDisk_);
    // let Recorder (if it is there) know what the new disk is
    // emit opticalDiskChanged(currentDisk_);
}

void Navigator::createCatalogs() {
    catalogs_ = new Catalogs(options_, currentDisk_->labelPath());
}
 
void Navigator::createCentralWidget() {
    centralWidget_ = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(centralWidget_);
    createButtonFrame();
    createTableListView();
    refreshCatalogs();
}

/**
 * Create the "blue bar" to the side of the Navigator window.  Uses
 * setupButton to make each button.  The parent of the buttonFrame is
 * the centralWidget_.  This is also the parent of the 
 * tableListView_.
 */
void Navigator::createButtonFrame() {
    if (options_->bluePanelStyle == 
        Options::TransparentButtons) // set up flat buttons
        buttonFrame_ = new NewStyleButtonFrame(centralWidget_);
    else
        buttonFrame_ = new OldStyleButtonFrame(centralWidget_);
    buttonFrame_->addButton("New Study", "hi64-newstudy",
        SLOT(newStudy()));
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
    tableListView_ = new TableListView(centralWidget_,
        options_->oldStyleNavigator);
    connect(tableListView_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,
        int)), this, SLOT(newStudy()));
}

void Navigator::createStatusBar() {
    statusBar_ = new StatusBar(catalogs_->currentCatalog()->path(), this);
    setStatusBar(statusBar_);
    updateStatusBarUserLabel();
}

void Navigator::updateMenus() {
    simulatorSettingsAction()->setVisible(showSimulatorSettings());
}

void Navigator::createActions() {
    using EpGui::createAction;    
    // Study menu
    newAction_ = createAction(this, tr("&New..."), tr("New study"),
        SLOT(newStudy()), QKeySequence(QKeySequence::New),
        "hi32-newstudy.png");
    continueAction_ = createAction(this, tr("&Continue"), 
        tr("Continue study"),
        SLOT(continueStudy()), 0, "hi32-continuestudy.png");
    reviewAction_ = createAction(this, tr("&Review"),
        tr("Review study"), SLOT(reviewStudy()), 0, "hi32-reviewstudy.png");
    preregisterAction_= createAction(this, tr("&Pre-Register"),
        tr("Pre-register patient"), SLOT(preregisterPatient()), 
        0, "hi32-preregister.png");
    reportsAction_= createAction(this, tr("R&eports..."),
        tr("Procedure reports"), SLOT(reports()), 0,
        "hi32-reports.png" );
    copyAction_= createAction(this, tr("Copy..."), tr("Copy study"),
        SLOT(copyStudy()));
    moveAction_ = createAction(this, tr("Move..."), tr("Move study"),
        SLOT(moveStudy()));
    deleteAction_= createAction(this, tr("Delete..."), tr("Delete study"),
        SLOT(deleteStudy()));
    exportAction_ = createAction(this, tr("Export..."), tr("Export study"),
        SLOT(exportCatalog()), 0, "hi32-exportstudy.png");
    exitAction_= createAction(this, tr("E&xit"), tr("Exit EP Simulator"),
        SLOT(close()), tr("Ctrl+Q"));
    // Catalog menu
    // Submenu of Switch...
    // an action "Archive Server" is skipped here, but is present on Prucka
    networkSwitchAction_ = createAction(this, tr("Network"),
        tr("Switch to network catalog"), SLOT(setCatalogNetwork()));
    // networkSwitchAction_ only enabled if set in options
    networkSwitchAction_->setEnabled(options_->
        filePathFlags.testFlag(Options::EnableNetworkStorage));
    systemSwitchAction_ = createAction(this, tr("System"),
        tr("Switch to system catalog"), SLOT(setCatalogSystem()));
    opticalSwitchAction_ = createAction(this, tr("Optical"),
        tr("Switch to optical catalog"), SLOT(setCatalogOptical()));
    // back to main menu items
    browseSwitchAction_ = createAction(this, tr("Browse..."),
        tr("Browse for catalog files"), SLOT(setCatalogOther()));
    filterStudiesAction_ = createAction(this, tr("Filter Studies..."),
        tr("Filter studies"), SLOT(filterStudies()),
        0, "hi32-filterstudies.png");
    removeStudiesFilterAction_ = createAction(this, 
        tr("Remove Studies Filter"),
        tr("Remove studies filter"), SLOT(unfilterStudies()),
        0, "hi32-removefilter.png");
    // inactivate removeStudiesFilterAction_ by default
    removeStudiesFilterAction_->setEnabled(false);
    refreshViewAction_ = createAction(this, tr("Refresh"),
        tr("Refresh the catalog"), SLOT(refreshCatalogs()),
        0, "hi32-refreshcatalog.png");
    regenerateAction_ = createAction(this, tr("Regenerate"),
        tr("Regenerate the catalog"), SLOT(regenerateCatalogs()));
    relabelDiskAction_ = createAction(this, tr("Re-Label Disk..."),
        tr("Re-label the optical disk"), SLOT(relabelDisk()));
    mergeStudiesAction_ = createAction(this, tr("Merge Studies..."),
        tr("Merge studies together"));

    // Utilities menu
    exportListsAction_ = createAction(this, tr("Export Lists..."),
        tr("Export lists"));
    exportReportFormatsAction_ = createAction(this, 
        tr("Export Report Formats..."),
        tr("Export report formats"));
    importListsAction_ = createAction(this, tr("Import Lists..."),
        tr("Import lists"));
    importReportFormatsAction_ = createAction(this, 
        tr("Import Report Formats..."),
        tr("Import report formats"));
    ejectOpticalDiskAction_ = createAction(this, tr("Eject Optical Disk"),
        tr("Eject optical disk"), SLOT(ejectDisk()));

    // Administration menu
    intervalsAction_= createAction(this, tr("Intervals"),
        tr("Intervals"), SLOT(setIntervals()));
    columnFormatsAction_= createAction(this, tr("Column Formats"),
        tr("Column formats"), SLOT(setColumnFormats()));
    protocolsAction_= createAction(this, tr("Protocols"),
        tr("Protocols"), SLOT(setProtocols()));
    studyConfigurationsAction_= createAction(this, tr("Study Configurations"),
        tr("Study configurations"), SLOT(setStudyConfigurations()));
//     systemSettingsAction_= createAction(this, tr("System Settings"),
//         tr("Change system settings"), SLOT(systemSettings()));
//     simulatorSettingsAction_ = createAction(this, tr("*Simulator Settings*"),
//         tr("Change simulator settings"), SLOT(simulatorSettings()));

    // Help menu -- in AbstractMainWindow
}

void Navigator::createToolBars() {
    navigatorToolBar_ = new QToolBar(tr("Navigator")); 
    navigatorToolBar_->setObjectName("NavigatorToolBar");
    navigatorToolBar_->setAutoFillBackground(true);
    catalogComboBox_ = new CatalogComboBox(options_);
    navigatorToolBar_->addWidget(catalogComboBox_);
    navigatorToolBar_->addSeparator();
    navigatorToolBar_->addAction(filterStudiesAction_);
    navigatorToolBar_->addAction(removeStudiesFilterAction_);
    navigatorToolBar_->addSeparator();
    navigatorToolBar_->addAction(refreshViewAction_);
    navigatorToolBar_->addSeparator();
    navigatorToolBar_->addAction(exportAction_);
    addToolBar(navigatorToolBar_);
}

void Navigator::createMenus() {
    studyMenu_ = menuBar()->addMenu(tr("&Study"));
    studyMenu_->addAction(newAction_);
    studyMenu_->addAction(continueAction_);
    studyMenu_->addAction(reviewAction_);
    studyMenu_->addAction(preregisterAction_);
    studyMenu_->addAction(reportsAction_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(copyAction_);
    studyMenu_->addAction(moveAction_);
    studyMenu_->addAction(deleteAction_);
    studyMenu_->addAction(exportAction_);
    studyMenu_->addSeparator();
    studyMenu_->addAction(exitAction_);

    catalogMenu_ = menuBar()->addMenu(tr("&Catalog"));
    switchSubMenu_ = new QMenu(tr("Switch"));
    switchSubMenu_->addAction(networkSwitchAction_);
    switchSubMenu_->addAction(systemSwitchAction_);
    switchSubMenu_->addAction(opticalSwitchAction_);
    switchSubMenu_->addAction(browseSwitchAction_);
    catalogMenu_->addMenu(switchSubMenu_);
    catalogMenu_->addAction(filterStudiesAction_);
    catalogMenu_->addAction(removeStudiesFilterAction_);
    catalogMenu_->addSeparator();
    catalogMenu_->addAction(refreshViewAction_);
    catalogMenu_->addAction(regenerateAction_);
    catalogMenu_->addAction(relabelDiskAction_);
    catalogMenu_->addAction(mergeStudiesAction_);

    utilitiesMenu_ = menuBar()->addMenu(tr("&Utilities"));
    utilitiesMenu_->addAction(exportListsAction_);
    utilitiesMenu_->addAction(exportReportFormatsAction_);
    utilitiesMenu_->addSeparator();
    utilitiesMenu_->addAction(importListsAction_);
    utilitiesMenu_->addAction(importReportFormatsAction_);
    utilitiesMenu_->addSeparator();
    utilitiesMenu_->addAction(ejectOpticalDiskAction_);

    administrationMenu_ = menuBar()->addMenu(tr("&Administration"));
    securitySubMenu_ = new QMenu(tr("Security"));
    securitySubMenu_->addAction(loginAction());
    securitySubMenu_->addAction(logoutAction());
    securitySubMenu_->addAction(changePasswordAction());
    administrationMenu_->addMenu(securitySubMenu_);
    //insert Lists submenu here
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(intervalsAction_);
    administrationMenu_->addAction(columnFormatsAction_);
    administrationMenu_->addAction(protocolsAction_);
    administrationMenu_->addAction(studyConfigurationsAction_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(systemSettingsAction());
    administrationMenu_->addAction(simulatorSettingsAction());
    // insert reports submenu here

    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(helpAction());
    helpMenu_->addAction(aboutAction());
    helpMenu_->addAction(aboutQtAction());

    updateMenus();
}

void Navigator::writeSettings() {
    QSettings settings;
    settings.beginGroup("navigator");
    settings.setValue("geometry", saveGeometry());
    // size and pos not used but might be needed for some OSes.
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("state", saveState());    
    settings.setValue("centralWidgetState",   
        centralWidget_->saveState());
    settings.endGroup();
}

/**
 * Read the current settings, including location of the centralWidget_.
 * This will also read the last "disk" used if optical disk emulation is
 * on.
 */
void Navigator::readSettings() {
    QSettings settings;
    settings.beginGroup("navigator");
    QVariant size = settings.value("size");
    if (size.isNull())  // initial run, window is maximized by default
        setWindowState(windowState() ^ Qt::WindowMaximized);
    else {  // but if not initial run, use previous window settings
        // this seems to work with X11, probably don't need
        // resize() and move().
        restoreGeometry(settings.value("geometry").toByteArray());
        //resize(size.toSize());
        //move(settings.value("pos").toPoint());
        centralWidget_->restoreState(settings.value(
            "centralWidgetState").toByteArray());
        restoreState(settings.value("state").toByteArray());
    }
    settings.endGroup();
}

void Navigator::processFilter() {
        QRegExp lastNameRegExp(filterCatalogDialog_->lastNameFilter(), 
            Qt::CaseInsensitive, QRegExp::Wildcard);
	QRegExp firstNameRegExp(filterCatalogDialog_->firstNameFilter(), 
            Qt::CaseInsensitive, QRegExp::Wildcard);
        QRegExp mrnRegExp(filterCatalogDialog_->mrnFilter(), Qt::CaseInsensitive, QRegExp::Wildcard);
	QRegExp studyConfigRegExp(filterCatalogDialog_->studyConfigFilter(),
            Qt::CaseInsensitive, QRegExp::Wildcard);
	QRegExp studyNumberRegExp(filterCatalogDialog_->studyNumberFilter(),
            Qt::CaseInsensitive, QRegExp::Wildcard);
	QRegExp studyLocationRegExp(filterCatalogDialog_->studyLocationFilter(),
            Qt::CaseInsensitive, QRegExp::Wildcard);
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
        filterStudiesAction_->setEnabled(false);
        removeStudiesFilterAction_->setEnabled(true);
        // only disallow regenerating when catalog is filtered
        /// TODO This may not be necessary.  
        /// Catalog will be refreshed automatically
        /// after it is regenerated.
        regenerateAction_->setEnabled(false);
}

void Navigator::startStudy(Study* study, bool review) {
    // write study files
    QString studiesPath = currentDisk_->studiesPath();
    QDir studiesDir(studiesPath);
    if (!studiesDir.exists() && !studiesDir.mkdir(studiesPath)) {
            delete study;
            throw EpCore::IoError(studiesPath, "could not create studiesPath");
    }
    // create study directory and write study.dat file
    QString studyPath = studiesPath + study->dirName();
    QDir studyDir(studyPath);
    if (!studyDir.exists() && !studyDir.mkdir(studyPath)) {
            delete study;
            throw EpCore::IoError(studyPath, "could not create studyPath");
    }
    study->setPath(studyPath);
    study->save();
    using EpRecorder::Recorder;
    bool allowAcquisition = options_->
        filePathFlags.testFlag(Options::EnableAcquisition) && !review;
    Recorder* recorder = new Recorder(this, study, currentDisk_, user_, 
        allowAcquisition);
    recorder->restore();
    recorder->setupInitialScreen();
    connect(recorder, SIGNAL(simulatorSettingsChanged()),
            this, SLOT(updateSimulatorSettings()));
    connect(recorder, SIGNAL(systemSettingsChanged()),
            this, SLOT(updateSystemSettings()));
    connect(recorder, SIGNAL(destroyed()), 
            this, SLOT(updateAll()));
    hide();
    updateAll();
}

void Navigator::reviewStudy(Study* study) {
    startStudy(study, true);
}

void Navigator::reports(Study*) {
    QMessageBox::information(this, tr("Starting Report Simulation"),
        tr("The Report simulation is not implemented yet.\n"
           "Will return to Navigator."));
}

/// FIXME Problem is that study key() is not fixed until the first time key() is called.
/// If key() is not called between invocations of PatientDialog, key() can change!!
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
        // study number will be set to blank, 
        // or advanced automatically if an int.
        if (int num = study->number().toInt())
            study->setNumber(QString::number(++num));
        else
            study->setNumber(QString());
        // need a new key for a new study.
        study->resetKey(); 
        return study;
    }
    else
        // Current date time already set with new study.
        return new Study;
}

/// This checks to make sure the selected study is on the optical disk catalog.
/// If something is wrong with the catalog and the study is not physically present
/// on disk, despite being in the catalog, the actual disk processing 
/// should raise an exception.
bool Navigator::studyOnDisk(const Study* s) const {
    return catalogs_->studyPresentOnOpticalDisk(s);
}

// Below reports the error and changes the disk.  Non-const function
// since disk is changed.
void Navigator::studyNotOnDiskError() {
    ejectDisk();
}

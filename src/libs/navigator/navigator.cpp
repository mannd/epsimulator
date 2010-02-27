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
#include "columnformat.h"
#include "disklabeldialog.h"
#include "editcolumnformatsdialog.h"
#include "editintervalsdialog.h"
#include "editlistdialog.h"
#include "error.h"
#include "fileutilities.h"
#include "filtercatalogdialog.h"
#include "interval.h"
#include "itemlist.h"
#include "movecopystudydialog.h"
#include "opticaldisk.h"
#include "options.h"
#include "patientdialog.h"
#include "recorder.h"
#include "selectstudyconfigdialog.h"
#include "editstudyconfigsdialog.h"
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
#include <QList>
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

using EpCore::ColumnFormat;
using EpCore::EpLists;
using EpCore::Interval;
using EpCore::ItemList;
using EpCore::Options;
using EpCore::User;
using EpCore::VersionInfo;
using EpGui::EditColumnFormatsDialog;
using EpGui::EditIntervalsDialog;
using EpGui::EditListDialog;
using EpGui::PatientDialog;
using EpGui::SelectStudyConfigDialog;
using EpStudy::Study;
using EpStudy::StudyConfiguration;
using EpStudy::StudyConfigurations;
using EpNavigator::EditStudyConfigsDialog;
using EpNavigator::Navigator;
using EpNavigator::StatusBar;

using namespace EpHardware::EpOpticalDisk;

// Note that user_ is not a Singleton, but the single instance is owned
// by Navigator and passed to Recorder.
Navigator::Navigator(QWidget* parent) : AbstractMainWindow(parent), 
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
    createLists();

    // NB: activated(), not currentIndexChanged() is required here,
    // as currentIndexChanged() is sent when the combobox is 
    // programmatically changed as well as changed by the user and
    // that will cause duplicate entries in the combobox when the
    // Other catalog is selected.
    connect(catalogComboBox_, SIGNAL(activated(int)),
        this, SLOT(changeCatalog()));

    updateWindowTitle();
    readSettings();
}

Navigator::~Navigator() {
    delete catalogs_;
    delete currentDisk_;
    delete user_;
    Options::destroy();
    VersionInfo::destroy();
}

void Navigator::clearSelection() {
    tableListView_->clearSelection();
}

// protected

void Navigator::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

// private slots

bool Navigator::acquisitionEnabled() {
    if (!epOptions->filePathFlags.testFlag(Options::EnableAcquisition)) {
        QMessageBox::information(this, tr("Acquisition Not Enabled"),
            tr("This workstation is not set up for acquisition. "
            "Select the System Settings menu item to enable acquisition."));
        return false;
    }
    return true;
}

// Blue bar actions
void Navigator::newStudy() {
    if (!acquisitionEnabled())
	return;
    if (!currentDisk_->isLabeled()) 
        relabelDisk();
    // if after all the above we finally have a label...
    if (currentDisk_->isLabeled()) {
        Study* study = getNewStudy();
        SelectStudyConfigDialog* selectStudyConfigDialog  =
            new SelectStudyConfigDialog(this);
        if (selectStudyConfigDialog->exec() == QDialog::Accepted) {
            const QString configName =
                    selectStudyConfigDialog->config();
            study->setPreregisterStudy(false);
            StudyConfigurations configList;
            if (configList.isPresent(configName))
                study->setStudyConfiguration(
                        *configList.studyConfiguration(configName));
          if (getStudyInformation(study)) {
                catalogs_->addStudy(study, currentDisk_->label(),
                                    currentDisk_->translatedSide(),
                                    epOptions->labName, user_->machineName());
                refreshCatalogs();
                startStudy(study);
            }
        }
        delete selectStudyConfigDialog;
    }
}

void Navigator::continueStudy() {
    if (!acquisitionEnabled())
	return;
    if (!epOptions->filePathFlags.testFlag(Options::EnableAcquisition)) {
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
            study->setStudyConfiguration(
                    selectStudyConfigDialog->config());
            catalogs_->deleteStudy(study);
            catalogs_->addStudy(study, currentDisk_->label(),
                    currentDisk_->translatedSide(),
                    epOptions->labName, user_->machineName());
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
    study->setPreregisterStudy(true);
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
    moveStudy(Copy);
}

void Navigator::moveStudy() {
    moveStudy(Move);
}

void Navigator::moveStudy(MoveType moveType) {
    try {
        moveStudyMessageBox(moveType);
        if (moveType == Move) {
            MoveStudyDialog moveDialog(this, currentDisk_);
            moveStudyData(moveDialog, moveType);
        }
        else if (moveType == Copy) {
            CopyStudyDialog copyDialog(this, currentDisk_);
            moveStudyData(copyDialog, moveType);
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

void Navigator::moveStudyMessageBox(MoveType moveType) {
    QString moveTypeName;
    switch(moveType) {
    case Move:
        moveTypeName = tr("move");
        break;
    case Copy:
        moveTypeName = tr("copy");
        break;
    }
    QMessageBox::information(this, tr("Study %1 Wizard").arg(capitalize(moveTypeName)),
        tr("This wizard will enable you to %1 patient studies"
           " from one location to another.  You will need to provide"
           " the location of the source and destination"
           " folders to complete the %1.").arg(moveTypeName));
}

void Navigator::moveStudyData(MoveCopyStudyDialog& dialog, MoveType moveType) {
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
            if (moveType == Move)
                regenerateCatalogs();
            else
                c.create(currentDisk_->label(), currentDisk_->side(),
                          epOptions->labName, user_->machineName());
        }
        else {    // we are copying from disk or dir to dir
            EpCore::copyDir(tmpDir.absolutePath(), dialog.destinationPath());
            OpticalCatalog c(dialog.destinationPath());
            c.create(dialog.destinationPath(), QString(), epOptions->labName,
                user_->machineName());
        }
    }
}

void Navigator::deleteStudy() {
    if (!administrationAllowed())
        return;
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
                    && epOptions->permanentDelete)
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
                          epOptions->labName, user_->machineName());
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

void Navigator::exportLists() {
    moveData(Export, Lists);
}

void Navigator::exportReportFormats() {
    moveData(Export, ReportFormats);
}

void Navigator::importLists() {
    moveData(Import, Lists);
}

void Navigator::importReportFormats() {
    moveData(Import, ReportFormats);
}

void Navigator::moveData(DataFlow flow, DataType type) {
    if (!administrationAllowed())
        return;
    try {
        if (moveDataMessageBox(flow, type)) {
            QFileDialog d(this);
            d.setFileMode(QFileDialog::DirectoryOnly);
            QString caption = tr("Data %1 Wizard")
                              .arg(flow == Import ? tr("Import")
                                  : tr("Export"));
            d.setWindowTitle(caption);
            if (d.exec()) {
                qDebug() << "Selected Dir = " << d.selectedFiles();
                QString path = d.selectedFiles()[0];
                QStringList selectedFiles;
                QList<unsigned int> magicNumbers;
                if (type == Lists) {
                    selectedFiles.append(EpLists::fileName());
                    magicNumbers.append(EpLists::magicNumber());
                    selectedFiles.append(Interval::fileName());
                    magicNumbers.append(Interval::magicNumber());
                    selectedFiles.append(ColumnFormat::fileName());
                    magicNumbers.append(ColumnFormat::magicNumber());
                    // add the rest here
                }
                else if (type == ReportFormats) {
                    // add here
                }
                if (flow == Export) {
                    statusBar()->showMessage(tr("Copying files..."));
                    EpCore::copyFilesToPath(selectedFiles,
                                            EpCore::activeSystemPath(),
                                            path, EpCore::Overwrite);
                    statusBar()->clearMessage();
                }
                else if (flow == Import) {
                    // check magic numbers since your are overwriting
                    // system files.
                    QStringListIterator iter(selectedFiles);
                    QListIterator<unsigned int> magicIter(magicNumbers);
                    QString filePath;
                    int result = QMessageBox::warning(this,
                                 tr("Overwriting Critical System Files"),
                                 tr("Note that you will be importing "
                                    "data into your system files and "
                                    "overwriting these files.  This "
                                    "is ok if you trust these files.  "
                                    "It is recommended you backup your "
                                    "system directories before importing."),
                                     QMessageBox::Ok | QMessageBox::Cancel);
                    if (result == QMessageBox::Ok)  {
                        while (iter.hasNext()) {
                            filePath = EpCore::joinPaths(path, iter.next());
                            if (EpCore::magicNumber(filePath)
                                    != magicIter.next()) {
                                throw EpCore::WrongFileTypeError(filePath);
                            }
                        }
                        statusBar()->showMessage(tr("Copying files..."));
                        EpCore::copyFilesToSystem(selectedFiles, path,
                                              EpCore::Overwrite);
                        statusBar()->clearMessage();
                    }
                }
            }
        }
    }
    catch (EpCore::SourceDestinationSameError& e) {
        QMessageBox::information(this,
            tr("Directories Identical"),
            tr("Source and destination directories must be different"));
        statusBar()->clearMessage();

    }
    catch (EpCore::WrongFileTypeError& e) {
        QMessageBox::warning(this,
                             tr("Wrong File Type"),
                             tr("File types are incompatible.  The file "
                                "%1 file type was not correct.  Data move "
                                "aborted").arg(e.fileName()));
        statusBar()->clearMessage();
    }

    catch (EpCore::IoError& e) {
        QMessageBox::warning(this,
            tr("Error moving data"),
            tr("Data could not be moved"));
        statusBar()->clearMessage();
    }
}

bool Navigator::moveDataMessageBox(Navigator::DataFlow flow,
                                   Navigator::DataType type) {
    QString dataFlow, movePhrase;
    switch(flow) {
    case Navigator::Import:
        dataFlow = tr("import");
        movePhrase = tr("from which");
        break;
    case Navigator::Export:
        dataFlow = tr("export");
        movePhrase = tr("to which");
        break;
    }
    QString dataType;
    switch(type) {
    case Navigator::Lists:
        dataType = tr("Lists"); break;
    case Navigator::ReportFormats:
        dataType = tr("Report Formats"); break;
    }
    int result = QMessageBox::information(this, tr("%1 %2 Wizard").arg(dataType)
                             .arg(capitalize(dataFlow)),
        tr("This wizard will enable you to %1 %2."
           " You will need to provide"
           " the location of a folder"
           " %3 to %1 the %2.").arg(dataFlow).arg(dataType).arg(movePhrase),
        QMessageBox::Ok | QMessageBox::Cancel);
    return result == QMessageBox::Ok;
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
            epOptions->opticalStudyPath = QDir::homePath();
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

void Navigator::pacingSites() {
    editEpList(EpLists::PacingSites,
               tr("Pacing Sites"),
               tr("Site"));
}

void Navigator::phases() {
    editEpList(EpLists::Phases,
               tr("Phases"),
               tr("Phase"));
}

void Navigator::arrhythmiaTypes() {
    editEpList(EpLists::ArrhythmiaTypes,
               tr("Arrhythmia Types"),
               tr("Type"));
}

void Navigator::arrhythmiaTolerances() {
    editEpList(EpLists::ArrhythmiaTolerances,
               tr("Arrhythmia Tolerances"),
               tr("Tolerance"));}

void Navigator::blockDescriptions() {
    editEpList(EpLists::BlockDescriptions,
               tr("Block Descriptions"),
               tr("Description"));}

void Navigator::refractoryLocations() {
    editEpList(EpLists::RefractoryLocations,
               tr("Refractory Locations"),
               tr("Location"));}

void Navigator::editEpList(EpLists::EpListType type,
                           const QString& title,
                           const QString& label) {
    EpLists epLists;
    QStringList list = epLists[type];
    EditListDialog d(list, title, label, this);
    if (d.exec()) {
        list = d.items();
        epLists[type] = list;
        epLists.update();
    }
}

void Navigator::manageSections() {}

void Navigator::manageFormats() {}

void Navigator::updateWindowTitle() {
    AbstractMainWindow::updateWindowTitle(tr("Navigator"));
}

void Navigator::updateStatusBarUserLabel() {
    statusBar_->updateUserLabel(epOptions->oldStyleNavigator ?
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
    if (administrationAllowed()) {
        EditIntervalsDialog d(this);
        if (d.exec()) {
            d.intervals().update();
        }
    }
}

void Navigator::setColumnFormats() {
    if (administrationAllowed()) {
        EditColumnFormatsDialog d(this);
        if (d.exec()) {
            d.columnFormats().update();
        }
    }
}

void Navigator::setProtocols() {
    if (administrationAllowed())
        filler();
}

void Navigator::setStudyConfigurations() {
    if (administrationAllowed()) {
        EditStudyConfigsDialog d(this);
        d.exec();
    }
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
        epOptions->systemCatalogPath, Catalog::defaultFileName());
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
        tableListView_->setOldStyle(epOptions->oldStyleNavigator);
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
    networkSwitchAction_->setEnabled(epOptions
        ->filePathFlags.testFlag(Options::EnableNetworkStorage));
    // optical disk, status bar and catalog might be changed 
    createOpticalDrive();
    // Change the blue button bar if Enable Acquisition changed
    delete centralWidget_;
    createCentralWidget();
    delete catalogs_;
    createCatalogs();
    refreshCatalogs();
    statusBar_->updateSourceLabel(catalogs_
        ->currentCatalog()->path());
    // might need to create lists if Network storage now enabled.
    createLists();
    updateMenus();
}

// private

void Navigator::initializeOpticalDisk() {
   try {
        delete currentDisk_;
        currentDisk_ = 0;
        if (epOptions->opticalDiskFlags.testFlag(Options::Emulation)) {
            currentDisk_ = EmulatedOpticalDisk::getLastDisk(
                epOptions->opticalStudyPath);
            if (!currentDisk_) {
                currentDisk_ = new EmulatedOpticalDisk(
                    epOptions->opticalStudyPath,
                    epOptions->opticalDiskFlags.testFlag(Options::DualSided));
                currentDisk_->saveLastDisk();
            }
        }
        else
            currentDisk_ = new OpticalDisk(epOptions->opticalStudyPath);
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
                        epOptions->opticalStudyPath = fileName;
                        epOptions->writeSettings();
                    }
                }
            }
            delete fd;
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
    catalogs_ = new Catalogs(epOptions, currentDisk_->labelPath());
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
    if (epOptions->bluePanelStyle ==
        Options::TransparentButtons) // set up flat buttons
        buttonFrame_ = new NewStyleButtonFrame(centralWidget_);
    else
        buttonFrame_ = new OldStyleButtonFrame(centralWidget_);
    if (epOptions->filePathFlags.testFlag(Options::EnableAcquisition)) {
        buttonFrame_->addButton("New Study", "hi64-newstudy",
            SLOT(newStudy()));
        buttonFrame_->addButton("Continue Study", "hi64-continuestudy",
        SLOT(continueStudy()));
    }
    buttonFrame_->addButton("Review Study", "hi64-reviewstudy",
        SLOT(reviewStudy()));
    if (epOptions->filePathFlags.testFlag(Options::EnableAcquisition))
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
        epOptions->oldStyleNavigator);
    connect(tableListView_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,
        int)), this, SLOT(newStudy()));
}

void Navigator::createStatusBar() {
    statusBar_ = new StatusBar(catalogs_->currentCatalog()->path(), this);
    setStatusBar(statusBar_);
    updateStatusBarUserLabel();
}

// make sure default lists are present on first run of program
void Navigator::createLists() {
    // just initializing these lists writes default values for each
    // of them to disk if the files aren't there already.
    // But don't bother if the file is already there.
    if (!EpCore::systemFileExists(EpLists::fileName()))
        EpLists lists;
    if (!EpCore::systemFileExists(Interval::fileName()))
        ItemList<Interval> intervals;
    if (!EpCore::systemFileExists(ColumnFormat::fileName()))
        ItemList<ColumnFormat> columnformats;
}

void Navigator::updateMenus() {
    simulatorSettingsAction()->setVisible(showSimulatorSettings());
    bool enableAcquisition = epOptions->filePathFlags.testFlag(Options::EnableAcquisition);
    newAction_->setEnabled(enableAcquisition);
    continueAction_->setEnabled(enableAcquisition);
    preregisterAction_->setEnabled(enableAcquisition);
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
    networkSwitchAction_->setEnabled(epOptions->
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
        tr("Export lists"), SLOT(exportLists()));
    exportReportFormatsAction_ = createAction(this, 
        tr("Export Report Formats..."),
        tr("Export report formats"), SLOT(exportReportFormats()));
    importListsAction_ = createAction(this, tr("Import Lists..."),
        tr("Import lists"), SLOT(importLists()));
    importReportFormatsAction_ = createAction(this, 
        tr("Import Report Formats..."),
        tr("Import report formats"), SLOT(importReportFormats()));
    ejectOpticalDiskAction_ = createAction(this, tr("Eject Optical Disk"),
        tr("Eject optical disk"), SLOT(ejectDisk()));

    // Administration menu
    pacingSitesAction_ = createAction(this, tr("Pacing Sites"),
                                      tr("Pacing sites"), SLOT(pacingSites()));
    phasesAction_ = createAction(this, tr("Phases"),
                                 tr("Phases"), SLOT(phases()));
    arrhythmiaTypesAction_ = createAction(this, tr("Arrhythmia Types"),
                                          tr("Arrhythmia types"),
                                          SLOT(arrhythmiaTypes()));
    arrhythmiaTolerancesAction_ = createAction(this, tr("Arrhythmia Tolerances"),
                                               tr("Arrhythmia tolerances"),
                                               SLOT(arrhythmiaTolerances()));
    blockDescriptionsAction_ = createAction(this, tr("Block Descriptions"),
                                            tr("Block descriptions"),
                                            SLOT(blockDescriptions()));
    refractoryLocationsAction_ = createAction(this, tr("Refractory Locations"),
                                               tr("Refractory locations"),
                                               SLOT(refractoryLocations()));
    intervalsAction_= createAction(this, tr("Intervals"),
        tr("Intervals"), SLOT(setIntervals()));
    columnFormatsAction_= createAction(this, tr("Column Formats"),
        tr("Column formats"), SLOT(setColumnFormats()));
    protocolsAction_= createAction(this, tr("Protocols"),
        tr("Protocols"), SLOT(setProtocols()));
    studyConfigurationsAction_= createAction(this, tr("Study Configurations"),
        tr("Study configurations"), SLOT(setStudyConfigurations()));
    manageSectionsAction_ = createAction(this, tr("Manage Sections"),
                                         tr("Manage sections"),
                                         SLOT(manageSections()));
    manageFormatsAction_ = createAction(this, tr("Manage Formats"),
                                        tr("Manage formats"),
                                        SLOT(manageFormats()));
    templatesAction_ = createAction(this, tr("Templates"),
                                    tr("Edit report templates"),
                                    0, 0);

    // Help menu -- in AbstractMainWindow

    // Non-menu actions
    // clears TableListView selection with escape key
    clearSelectionAction_ = createAction(this, tr("Clear Selection"),
                                         tr("Clear selection"),
                                         SLOT(clearSelection()),
                                         Qt::Key_Escape);
    // add directly to Navigator window -- not on menu
    addAction(clearSelectionAction_);


}

void Navigator::createToolBars() {
    navigatorToolBar_ = new QToolBar(tr("Navigator")); 
    navigatorToolBar_->setObjectName("NavigatorToolBar");
    navigatorToolBar_->setAutoFillBackground(true);
    catalogComboBox_ = new CatalogComboBox;
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
    QMenu* switchSubMenu = new QMenu(tr("Switch"));
    switchSubMenu->addAction(networkSwitchAction_);
    switchSubMenu->addAction(systemSwitchAction_);
    switchSubMenu->addAction(opticalSwitchAction_);
    switchSubMenu->addAction(browseSwitchAction_);
    catalogMenu_->addMenu(switchSubMenu);
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
    QMenu* securitySubMenu = new QMenu(tr("Security"));
    securitySubMenu->addAction(loginAction());
    securitySubMenu->addAction(logoutAction());
    securitySubMenu->addAction(changePasswordAction());
    administrationMenu_->addMenu(securitySubMenu);
    administrationMenu_->addSeparator();
    QMenu* listsSubMenu = new QMenu(tr("Lists"));
    listsSubMenu->addAction(pacingSitesAction_);
    listsSubMenu->addAction(phasesAction_);
    listsSubMenu->addAction(arrhythmiaTypesAction_);
    listsSubMenu->addAction(arrhythmiaTolerancesAction_);
    listsSubMenu->addAction(blockDescriptionsAction_);
    listsSubMenu->addAction(refractoryLocationsAction_);
    administrationMenu_->addMenu(listsSubMenu);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(intervalsAction_);
    administrationMenu_->addAction(columnFormatsAction_);
    administrationMenu_->addAction(protocolsAction_);
    administrationMenu_->addAction(studyConfigurationsAction_);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(systemSettingsAction());
    QMenu* reportsSubMenu = new QMenu(tr("Reports"));
    reportsSubMenu->addAction(manageSectionsAction_);
    reportsSubMenu->addAction(manageFormatsAction_);
    reportsSubMenu->addAction(templatesAction_);
    administrationMenu_->addMenu(reportsSubMenu);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(simulatorSettingsAction());


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
    // size and pos work best on X11
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
        // restoreGeometry doesn't work on X11
        // but resize() and move() work ok.
        // see Window Geometry section of Qt Reference Doc
        //restoreGeometry(settings.value("geometry").toByteArray());

#ifdef Q_OS_WIN32
        restoreGeometry(settings.value("geometry").toByteArray());
#else   // Mac or Linux
        // restoreGeometry doesn't work on X11
        // but resize() and move() work ok.
        // see Window Geometry section of Qt Reference Doc
        resize(size.toSize());
        move(settings.value("pos").toPoint());
#endif
        restoreState(settings.value("state").toByteArray());
        centralWidget_->restoreState(settings.value(
            "centralWidgetState").toByteArray());
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
    bool allowAcquisition = epOptions->
        filePathFlags.testFlag(Options::EnableAcquisition) && !review;
    Recorder* recorder = new Recorder(this, study, currentDisk_, user_, 
        allowAcquisition);
    recorder->restore();
    //recorder->setupInitialScreen();
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
        tr("The Report simulation is not implemented yet."));
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


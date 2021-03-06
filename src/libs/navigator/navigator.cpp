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

#include "navigator.h"

#include "actions.h"
#include "buttonframe.h"
#include "catalogcombobox.h"
#include "columnformat.h"
#include "coreconstants.h"
#include "disklabeldialog.h"
#include "editlistdialog.h"
#include "error.h"
#include "fileutilities.h"
#include "filtercatalogdialog.h"
#include "guiutilities.h"
#include "interval.h"
#include "movecopystudydialog.h"
#include "networkstorage.h"
#include "opticaldisk.h"
#include "options.h"
#include "patientdialog.h"
#include "recorder.h"
#include "selectstudyconfigdialog.h"
#include "editstudyconfigsdialog.h"
#include "statusbar.h"
#include "study.h"
#include "studyconfiguration.h"
#include "studymanager.h"
#include "studytable.h"
#include "studywriter.h"
#include "systemstorage.h"
#include "tablelistview.h"
#include "user.h"

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
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QToolBar>
#include <QVariant>

#include <algorithm>
#include <memory>

using EpCore::capitalize;
using EpCore::ColumnFormat;
using EpCore::Interval;
using EpCore::NetworkStorage;
using EpCore::Options;
using EpCore::User;
using EpGui::EditListDialog;
using EpGui::PatientDialog;
using EpGui::SelectStudyConfigDialog;
using EpStudy::Study;
using EpStudy::StudyConfiguration;
using EpStudy::StudyManager;
using EpStudy::StudyWriter;
using EpNavigator::Navigator;

using namespace EpHardware::EpOpticalDisk;

Navigator::Navigator(QWidget* parent)
    : AbstractMainWindow(Options::instance(),
                         User::instance(),
                         parent),
      filterCatalogDialog_(0),
      currentDisk_(0),
      networkStorage_(0) {
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumWidth(1000);

    options()->load();
    if (options()->includeNetworkCatalog())
        networkStorage_ = new NetworkStorage(options()->networkStudyPath);
    createOpticalDrive();
    createStudyManager();
    createCatalogs();
    createActions();
    createMenus();
    createToolBars();
    createCentralWidget();
    createStatusBar();

    connect(catalogComboBox_, SIGNAL(activated(int)),
        this, SLOT(changeCatalog()));

    updateWindowTitle();
}

Navigator::~Navigator() {
    delete catalogs_;
    delete studyManager_;
    delete studyWriter_;
    delete currentDisk_;
    delete networkStorage_;
    deletePointers();
}

void Navigator::clearSelection() {
    studyTable_->clearSelection();
}

// protected

void Navigator::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

// private slots

// Blue bar actions
void Navigator::newStudy() {
    // New Study button is grayed out/absent if EnableAcquisition not set,
    // so this shouldn't happen:
    Q_ASSERT(options()->filePathFlags.testFlag(Options::EnableAcquisition));
    if (!currentDisk_->isLabeled()) {
        relabelDisk();
        if (!currentDisk_->isLabeled())
            return; // couldn't label disk so bail out
    }
    // if after all the above we finally have a label...
    Study* study = getNewStudy();
    // need try/catch block here so that study gets deleted if error.
    SelectStudyConfigDialog* selectStudyConfigDialog  =
        new SelectStudyConfigDialog(this);
    if (selectStudyConfigDialog->exec() == QDialog::Accepted) {
        study->setPreregisterStudy(false);
        study->setStudyConfiguration(selectStudyConfigDialog
                                     ->studyConfiguration());
        if (getStudyInformation(study)) {
//            catalogs_->addStudy(study, currentDisk_->label(),
//                                currentDisk_->translatedSide(),
//                                options()->labName, user()->machineName());
            studyTable_->addStudy(study, currentDisk_->label());
//            refreshCatalogs();
            // startStudy(study);
        }
        else
            delete study;
    }
    else
        delete study;
    delete selectStudyConfigDialog;
}

void Navigator::continueStudy() {
    /// FIXME
    return;
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
                    selectStudyConfigDialog->studyConfiguration());
            catalogs_->deleteStudy(study);
            study->setPreregisterStudy(false);
            catalogs_->addStudy(study, currentDisk_->label(),
                    currentDisk_->translatedSide(),
                    options()->labName, user()->machineName());
            refreshCatalogs();
            startStudy(study);
        }
        else
            delete study;
        delete selectStudyConfigDialog;
    }
    else if (!studyOnDisk(study)) {
        studyNotOnDiskError();
        delete study;
    }
    else
        startStudy(study);
}

void Navigator::reviewStudy() {
    /// FIXME
    return;
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
    // need try/catch around this so that study is
    // deleted if IO error.
    if (getStudyInformation(study)) {
        studyManager_->addStudy(study);
        // add to System and possibly Network StudyTables
        studyTable_->addStudy(study, QString());
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

void Navigator::editStudyInCatalogs(const Study* study) {
    catalogs_->editStudy(study);
    refreshCatalogs();
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
    QMessageBox::information(this, 
                             tr("Study %1 Wizard")
                             .arg(capitalize(moveTypeName)),
        tr("This wizard will enable you to %1 patient studies "
           "from one location to another.  You will need to provide "
           "the location of the source and destination "
           "folders to complete the %1.").arg(moveTypeName));
}

void Navigator::moveStudyData(MoveCopyStudyDialog& dialog, 
                              MoveType moveType) {
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
                labelDisk(false);
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
                          options()->labName, user()->machineName());
        }
        else {    // we are copying from disk or dir to dir
            EpCore::copyDir(tmpDir.absolutePath(), dialog.destinationPath());
            OpticalCatalog c(dialog.destinationPath());
            c.create(dialog.destinationPath(), QString(), options()->labName,
                user()->machineName());
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
                    && options()->permanentDelete)
                    EpCore::deleteDir(currentDisk_->studiesPath() 
                        + study->dirName());
            }
        } 
        else 
            noStudySelectedError();
        delete study;
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
    // tableListView_->removeFilter();
    studyTable_->removeFilter();
    statusBar_->updateFilterLabel(false);
    removeStudiesFilterAction_->setEnabled(false);
    filterStudiesAction_->setEnabled(true);
//    refreshViewAction_->setEnabled(true);
    regenerateAction_->setEnabled(true);
}

void Navigator::refreshCatalogs() {
    bool includeNetwork = options()->
                          filePathFlags.testFlag(Options::EnableNetworkStorage);
    catalogComboBox_->refresh(includeNetwork);
    catalogs_->refresh();
    catalogs_->setCurrentCatalog(catalogComboBox_->source());
    tableListView_->load(catalogs_->currentCatalog());
    studyTable_->setSource(catalogComboBox_->source());
    applyFilter();
}

void Navigator::applyFilter() {
    if (tableListView_->filtered())
        processFilter();
}

void Navigator::regenerateCatalogs() {
    catalogs_->regenerate(currentDisk_->label(), currentDisk_->side(), 
                          options()->labName, user()->machineName());
    refreshCatalogs();
}

void Navigator::changeCatalog() {
    studyTable_->setSource(catalogComboBox_->source());
    catalogs_->setCurrentCatalog(catalogComboBox_->source());
    tableListView_->load(catalogs_->currentCatalog());
    applyFilter();
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
                // place lock on database
                QSqlQuery("BEGIN IMMEDIATE;");  // ? Sqlite specific?
                if (type == Lists) {
                    selectedFiles.append(EpCore::Constants::EPSIM_DB_FILENAME);
                    // append any other files here
                }
                else if (type == ReportFormats) {
                    // add here
                }
                if (flow == Export) {
                    statusBar()->showMessage(tr("Copying files..."));
                    EpCore::copyFilesToPath(selectedFiles,
                                            options()->activeSystemPath(),
                                            path, EpCore::Overwrite);
                    statusBar()->clearMessage();
                }
                else if (flow == Import) {
                    QStringListIterator iter(selectedFiles);
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
                            }
                        statusBar()->showMessage(tr("Copying files..."));
                        EpCore::copyFilesToSystem(selectedFiles, path,
                                              EpCore::Overwrite);
                        statusBar()->clearMessage();
                    }
                }
                // unlock database
                QSqlQuery("ROLLBACK;"); // Sqlite specific?
                // reset database connection
                QSqlDatabase db = QSqlDatabase::database();
                db.close();
                db.open();
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
        labelDisk(false);
    delete catalogs_;
    createCatalogs();
    refreshCatalogs();
    statusBar_->updateSourceLabel(catalogs_->currentCatalog()->path());
}

 
// Label the optical disk. Opens a dialog to get the label and side,
// then writes both to the label.dat file in the catalog directory
// of the disk.  Not that the catalog directory is NOT necessarily the
// root directory of the disk; with emulated optical disks they are
// different, but the disk itself handles this.  
// Here is the logic:
//           Not an emulated disk:   Allow side changes any time.  All
//               three buttons are always available. 
//           Emulated disk:
//               New disk: Only allow side change if isTwoSided()
//               Flipping a disk: allow side change (don't allow 
//                  flipping 1 sided disks)
//               Relabeling: don't allow any side changes at all
//           OpticalDisk::allowSideChange() will set itself appropriately
void Navigator::labelDisk(bool reLabel) {
    DiskLabelDialog* diskLabelDialog = new DiskLabelDialog(this);
    QString oldLabel = currentDisk_->label();
    // Disabled buttons can't be set, so do this first.
    diskLabelDialog->setLabelSide(oldLabel, currentDisk_->side());
    diskLabelDialog->enableNoneButton(currentDisk_->showAllSideButtons() || 
        !currentDisk_->isTwoSided());    
    diskLabelDialog->enableSideButtons(currentDisk_->allowSideChange());
    if (diskLabelDialog->exec() == QDialog::Accepted) {
        currentDisk_->setLabelSide(diskLabelDialog->label(), 
            diskLabelDialog->side());
        currentDisk_->writeLabel();
        if (reLabel)
            catalogs_->relabel(diskLabelDialog->label(), 
                diskLabelDialog->side());
        refreshCatalogs();
    }
    delete diskLabelDialog;
}

void Navigator::relabelDisk() {
    labelDisk(true);
}

void Navigator::channelLabels() {
    enum {
        ChannelLabel_Id = 0,
        ChannelLabel_Name = 1,
        ChannelLabel_MeasurementTypeId = 2
    };
    QSqlRelationalTableModel* model = new QSqlRelationalTableModel;
    model->setTable("ChannelLabels");
    model->setRelation(ChannelLabel_MeasurementTypeId,
                       QSqlRelation("MeasurementTypes",
                                    "MeasurementTypeID",
                                    "Name"));
    connect(model, SIGNAL(primeInsert(int, QSqlRecord&)),
            this, SLOT(createDefaultChannelLabel(int, QSqlRecord&)));
    EditListDialog d(model, tr("Channel Labels"),
                     QStringList() << tr("Channel")
                        << tr("Measurement Type"),
                     this);
    d.exec();
    delete model;
}

void Navigator::createDefaultChannelLabel(int /* row */,
                                          QSqlRecord& record) {
    const int measurementTypeId = 2;
    const int intracardiacType = 1;
    record.setValue(measurementTypeId, intracardiacType);
}

void Navigator::pacingSites() {
    editEpList("PacingSites",
               tr("Pacing Sites"),
               QStringList() << tr("Site"));
}

void Navigator::phases() {
    editEpList("Phases",
               tr("Phases"),
               QStringList() << tr("Phase"));
}

void Navigator::arrhythmiaTypes() {
    editEpList("ArrhythmiaTypes",
               tr("Arrhythmia Types"),
               QStringList() << tr("Type"));
}

void Navigator::arrhythmiaTolerances() {
    editEpList("ArrhythmiaTolerances",
               tr("Arrhythmia Tolerances"),
               QStringList() << tr("Tolerance"));}

void Navigator::blockDescriptions() {
    editEpList("BlockDescription",
               tr("Block Descriptions"),
               QStringList() << tr("Description"));}

void Navigator::refractoryLocations() {
    editEpList("RefractoryLocations",
               tr("Refractory Locations"),
               QStringList() << tr("Location"));}

void Navigator::editEpList(const QString& table,
                           const QString& title,
                           const QStringList& labels) {
    EditListDialog d(table, title, labels, this);
    d.exec();
}

void Navigator::manageSections() {}

void Navigator::manageFormats() {}

void Navigator::updateWindowTitle() {
    AbstractMainWindow::updateWindowTitle(tr("Navigator"));
}

void Navigator::updateStatusBarUserLabel() {
    statusBar_->updateUserLabel(options()->oldStyleNavigator ?
        user()->role() : user()->name());
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
        options()->systemCatalogPath, Catalog::defaultFileName());
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
                qDebug() << e.what();
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
        tableListView_->setOldStyle(options()->oldStyleNavigator);
        tableListView_->adjustColumns();
        studyTable_->setOldStyle(options()->oldStyleNavigator);
        studyTable_->adjustColumns();
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
    networkSwitchAction_->setEnabled(options()
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
    updateMenus();
}

// private

void Navigator::initializeOpticalDisk() {
   try {
        delete currentDisk_;
        currentDisk_ = 0;
        if (options()->opticalDiskFlags.testFlag(Options::Emulation)) {
            currentDisk_ = EmulatedOpticalDisk::getLastDisk(
                options()->opticalStudyPath);
            if (!currentDisk_) {
                currentDisk_ = new EmulatedOpticalDisk(
                    options()->opticalStudyPath,
                    options()->cachePath,
                    options()->opticalDiskFlags.testFlag(Options::DualSided));
                currentDisk_->saveLastDisk();
            }
        }
        else if (!EpCore::isRemovableMedia(options()->opticalStudyPath))
            currentDisk_ = new HardDrive(options()->opticalStudyPath,
                                         options()->cachePath);
        else 
            currentDisk_ = new OpticalDisk(options()->opticalStudyPath,
                                           options()->cachePath);
        currentDisk_->setCacheControl(options()->cacheControl);
        currentDisk_->init();
        currentDisk_->readLabel();
    }
    catch (EpCore::IoError& e) { 
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Could not find disk %1.").arg(e.fileName()));
        msgBox.setInformativeText(tr("Enter the correct path to your "
                                     "optical drive or Exit"));
        QPushButton* continueButton = msgBox.addButton(tr("Continue"), 
                                                       QMessageBox::AcceptRole);
        QPushButton* exitButton = msgBox.addButton(tr("Exit Program"), 
                                                   QMessageBox::RejectRole);
        msgBox.exec();
        if (msgBox.clickedButton() == exitButton)
            exit(1);
        else if (msgBox.clickedButton() == continueButton) {
            QFileDialog* fd = new QFileDialog(this);
            fd->setFileMode(QFileDialog::Directory);
            if (fd->exec() == QDialog::Accepted) {
                QStringList files = fd->selectedFiles();
                if (!files.isEmpty()) {
                    QString fileName = files[0];
                    options()->opticalStudyPath = fileName;
                    options()->writeSettings();
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
    catalogs_ = new Catalogs(options(), currentDisk_->labelPath());

    currentDisk_->createOpticalCatalogDbConnection();
}

void Navigator::createStudyManager() {
    bool useNetwork = options()->includeNetworkCatalog();
    studyWriter_ = new StudyWriter(useNetwork);
    QString networkPath = useNetwork ? networkStorage_->path() : QString();
    studyManager_ = new StudyManager(currentDisk_, studyWriter_, networkPath);
}
 
void Navigator::createCentralWidget() {
    centralWidget_ = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(centralWidget_);
    createButtonFrame();
    createTableListView();
    createStudyTable();
    //refreshCatalogs();
}

// Create the "blue bar" to the side of the Navigator window.  Uses
// setupButton to make each button.  The parent of the buttonFrame is
// the centralWidget_.  This is also the parent of the 
// tableListView_.
void Navigator::createButtonFrame() {
    if (options()->bluePanelStyle ==
        Options::TransparentButtons) // set up flat buttons
        buttonFrame_ = new NewStyleButtonFrame(centralWidget_,
                                               options()->bluePanelTweak);
    else
        buttonFrame_ = new OldStyleButtonFrame(centralWidget_);
    if (options()->filePathFlags.testFlag(Options::EnableAcquisition)) {
        buttonFrame_->addButton("New Study", "hi64-newstudy",
                                SLOT(newStudy()));
    }
    buttonFrame_->addButton("Continue Study", "hi64-continuestudy",
        SLOT(continueStudy()));
    buttonFrame_->addButton("Review Study", "hi64-reviewstudy",
        SLOT(reviewStudy()));
    buttonFrame_->addButton("Pre-Register", "hi64-preregister",
        SLOT(preregisterPatient()));
    buttonFrame_->addButton("Reports", "hi64-reports", 
        SLOT(reports()), true); 
}

// The tableListView_ contains the list of studies, and reflects whatever
// source is current in the catalogComboBox_.
void Navigator::createTableListView() {
    // tableListView_ = new TableListView(centralWidget_,
    //     options()->oldStyleNavigator);
    // connect(tableListView_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,
    //     int)), this, SLOT(newStudy()));
    tableListView_ = new TableListView(0,
        options()->oldStyleNavigator);
    //connect(tableListView_, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,
    //                                                 int)), this,
    //        SLOT(newStudy(const QModelIndex&)));
    //studyTable_ = new StudyTable(options()->oldStyleNavigator, centralWidget_);
}

void Navigator::createStudyTable() {
    studyTable_ = new StudyTable(options()->oldStyleNavigator, 
                                 studyWriter_,
                                 centralWidget_);
    connect(studyTable_, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(newStudy()));

}

void Navigator::createStatusBar() {
    statusBar_ = new StatusBar(catalogs_->currentCatalog()->path(), this);
    setStatusBar(statusBar_);
    updateStatusBarUserLabel();
}

void Navigator::updateMenus() {
    simulatorSettingsAction()->setVisible(showSimulatorSettings());
    bool enableAcquisition = options()->filePathFlags.testFlag(Options::EnableAcquisition);
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
    networkSwitchAction_->setEnabled(options()->
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
    proceduresAction_ = createAction(this, tr("Procedures"),
                                     tr("Procedures"), 0,
                                     tr("Alt+X"));
    staffAction_ = createAction(this, tr("Staff"), tr("Staff"), 0,
                                tr("Alt+C"));
    suppliesAction_ = createAction(this, tr("Supplies"), tr("Supplies"),
                                   0, tr("Alt+V"));
    medicationsAction_ = createAction(this, tr("Medications"),
                                      tr("Medications"), 0 , tr("Alt+B"));
    complicationsAction_ = createAction(this, tr("Complications"),
                                        tr("Complications"), 0,
                                        tr("Alt+M"));
    contrastAction_ = createAction(this, tr("Contrast"), tr("Contrast"));
    channelLabelsAction_ = createAction(this, tr("Channel Labels"),
                                        tr("Channel labels"),
                                        SLOT(channelLabels()));
    pacingSitesAction_ = createAction(this, tr("Pacing Sites"),
                                      tr("Pacing sites"), SLOT(pacingSites()));
    phasesAction_ = createAction(this, tr("Phases"),
                                 tr("Phases"), SLOT(phases()));
    arrhythmiaTypesAction_ = createAction(this, tr("Arrhythmia Types"),
                                          tr("Arrhythmia types"),
                                          SLOT(arrhythmiaTypes()));
    arrhythmiaTolerancesAction_ = createAction(this, 
                                               tr("Arrhythmia Tolerances"),
                                               tr("Arrhythmia tolerances"),
                                               SLOT(arrhythmiaTolerances()));
    blockDescriptionsAction_ = createAction(this, tr("Block Descriptions"),
                                            tr("Block descriptions"),
                                            SLOT(blockDescriptions()));
    refractoryLocationsAction_ = createAction(this, tr("Refractory Locations"),
                                               tr("Refractory locations"),
                                               SLOT(refractoryLocations()));
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
    catalogComboBox_ = new CatalogComboBox(options()->includeNetworkCatalog());
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
    listsSubMenu->addAction(proceduresAction_);
    listsSubMenu->addAction(staffAction_);
    listsSubMenu->addAction(suppliesAction_);
    listsSubMenu->addAction(medicationsAction_);
    listsSubMenu->addAction(complicationsAction_);
    listsSubMenu->addAction(contrastAction_);
    listsSubMenu->addSeparator();
    listsSubMenu->addAction(channelLabelsAction_);
    listsSubMenu->addAction(phasesAction_);
    listsSubMenu->addAction(pacingSitesAction_);
    listsSubMenu->addAction(arrhythmiaTypesAction_);
    listsSubMenu->addAction(arrhythmiaTolerancesAction_);
    listsSubMenu->addAction(blockDescriptionsAction_);
    listsSubMenu->addAction(refractoryLocationsAction_);
    administrationMenu_->addMenu(listsSubMenu);
    administrationMenu_->addSeparator();
    administrationMenu_->addAction(intervalsAction());
    administrationMenu_->addAction(columnFormatsAction());
    administrationMenu_->addAction(protocolsAction());
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


// Read the current settings, including location of the centralWidget_.
// This will also read the last "disk" used if optical disk emulation is
// on.
void Navigator::readSettings() {
    QSettings settings;
    settings.beginGroup("navigator");
    QVariant size = settings.value("size");
    if (size.isNull())  // initial run
        //setWindowState(windowState() ^ Qt::WindowMaximized);
        return; // just use default show()
    else {  // but if not initial run, use previous window settings
        EpGui::osDependentRestoreGeometry(this, settings);
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
        // TableListView::FilterStudyType filterStudyType =
        //     filterCatalogDialog_->filterStudyType();
        StudyTable::FilterStudyType filterStudyType =
            static_cast<StudyTable::FilterStudyType>(filterCatalogDialog_->filterStudyType());
 
        // tableListView_->applyFilter(filterStudyType, lastNameRegExp,
        //     firstNameRegExp, mrnRegExp, studyConfigRegExp, 
        //     studyNumberRegExp, studyLocationRegExp, 
        //     anyDate, startDate, endDate);
        studyTable_->applyFilter(filterStudyType, lastNameRegExp,
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
    /// FIXME
    return;
    // write study files
    QString studiesPath = currentDisk_->studiesPath();
    if (EpCore::useDiskCache(studiesPath)) {
        EpCore::SystemStorage systemStorage;
        studiesPath = systemStorage.filePath("studies");
    }
    QDir studiesDir(studiesPath);
    /// FIXME this will fail for an optical disk
    if (!studiesDir.exists() && !studiesDir.mkdir(studiesPath)) {
            delete study;
            throw EpCore::IoError(studiesPath, "could not create studiesPath");
    }
    // create study directory and write study.dat file
    QString studyPath = EpCore::joinPaths(studiesPath, study->dirName());
    QDir studyDir(studyPath);
    if (!studyDir.exists() && !studyDir.mkdir(studyPath)) {
            delete study;
            throw EpCore::IoError(studyPath, "could not create studyPath");
    }
    study->setPath(studyPath);
    study->save();
    bool allowAcquisition = options()->
        filePathFlags.testFlag(Options::EnableAcquisition) && !review;
    using EpRecorder::Recorder;
    Recorder* recorder = new Recorder(study, currentDisk_, user(),
                                      options(), allowAcquisition,
                                      EpRecorder::Primary, this);
    recorder->restore();
    //recorder->setupInitialScreen();
    connect(recorder, SIGNAL(simulatorSettingsChanged()),
            this, SLOT(updateSimulatorSettings()));
    connect(recorder, SIGNAL(systemSettingsChanged()),
            this, SLOT(updateSystemSettings()));
    connect(recorder, SIGNAL(destroyed()), 
            this, SLOT(updateAll()));
    // note that in signals and slots, the namespace must be given
    // explicitly -- no automatic lookup!
    connect(recorder, SIGNAL(studyDataChanged(const EpStudy::Study*)),
            this, SLOT(editStudyInCatalogs(const EpStudy::Study*)));
    hide();
    updateAll();
}

void Navigator::reviewStudy(Study* study) {
    startStudy(study, true);
}

void Navigator::reports(Study* s) {
    QMessageBox::information(this, tr("Starting Report Simulation"),
        tr("The Report simulation is not implemented yet."));
    /// TODO reports module will need to delete Study pointer,
    /// for now do it here to avoid memory leak.
    delete s;
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

// This returns a pointer to a study selected from the catalog, 
// returns 0 if no study selected
Study* Navigator::getSelectedStudy() {
    QString key = studyTable_->key();
    return studyManager_->study(key);
}

// Returns study selected in the catalog, or, if none selected, a new study.
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

// This checks to make sure the selected study is on the optical disk catalog.
// If something is wrong with the catalog and the study 
// is not physically present
// on disk, despite being in the catalog, the actual disk processing 
// should raise an exception.
bool Navigator::studyOnDisk(const Study* s) const {
    return catalogs_->studyPresentOnOpticalDisk(s);
}

// Below reports the error and changes the disk.  Non-const function
// since disk is changed.
void Navigator::studyNotOnDiskError() {
    ejectDisk();
}


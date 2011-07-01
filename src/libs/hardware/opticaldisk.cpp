/***************************************************************************
 *   Copyright (C) 2006, 2011 by EP Studios, Inc.                          *
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

#include "opticaldisk.h"

#include "coreconstants.h"
#include "error.h"
#include "fileutilities.h"
#include "selectemulateddiskdialog.h"

#include <QDataStream>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QSqlDatabase>
#include <QStringList>

#include <QtDebug>

#include <vector>

namespace EpConstants = EpCore::Constants;

namespace EpHardware { namespace EpOpticalDisk {

using EpCore::Options;

// struct LabelData

bool LabelData::operator==(const LabelData& rhs) const {
    return rhs.label == label && rhs.side == side;
}

bool LabelData::operator!=(const LabelData& rhs) const {
    return !(rhs == *this);
}

QDataStream& operator<<(QDataStream& out, const LabelData& labelData) {
    out << labelData.label << labelData.side;
    return out;
}

QDataStream& operator>>(QDataStream& in, LabelData& labelData) {
    in >> labelData.label >> labelData.side;
    return in;
}

// class OpticalDisk

const QString OpticalDisk::labelFileName_ = "label.dat";

OpticalDisk::OpticalDisk(const QString& path, const QString& cachePath)
    : path_(path),
      cachePath_(cachePath),
      initialized_(false) {
    // make sure absolute paths are used
    QDir pathDir = QDir(path_);
    QDir cachePathDir = QDir(cachePath_);
    path_ = pathDir.absolutePath();
    cachePath_ = cachePathDir.absolutePath();

    Q_ASSERT(pathDir != cachePathDir); // we don't want this because clearing
                                 // cache would erase data
    setCacheControl(Options::AutoCache);
}

OpticalDisk::~OpticalDisk() {}

void OpticalDisk::setCacheControl(Options::CacheControl cacheControl) {
    switch(cacheControl) {
    case Options::AutoCache:
    case Options::ForceCache: 
        setUseCache(true);      // setUseCache sets workingPath_
        break;
    case Options::NoCache:
	// always use a cache if removable disk
        setUseCache(isRemovable());
        break;
    }
}

bool OpticalDisk::isRemovable() const {
    return EpCore::isRemovableMedia(path_);
}

QString OpticalDisk::studiesDirName() {
    return EpConstants::EPSIM_STUDIES_DIRNAME;
}

void OpticalDisk::init() {
    if (initialized_)
        return;
    bool labelFileExists = QDir(path_).exists(labelFileName_);
    bool catalogFileExists = QDir(path_)
        .exists(EpConstants::EPSIM_CATALOG_DB_FILENAME);
    bool studiesDirExists = QDir(path_).exists(studiesDirName());
    if (useCache_) {
        clearCache();
        QStringList files;
        if (labelFileExists)
            files << labelFileName_;
        if (catalogFileExists)
            files << EpConstants::EPSIM_CATALOG_DB_FILENAME;
        EpCore::copyFilesToPath(files, path_, cachePath_);
        if (studiesDirExists)
            EpCore::copyDir(EpCore::joinPaths(path_, studiesDirName()),
                            cachePath_);
    }
    readLabel();                // reads label.dat in workingPath_
                                // or creates it
    if (!workingCatalogFileExists())
        if (!createWorkingCatalogFile())
            throw EpCore::WriteError(EpConstants::
                                     EPSIM_CATALOG_DB_FILENAME);
    // create studies dir here if needed
    if (!QDir(workingPath_).exists(studiesDirName()))
        QDir(workingPath_).mkdir(studiesDirName());
    initialized_ = true;
}

void OpticalDisk::createOpticalCatalogDbConnection() {
    // catalog.db must be present, don't call until init() called
    Q_ASSERT(initialized_);
    QSqlDatabase db = QSqlDatabase::addDatabase(EpConstants::EPSIM_BACKEND_DB,
                                                EpConstants::EPSIM_OPTICAL_DB);
    db.setHostName(EpConstants::EPSIM_DB_HOSTNAME);
    db.setDatabaseName(EpCore::joinPaths(workingPath_, 
                                         EpConstants::EPSIM_CATALOG_DB_FILENAME));
    db.setUserName(EpConstants::EPSIM_DB_USERNAME);
    db.setPassword(EpConstants::EPSIM_DB_PASSWORD);
    if (!db.open()) {
         throw EpCore::DatabaseError(db.databaseName());
    }
}

void OpticalDisk::removeDatabase() {
    QSqlDatabase db =
            QSqlDatabase::database(EpConstants::EPSIM_OPTICAL_DB);
    QSqlDatabase::removeDatabase(db.connectionName());
    db.close();
}

void OpticalDisk::clearCache() {
    if (!useCache())
        return;
    QDir cacheDir(cachePath_);
    if (cacheDir.exists())
        EpCore::deleteDirContents(cachePath_);
}

bool OpticalDisk::workingCatalogFileExists() {
    return QDir(workingPath_).exists(EpConstants::EPSIM_CATALOG_DB_FILENAME);
}

bool OpticalDisk::createWorkingCatalogFile() {
    QString catalogFileName = EpConstants::EPSIM_CATALOG_DB_FILENAME;
    QString catalogTemplateName = EpCore::joinPaths(EpCore::rootPath(), "db/",
                                                    catalogFileName);
    QString opticalCatalogName = EpCore::joinPaths(workingPath_, 
                                                   catalogFileName);
    return QFile::copy(catalogTemplateName, opticalCatalogName);
}

// done with disk, copy cache is necessary
void OpticalDisk::close() {
    if (!initialized_)
        return;
    if (useCache_) {
        QStringList files;
        files << labelFileName_ 
              << EpConstants::EPSIM_CATALOG_DB_FILENAME;
        if (!isRemovable()) {
            EpCore::copyFilesToPath(files, cachePath_, path_);
           // EpCore::deleteDir(studiesPath());
            EpCore::copyDir(EpCore::joinPaths(workingPath_, studiesDirName()),
                            path_);
        }
        else {
            burn(files, cachePath_);
            //burn studies directory from cachePath_ to path_
        }
        clearCache();
    }
    QSqlDatabase::removeDatabase(EpConstants::EPSIM_OPTICAL_DB);
    initialized_ = false;
}

QString OpticalDisk::makeStudiesPath(const QString& path) {
    return EpCore::joinPaths(path, studiesDirName());
}


/**
 * Displays change disk message.  Does nothing unless the actual disk is
 * changed, e.g. with a real optical disk drive.  If using a directory 
 * such as ~/MyStudies, there will be no change in the disk files, i.e.
 * it will seem the disk has not changed, which it hasn't.
 * @param w This is the parent window calling this function.
 */
void OpticalDisk::eject(QWidget* w) {
    // needs to identify particular disk in case multiple disks
    // are present
    if (isRemovable()) {
        QProcess process;
        // ? linux specific ?
        process.start("eject", QStringList() << path_);
        process.waitForFinished();
        QMessageBox msgBox(w);
        msgBox.setWindowTitle(tr("Eject Disk"));
        msgBox.setText(tr("Change Disk and select OK when done." ));
        msgBox.exec();
    }
}

void OpticalDisk::burn() {}

void OpticalDisk::burn(const QStringList& /*files*/,
                       const QString& /*source*/) {
    // burn files in source onto disk
}

// full path of the label.dat file, including file name.
QString OpticalDisk::labelFilePath() const {
    return EpCore::joinPaths(workingPath_, labelFileName_);
}

// full path to the studies directory on the disk. 
QString OpticalDisk::studiesPath() const {
    return makeStudiesPath(workingPath_);
}

// loads the label and side data in label.dat.
void OpticalDisk::load(const QString& fileName) {
 EpCore::loadData(fileName, MagicNumber, labelData_);
}

// saves the label and side to label.dat.
void OpticalDisk::save(const QString& fileName) const {
 EpCore::saveData(fileName, MagicNumber, labelData_);
}

void OpticalDisk::readLabel() {
    load(labelFilePath());
}

void OpticalDisk::writeLabel() const {
    save(labelFilePath());
}

void OpticalDisk::setLabel(const QString& label) {
    labelData_.label = label;
}

void OpticalDisk::setSide(const QString& side) {
    labelData_.side = side;
}

void OpticalDisk::setLabelData(const LabelData& labelData) {
    labelData_ = labelData;
}

LabelData OpticalDisk::labelData() const {
    return labelData_;
}

QString OpticalDisk::label() const {
    return labelData_.label;
}

QString OpticalDisk::side() const {
    return labelData_.side;
}

QString OpticalDisk::translatedSide() const {
    return tr(qPrintable(labelData_.side));
}

QString OpticalDisk::translateSide(const QString& side) {
    if (side.isEmpty())
        return side;
    return (side == "A" ? tr("A") : tr("B"));
}

// class EmulatedOpticalDisk

EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, 
                                         const QString& cachePath,
                                         bool isTwoSided)
                                         : OpticalDisk(path, cachePath),
                                         isTwoSided_(isTwoSided) {
    diskName_ = "disk_" + QDateTime::currentDateTime().toString(
            "ddMMyyyyhhmmsszzz");
    setSide(isTwoSided_ ? "A" : 0);
    setCacheControl(Options::AutoCache);
//    saveLastDisk();  // this must be done by calling function
}

EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path,
                                         const QString& cachePath,
                                         const QString& diskName)
                                         : OpticalDisk(path, cachePath),
                                         diskName_(diskName) {}

EmulatedOpticalDisk::~EmulatedOpticalDisk() {}

/**
 * static function to get the last disk used, 0 if none.
 * @param path = path to the optical disk
 * @return 0 if no prior disk, otherwise a pointer to the last disk used
 */
EmulatedOpticalDisk* EmulatedOpticalDisk::getLastDisk(const QString& path) {
    EmulatedOpticalDisk* e = new EmulatedOpticalDisk(path, QString());
    // we will now substitute the last disk settings for the new ones generated
    e->lastDisk();
    e->setIsTwoSided(!e->label().isEmpty());
    if (e->diskName().isEmpty())
        return 0;
    return e;
}


int EmulatedOpticalDisk::makeLabel(const QString& diskName, 
                                    QStringList& labelList,
                                    DiskInfoList& diskInfoList,
                                    int& row) {
    LabelData labelData;
    QFile f;
    int currentDiskRow = -1;
    typedef std::vector<QString> Sides;
    Sides sides;
    sides.push_back("A");
    sides.push_back("B");
    for (Sides::iterator it = sides.begin(); it != sides.end(); ++it) {
        f.setFileName(QDir::cleanPath(disksPath() + "/" + diskName + "/"
                  + (*it) + "/" + labelFileName_));
        if (f.exists()) { 
         EpCore::loadData(f.fileName(), MagicNumber, labelData);
            labelList.append(labelData.label + 
                (labelData.side.isEmpty() ? QString() : " - " 
                 + translateSide(labelData.side)));
            DiskInfo* diskInfo = new DiskInfo;
            diskInfo->name = diskName;
            diskInfo->labelData.side = labelData.side;
            diskInfo->labelData.label = labelData.label;
            diskInfoList.push_back(diskInfo);
            ++row;            
            if (diskName_ == diskName && side() == labelData.side)
                currentDiskRow = row;
        }
    }
    return currentDiskRow;
}

void EmulatedOpticalDisk::eject(QWidget* w) {
    QDir diskDir(disksPath());
    QStringList diskList = diskDir.entryList(QStringList() << "disk*");
    QStringList labelList;
    QString label, labelFile;
    DiskInfoList diskInfoList;
    int row, currentDiskRow, currentRow;
    row = currentDiskRow = currentRow = -1;
    for (QStringList::Iterator it = diskList.begin(); 
        it != diskList.end(); ++it)  
        if ((currentRow = makeLabel(*it, labelList, diskInfoList, row)) > -1)
            currentDiskRow = currentRow;
    SelectEmulatedDiskDialog* d = new SelectEmulatedDiskDialog(w);
    d->setLabelList(labelList);
    d->setDiskRow(currentDiskRow);
    if (d->exec() == QDialog::Accepted) {
        if (d->newDisk()) {
            diskName_ = QString();
            setLabel(QString());    // Must do this to make sure disk
            setSide(QString());     // is set up right as 1 or 2 sided.
            saveLastDisk();
        }
        else if (d->flipDisk()) {
            if (d->selectedItem()) {
                // get diskName and side from selected item, then
                // if a two-sided disk, flip the side
                if (!diskInfoList[d->currentItem()]->labelData.side.isEmpty()) {
                    setSide(diskInfoList[d->currentItem()]->labelData.side 
                            == "A" ? "B" : "A");
                    setLabel(diskInfoList[d->currentItem()]->labelData.label);
                    diskName_ = diskInfoList[d->currentItem()]->name;
                }
                else {      // if disk can't be flipped, just select it
                    setLabelData(diskInfoList[d->currentItem()]->labelData);
                    diskName_ = diskInfoList[d->currentItem()]->name;
                }
                saveLastDisk();
                writeLabel();
            }
        }
        else if (d->selectDisk()) {
            if (d->selectedItem()) {
                // get diskName and side from selected item
                setLabelData(diskInfoList[d->currentItem()]->labelData);
                diskName_ = diskInfoList[d->currentItem()]->name;
                saveLastDisk();
            }
        }
    }
    // clean-up
    for (DiskInfoList::iterator it = diskInfoList.begin(); 
         it != diskInfoList.end(); ++it) {
        delete (*it);
    }
    delete d;
}

void EmulatedOpticalDisk::burn() {}

QString EmulatedOpticalDisk::labelFilePath() const {
    return EpCore::joinPaths(labelPath(), labelFileName_);
}

/// returns path to /disks directory
QString EmulatedOpticalDisk::disksPath() const {
    // have to clean path as it may or may not end in /
    return QDir::cleanPath(path() + "/disks");
}

/// returns path to specific emulated disk without the side
QString EmulatedOpticalDisk::diskPath() const {
    return EpCore::joinPaths(disksPath(), diskName_);
}

/// returns full path to label.dat, including side, excluding file itself
QString EmulatedOpticalDisk::labelPath() const {
    return EpCore::joinPaths(diskPath(), sideDir());
}

QString EmulatedOpticalDisk::sideDir() const {
    return (side() == "A" || side().isEmpty() ? "A" : "B");
}

QString EmulatedOpticalDisk::studiesPath() const {
    return makeStudiesPath(labelPath());
}

void EmulatedOpticalDisk::makePath() const {
    QDir disksDir(disksPath());
    if (!disksDir.exists()) {
        if (!disksDir.mkdir(disksPath()))
            throw EpCore::IoError(disksPath());
    }
    // each directory must be created separately!
    QDir diskDir(diskPath());
    if (!diskDir.exists()) {
        if (!diskDir.mkdir(diskPath()))
            throw EpCore::IoError(diskPath());
    }
    // which side here??
    QDir pathDir(labelPath());
    if (!pathDir.exists()) {
        if (!pathDir.mkdir(labelPath()))
            throw EpCore::IoError(labelPath());
    }
}

void EmulatedOpticalDisk::readLabel() {
    // create /disks dir if not already present.
    makePath();
    OpticalDisk::load(labelFilePath());
}

void EmulatedOpticalDisk::writeLabel() const {
    makePath();
    OpticalDisk::save(labelFilePath());
}

void EmulatedOpticalDisk::lastDisk() {
    QSettings settings;
    diskName_ = settings.value("/lastDisk", "").toString();
    isTwoSided_ = settings.value("/isTwoSided", false).toBool();
    setSide(settings.value("/lastSide", QString()).toString());
}

void EmulatedOpticalDisk::saveLastDisk() {
    QSettings settings;
    settings.setValue("/lastDisk", diskName_);
    settings.setValue("/isTwoSided", isTwoSided_);
    settings.setValue("/lastSide", side());
}

void EmulatedOpticalDisk::setCacheControl(Options::CacheControl cacheControl) {
    switch(cacheControl) {
    case Options::AutoCache:
    case Options::NoCache:
        setUseCache(false);
        break;
    case Options::ForceCache: 
        setUseCache(true);
        break;
    }
}


HardDrive::HardDrive(const QString& path, const QString& cachePath) :
        OpticalDisk(path, cachePath) {
    Q_ASSERT(!EpCore::isRemovableMedia(path) 
             && !EpCore::isRemovableMedia(cachePath));
    setLabel(path);
//    using EpConstants::EPSIM_CATALOG_DB_FILENAME;
//    if (!QFile::exists(EpCore::joinPaths(path, EPSIM_CATALOG_DB_FILENAME)))
//        if (!QFile::copy(EpCore::joinPaths(EpCore::rootPath(),
//                                           "db/"
//                                           + QString(EPSIM_CATALOG_DB_FILENAME)),
//                         EpCore::joinPaths(path, EPSIM_CATALOG_DB_FILENAME))) {
//            QMessageBox::critical(0, QObject::tr("Database Error"),
//                                  QObject::tr("Cannot find or create "
//                                              "local catalog database."));
//        }
    setCacheControl(Options::AutoCache);
}

void HardDrive::setCacheControl(Options::CacheControl cacheControl) {
    switch(cacheControl) {
    case Options::AutoCache:
    case Options::NoCache:
        setUseCache(false);
        break;
    case Options::ForceCache: 
        setUseCache(true);
        break;
    }
}

} // namespace EpOpticalDisk
} // namespace EpHardware

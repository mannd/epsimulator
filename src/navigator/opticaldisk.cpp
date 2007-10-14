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

#include "opticaldisk.h"

#include "epfuns.h"
#include "error.h"
#include "selectemulateddiskdialog.h"
#include "settings.h"

#include <qdatastream.h>
#include <qdatetime.h>
#include <qdir.h>
#include <q3listbox.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstringlist.h>

#include <vector>

// for debug
#ifndef NDEBUG
#include <iostream>
#endif

// LabelData operators.

QDataStream& operator<<(QDataStream& out, const LabelData& labelData) {
    out << labelData.label << labelData.side;
    return out;
}

QDataStream& operator>>(QDataStream& in, LabelData& labelData) {
    in >> labelData.label >> labelData.side;
    return in;
}

// class OpticalDisk.

const QString OpticalDisk::labelFileName_ = "label.dat";

OpticalDisk::OpticalDisk(const QString& path) 
    : path_(path), isLabeled_(false) {
}

/**
 * Displays change disk message.  Does nothing unless the actual disk is
 * changed, e.g. with a real optical disk drive.  If using a directory 
 * such as ~/MyStudies, there will be no change in the disk files, i.e.
 * it will seem the disk has not changed, which it hasn't.
 * @param w This is the parent window calling this function.
 */
void OpticalDisk::eject(QWidget* w) {
    QMessageBox::information( w, QObject::tr("Eject Disk"),
                              QObject::tr("Change Disk and select OK when done." ));
    // Would be nice to mechanically eject disk here.
}

/**
 * 
 * @return full path of the label.dat file, including file name.
 */
QString OpticalDisk::labelFilePath() const {
    return QDir::cleanDirPath(path_ + "/" + labelFileName_);
}

/**
 * 
 * @return full path to the studies directory on the disk. 
 */
QString OpticalDisk::studiesPath() const {
    return QDir::cleanDirPath(path_ + "/studies");
}

/**
 * Loads the label and side data in label.dat.
 * @param fileName 
 */
void OpticalDisk::load(const QString& fileName) {
    EpFuns::loadData(fileName, MagicNumber, labelData_);
}

/**
 * Saves the label and side to label.dat.
 * @param fileName 
 */
void OpticalDisk::save(const QString& fileName) const {
    EpFuns::saveData(fileName, MagicNumber, labelData_);
}

void OpticalDisk::readLabel() {
    load(labelFilePath());
    isLabeled_ = !label().isEmpty();
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
    return QObject::tr(labelData_.side);    
}

QString OpticalDisk::translateSide(const QString& side) {
    if (side.isEmpty())
        return side;
    return (side == "A" ? QObject::tr("A") : QObject::tr("B"));
}

OpticalDisk::~OpticalDisk() {
}

/// FIXME This is very messy.  There should be 2 distinct actions:
///     1) create a new emulated optical disk
///     2) use the last emulated optical disk, if there is one.
/// Changing this will have a lot of downstream effects, but it needs to be done.
// EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, 
//     bool isTwoSided) : OpticalDisk(path) {
//     // Need some housekeeping to setup fake optical disk.
//     lastDisk();     // sets diskName_, labelData_
//     setIsLabeled(!label().isEmpty());
//     if (diskName_.isEmpty()) {
//         diskName_ = "disk_" + QDateTime::currentDateTime().toString(
//    		"ddMMyyyyhhmmsszzz");
//         isTwoSided_ = isTwoSided;
//         setSide(isTwoSided_ ? "A" : QString::null);
//         //writeLabel();
//         saveLastDisk(); // also do this when disk changes
//     }
// }

EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, 
                                         bool isTwoSided) 
                                         : OpticalDisk(path) {
    diskName_ = "disk_" + QDateTime::currentDateTime().toString(
            "ddMMyyyyhhmmsszzz");
    isTwoSided_ = isTwoSided;
    setSide(isTwoSided_ ? "A" : 0);
//    saveLastDisk();  // this must be done by calling function
}

EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, const QString& diskName)
    : OpticalDisk(path), diskName_(diskName) {}
    


/**
 * static function to get the last disk used, 0 if none.
 * @param path = path to the optical disk
 * @return 0 if no prior disk, otherwise a pointer to the last disk used
 */
EmulatedOpticalDisk* EmulatedOpticalDisk::getLastDisk(const QString& path) {
    EmulatedOpticalDisk* e = new EmulatedOpticalDisk(path, QString::null);
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
        f.setName(QDir::cleanDirPath(disksPath() + "/" + diskName + "/" 
                  + (*it) + "/" + labelFileName_));
        if (f.exists()) { 
            EpFuns::loadData(f.name(), MagicNumber, labelData);
            labelList.append(labelData.label + 
                (labelData.side.isEmpty() ? QString::null : " - " 
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
    QStringList diskList = diskDir.entryList("disk*");
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
            diskName_ = QString::null;
            setLabel(QString::null);    // Must do this to make sure disk
            setSide(QString::null);     // is set up right as 1 or 2 sided.
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

QString EmulatedOpticalDisk::labelFilePath() const {
    return QDir::cleanDirPath(fullPath() + "/" + labelFileName_);
}

/// returns path to /disks directory
QString EmulatedOpticalDisk::disksPath() const {
    // have to clean path as it may or may not end in /
    return QDir::cleanDirPath(path() + "/disks");
}

/// returns path to specific emulated disk without the side
QString EmulatedOpticalDisk::diskPath() const {
    return QDir::cleanDirPath(disksPath() + "/" + diskName_);
}

/// returns full path to the disk, including side
QString EmulatedOpticalDisk::fullPath() const {
    return QDir::cleanDirPath(diskPath() + "/" + sideDir());
}

QString EmulatedOpticalDisk::sideDir() const {
    return (side() == "A" || side().isEmpty() ? "A" : "B");
}

QString EmulatedOpticalDisk::studiesPath() const {
    return QDir::cleanDirPath(fullPath() + "/studies");
}

void EmulatedOpticalDisk::makePath() const {
    QDir disksDir(disksPath());
    if (!disksDir.exists()) {
        if (!disksDir.mkdir(disksPath()))
            throw EpSim::IoError(disksPath());
    }
    // each directory must be created separately!
    QDir diskDir(diskPath());
    if (!diskDir.exists()) {
        if (!diskDir.mkdir(diskPath()))
            throw EpSim::IoError(diskPath());
    }
    // which side here??
    QDir pathDir(fullPath());
    if (!pathDir.exists()) {
        if (!pathDir.mkdir(fullPath()))
            throw EpSim::IoError(fullPath());
    }
}

void EmulatedOpticalDisk::readLabel() {
    // create /disks dir if not already present.
    makePath();
    OpticalDisk::load(labelFilePath());
    setIsLabeled(!label().isEmpty());
}

void EmulatedOpticalDisk::writeLabel() const {
    makePath();
    OpticalDisk::save(labelFilePath());
}

void EmulatedOpticalDisk::lastDisk() {
    Settings settings;
    diskName_ = settings.readEntry("/lastDisk", "");
    isTwoSided_ = settings.readBoolEntry("/isTwoSided", false);
    setSide(settings.readEntry("/lastSide", QString::null));
}

void EmulatedOpticalDisk::saveLastDisk() {
    Settings settings;
    settings.writeEntry("/lastDisk", diskName_);
    settings.writeEntry("/isTwoSided", isTwoSided_);
    settings.writeEntry("/lastSide", side());
}

EmulatedOpticalDisk::~EmulatedOpticalDisk() {
}

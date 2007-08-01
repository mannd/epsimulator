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
#include <qfile.h>
#include <qlistbox.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qstringlist.h>

#include <map>
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
QString OpticalDisk::filePath() const {
    return QDir::cleanDirPath(path_ + "/" + labelFileName_);
}

QString OpticalDisk::studiesPath() const {
    return QDir::cleanDirPath(path_ + "/studies");
}

/**
 * 
 * @return disk has been labelled.  Only call after label has been read from disk.
 */
bool OpticalDisk::hasLabel() {
    return !label().isEmpty();
}

void OpticalDisk::load(const QString& fileName) {
    EpFuns::loadData(fileName, MagicNumber, labelData_);
}

void OpticalDisk::save(const QString& fileName) const {
    EpFuns::saveData(fileName, MagicNumber, labelData_);
}

void OpticalDisk::readLabel() {
    load(filePath());
}

void OpticalDisk::writeLabel() const {
    save(filePath());
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

/// FIXME allowSideChange_ handling??????????
EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, 
    bool isTwoSided) : OpticalDisk(path) {
    // Need some housekeeping to setup fake optical disk.
    lastDisk();     // sets diskName_, labelData_
    setIsLabeled(!label().isEmpty());
    if (diskName_.isEmpty()) {
        diskName_ = "disk_" + QDateTime::currentDateTime().toString(
   		"ddMMyyyyhhmmsszzz");
        isTwoSided_ = isTwoSided;
        setSide(isTwoSided_ ? "A" : QString::null);
        //writeLabel();
        saveLastDisk(); // also do this when disk changes
    }
}

int EmulatedOpticalDisk::makeLabel(const QString& diskName, 
                                    QStringList& labelList,
                                    DiskInfoList& diskInfoList) {
    LabelData labelData;
    QFile f;
    int row, currentDiskRow;
    row = currentDiskRow = -1;
    typedef std::vector<QString> Sides;
    Sides sides;
    sides.push_back("A");
    sides.push_back("B");
    for (Sides::iterator it = sides.begin(); it != sides.end(); ++it) {
        f.setName(QDir::cleanDirPath(disksPath() + "/" + diskName + "/" + (*it) + "/" 
                + labelFileName_));
        if (f.exists()) { 
            EpFuns::loadData(f.name(), MagicNumber, labelData);
            labelList.append(labelData.label + 
                (labelData.side.isEmpty() ? QString::null : " - " + translateSide(labelData.side)));
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

/// TODO label.dat should really be in the diskName/ dir, not diskName/A/.
/// The problem is if we relabel one side of a disk, the other side
/// should have the same label.  We can perhaps force relabel to do this.
void EmulatedOpticalDisk::eject(QWidget* w) {
    QDir diskDir(disksPath());
    QStringList diskList = diskDir.entryList("disk*");
    QStringList labelList;
    QString label, labelFile;
    DiskInfoList diskInfoList;
    int row, currentDiskRow;
    row = currentDiskRow = -1;
    for (QStringList::Iterator it = diskList.begin(); 
        it != diskList.end(); ++it) 
        if ((row = makeLabel(*it, labelList, diskInfoList)) > -1)
            currentDiskRow = row;
    SelectEmulatedDiskDialog* d = new SelectEmulatedDiskDialog(w);
    d->setLabelList(labelList);
    if (currentDiskRow > -1)
        d->setDiskRow(currentDiskRow);
    if (d->exec() == QDialog::Accepted) {
        if (d->newDisk()) {
            diskName_ = QString::null;
            setLabel(QString::null);    // Must do this to make sure disk
            setSide(QString::null);     // is set up right as 1 or 2 sided.
            saveLastDisk();
        }
        /// FIXME when side changes, label can't be changed, or the 2 sides
        /// will have different labels.
        else if (d->flipDisk()) {
            if (d->selectedItem()) {
                // get diskName and side from selected item, then
                // if a two-sided disk, flip the side
                if (!diskInfoList[d->currentItem()]->labelData.side.isEmpty()) {
                    setSide(diskInfoList[d->currentItem()]->labelData.side == "A" ? "B" : "A");
                    setLabel(diskInfoList[d->currentItem()]->labelData.label);
                    diskName_ = diskInfoList[d->currentItem()]->name;
 //                   writeLabel();
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
    for (DiskInfoList::iterator it = diskInfoList.begin(); it != diskInfoList.end(); ++it) {
        delete (*it);
    }
    delete d;
}

QString EmulatedOpticalDisk::filePath() const {
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
    OpticalDisk::load(filePath());
}

void EmulatedOpticalDisk::writeLabel() const {
    makePath();
    OpticalDisk::save(filePath());
}

void EmulatedOpticalDisk::lastDisk() {
    Settings settings;
    diskName_ = settings.readEntry("/lastDisk", "");
    isTwoSided_ = settings.readEntry("/isTwoSided", false);
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

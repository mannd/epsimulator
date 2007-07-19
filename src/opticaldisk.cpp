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
#include <qmessagebox.h>
#include <qobject.h>
#include <qstringlist.h>

#include <map>

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
    : path_(path) {
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
 * @return disk has been labelled. 
 */
bool OpticalDisk::hasLabel() {
    try {
        readLabel();   
        return !label().isEmpty();
    }
    catch (EpSim::IoError&) {
        return false;
    }
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

OpticalDisk::~OpticalDisk() {
}


/// FIXME emulated disks are broken at the moment.
EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, 
    bool isTwoSided) : OpticalDisk(path) {
    // Need some housekeeping to setup fake optical disk.
    lastDisk();
    if (diskName_.isEmpty()) {
        diskName_ = "disk_" + QDateTime::currentDateTime().toString(
   		"ddMMyyyyhhmmsszzz");
        isTwoSided_ = isTwoSided;
        setSide(isTwoSided_ ? "A" : QString::null);
        saveLastDisk(); // also do this when disk changes
    }
}


bool EmulatedOpticalDisk::hasLabel() {
    try {
        readLabel();   
        return !label().isEmpty();
    }
    catch (EpSim::IoError&) {
        return false;
    }
}

void EmulatedOpticalDisk::makeLabel(const QString& fileName, QString& label, QStringList& labelList) {
    QFile f(fileName);
    LabelData labelData;
    if (f.exists()) { 
        EpFuns::loadData(fileName, MagicNumber, labelData);
        label = labelData.label + " - " + labelData.side;
        labelList.append(label);
    }
}

void EmulatedOpticalDisk::eject(QWidget* w) {
    QDir diskDir(disksPath());
    QStringList diskList = diskDir.entryList("disk*");
    QStringList labelList;
    QString label, labelFile;
//    int row = 0;
    for (QStringList::Iterator it = diskList.begin(); 
        it != diskList.end(); ++it) {
        std::cerr << "Disk " << *it << std::endl;
        labelFile = QDir::cleanDirPath(disksPath() + "/" + *it + "/A/" 
            + labelFileName_);
        makeLabel(labelFile, label, labelList);
        labelFile = QDir::cleanDirPath(disksPath() + "/" + *it + "/B/" 
            + labelFileName_);
        makeLabel(labelFile, label, labelList);
    }
//     for (QStringList::Iterator pos = labelList.begin();
// //         pos != labelList.end(); ++pos)
//         std::cerr << *pos << std::endl;   */ 
    SelectEmulatedDiskDialog* d = new SelectEmulatedDiskDialog(w);
    d->setLabelList(labelList);
    // note that labelList[0] == diskList[0] so that the selected row can index diskList[row]
    // NO that isn't true because not every disk has a label.dat I guess.
    if (d->exec() )
        ;
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

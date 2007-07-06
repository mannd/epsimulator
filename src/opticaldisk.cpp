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

// for debug
#ifndef NDEBUG
#include <iostream>
#endif

const QString OpticalDisk::labelFileName_ = "label.dat";

QDataStream& operator<<(QDataStream& out, const LabelData& labelData) {
    out << labelData.label_ << labelData.side_;
    return out;
}

QDataStream& operator>>(QDataStream& in, LabelData& labelData) {
    in >> labelData.label_ >> labelData.side_;
    return in;
}

OpticalDisk::OpticalDisk(const QString& path) 
    : path_(path) {
}

/// returns full file path of label data file, including file name itself
QString OpticalDisk::filePath() {
    return path() + "/" + labelFileName_;
}

bool OpticalDisk::hasLabel() {
    QFile file(filePath());
    return file.exists() && !label().isEmpty();
}

void OpticalDisk::load(const QString& fileName) {
    EpFuns::loadData(fileName, MagicNumber, labelData_);
}

void OpticalDisk::save(const QString& fileName) {
    EpFuns::saveData(fileName, MagicNumber, labelData_);
}

void OpticalDisk::setLabel(const QString& label) {
    labelData_.label_ = label;
    save(filePath());
}

void OpticalDisk::setSide(const QString& side) {
    labelData_.side_ = side;
    save(filePath());
}

QString OpticalDisk::label() {
    if (labelData_.label_.isNull())
        load(filePath());
    return labelData_.label_;
}

QString OpticalDisk::side() {
    if (labelData_.side_.isNull())
        load(filePath());
    return labelData_.side_;
}

QString OpticalDisk::translatedSide() const {
    return QObject::tr(labelData_.side_);    
}

OpticalDisk::~OpticalDisk() {
}

EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, 
    bool isTwoSided) : OpticalDisk(path), isTwoSided_(isTwoSided) {
    // Need some housekeeping to setup fake optical disk.
    labelData_.side_ = "A"; // this is default side, and a side is needed
                            // to set up the directory tree.
    lastDisk();
    if (diskName_.isEmpty())
        diskName_ = "disk_" + QDateTime::currentDateTime().toString(
   		"ddMMyyyyhhmmsszzz");    
    saveLastDisk(); // also do this when disk changes
    /// TODO must override load, save to create the file directory.
}

// void EmulatedOpticalDisk::setSide(const QString& side) {
//     // null side not allowed with emulated optical disk?
//     if (side == "A" || side.isEmpty())
//         side_ = "A";
//     else
//         side_ == "B";
// }
// 
// QString EmulatedOpticalDisk::side() const {
//     QString side = side == "A" || side.isEmpty() ? "A" : "B";
//     return side;
// }

QString EmulatedOpticalDisk::filePath() {
    return path() + "/" + labelFileName_;
}

/// returns path to /disks directory
QString EmulatedOpticalDisk::disksPath() const {
    // have to clean path as it may or may not end in /
    return QDir::cleanDirPath(path_ + "/disks");
}

/// returns path to specific emulated disk without the side
QString EmulatedOpticalDisk::diskPath() const {
    return disksPath() + "/" + diskName_;
}

/// returns full path to the disk, including side
QString EmulatedOpticalDisk::path() {
    return diskPath() + "/" + side();
}

void EmulatedOpticalDisk::load(const QString& fileName) {
    // create /disks dir if not already present.  Better to do here than
    // in constructor as can throw.
    QDir disksDir(disksPath());
    if (!disksDir.exists()) {
        if (!disksDir.mkdir(disksPath()))
            throw EpSim::IoError(disksPath());
    }
    // each directory must be created separately!
    QDir diskDir(diskPath());
#ifndef NDEBUG
    std::cout << "OpticalDisk diskpath = " << diskPath() << std::endl;
#endif
    if (!diskDir.exists()) {
        if (!diskDir.mkdir(diskPath()))
            throw EpSim::IoError(diskPath());
    }
    QDir pathDir(path());
#ifndef NDEBUG
    std::cout << "OpticalDisk path = " << path() << std::endl;
#endif
    if (!pathDir.exists()) {
        if (!pathDir.mkdir(path()))
            throw EpSim::IoError(path());
    }

    OpticalDisk::load(fileName);
}


void EmulatedOpticalDisk::lastDisk() {
    Settings settings;
    diskName_ = settings.readEntry("/lastDisk", "");
}

void EmulatedOpticalDisk::saveLastDisk() {
    Settings settings;
    settings.writeEntry("/lastDisk", diskName_);
}

EmulatedOpticalDisk::~EmulatedOpticalDisk() {
}

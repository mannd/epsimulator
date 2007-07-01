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

OpticalDisk::OpticalDisk(const QString& path, bool isTwoSided) 
    : isTwoSided_(isTwoSided), side_(QObject::tr("A")), path_(path) {
}

/// returns full file path of label data file, including file name itself
QString OpticalDisk::filePath() const {
    return path() + "/" + labelFileName_;
}

bool OpticalDisk::hasLabel() {
    QFile file(filePath());
    return file.exists() && !label().isEmpty();
}

QString OpticalDisk::load(const QString& fileName) {
    QString label;
    EpFuns::loadData(fileName, MagicNumber, label);
    return label;
}

void OpticalDisk::save(const QString& fileName, const QString& label) {
    EpFuns::saveData(fileName, MagicNumber, label);
}

void OpticalDisk::setLabel(const QString& label) {
    // write label to disk first   
    //label_ = label;
    save(filePath(), label);
}

QString OpticalDisk::label() {
    return load(filePath());
}

QString OpticalDisk::translatedSide() const {
    return side_ == "A" ? QObject::tr("A") : QObject::tr("B");
}

OpticalDisk::~OpticalDisk() {
}

EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, 
    bool isTwoSided) : OpticalDisk(path, isTwoSided) {
    // Need some housekeeping to setup fake optical disk.
    lastDisk();
    if (diskName_.isEmpty())
        diskName_ = "disk_" + QDateTime::currentDateTime().toString(
   		"ddMMyyyyhhmmsszzz");    
    saveLastDisk(); // also do this when disk changes
    /// TODO must override load, save to create the file directory.
}

QString EmulatedOpticalDisk::filePath() const {
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
QString EmulatedOpticalDisk::path() const {
    return diskPath() + "/" + side();
}

QString EmulatedOpticalDisk::load(const QString& fileName) {
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

    return OpticalDisk::load(fileName);
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

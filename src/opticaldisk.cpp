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
#include "error.h"
#include "settings.h"

#include <qdatetime.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qobject.h>

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
    QFile file(fileName);
    // create a studies file if it doesn't exist already
    if (!file.exists()) 
        save(fileName, "");
    if (!file.open(IO_ReadOnly))
        throw EpSim::IoError(file.name(), EpSim::OpenReadFail);
    QDataStream in(&file);
    in.setVersion(5);
    Q_UINT32 magic;
    in >> magic;
    if (magic != MagicNumber)
        throw EpSim::IoError(file.name(), EpSim::WrongFileType);
    QString label;
    in >> label;
    if (file.status() != IO_Ok)
        throw EpSim::IoError(file.name(), EpSim::ReadFail);
    file.close();
    return label;
}

void OpticalDisk::save(const QString& fileName, const QString& label) {
    QFile file(fileName);
    if (!file.open(IO_WriteOnly))
        throw EpSim::IoError(file.name(), EpSim::OpenWriteFail);
    QDataStream out(&file);
    out.setVersion(5);
    out << (Q_UINT32)MagicNumber;
    out << label;
    if (file.status() != IO_Ok)
        throw EpSim::IoError(file.name(), EpSim::WriteFail);
    file.close();
}

void OpticalDisk::setLabel(const QString& label) {
    // write label to disk first   
    //label_ = label;
    save(filePath(), label);
}

QString OpticalDisk::label() {
    return load(filePath());
}

void OpticalDisk::setSide(const QString& side) {
    if (!isTwoSided_)
        side_ = QObject::tr("A");
    else if (side == QObject::tr("A") || side == QObject::tr("a"))
        side_ = QObject::tr("A");
    else if (side == QObject::tr("B") || side == QObject::tr("b"))
        side_ = QObject::tr("B");
    else
        side_ = QObject::tr("A");
}

OpticalDisk::~OpticalDisk() {
}

EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, 
    bool isTwoSided) : OpticalDisk(path, isTwoSided) {
    // Need some housekeeping to setup fake optical disk.
    diskName_ = "disk_" + QDateTime::currentDateTime().toString(
   		"ddMMyyyyhhmmsszzz");    
}

QString EmulatedOpticalDisk::filePath() const {
    return path() + "/" + labelFileName_;
}

QString EmulatedOpticalDisk::path() const {
    return path_ + "/disks/" + diskName_ + "/" + side();
}

EmulatedOpticalDisk::~EmulatedOpticalDisk() {
}

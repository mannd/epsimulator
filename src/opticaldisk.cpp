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

#include <qfile.h>
#include <qmessagebox.h>
#include <qobject.h>

const QString OpticalDisk::labelFileName_ = "label.eps";

OpticalDisk::OpticalDisk(const QString& path, bool isTwoSided) 
    : isTwoSided_(isTwoSided), path_(path) {
}

bool OpticalDisk::hasLabel() {
    label();
    return !label_.isEmpty();
}

void OpticalDisk::load(const QString& fileName) {
    QFile file(fileName);
    // create a studies file if it doesn't exist already
    if (!file.exists()) 
        save(fileName);
    if (!file.open(IO_ReadOnly))
        throw EpSim::IoError(file.name(),
              QObject::tr("Cannot open file %1 for reading"));
    QDataStream in(&file);
    in.setVersion(5);
    Q_UINT32 magic;
    in >> magic;
    if (magic != MagicNumber)
        throw EpSim::IoError(file.name(),
              QObject::tr("File %1 is not a EP Simulator file"));
    in >> label_;
    if (file.status() != IO_Ok)
        throw EpSim::IoError(file.name(),
              QObject::tr("Error reading from file %1"));
}

void OpticalDisk::save(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(IO_WriteOnly))
        throw EpSim::IoError(file.name(),
              QObject::tr("Cannot open file %1 for writing"));
    QDataStream out(&file);
    out.setVersion(5);
    out << (Q_UINT32)MagicNumber;
    out << label_;
    if (file.status() != IO_Ok)
        throw EpSim::IoError(file.name(),
              QObject::tr("Error writing to file %1"));
    file.close();
}

void OpticalDisk::setLabel(const QString& label) {
    // write label to disk first   
    label_ = label;
    save(path() + "/" + labelFileName_);
}

QString OpticalDisk::label() {
    load(path() + "/" + labelFileName_);
    return label_;
}

void OpticalDisk::setSide(const QString& side) {
    if (!isTwoSided_)
        side_ = QObject::QObject::tr("A");
    else if (side == QObject::QObject::tr("A") || side == QObject::QObject::tr("a"))
        side_ = QObject::QObject::tr("A");
    else if (side == QObject::QObject::tr("B") || side == QObject::QObject::tr("b"))
        side_ = QObject::QObject::tr("B");
    else
        side_ = QObject::QObject::tr("A");
}

OpticalDisk::~OpticalDisk() {
}

EmulatedOpticalDisk::EmulatedOpticalDisk(const QString& path, 
    bool isTwoSided) : OpticalDisk(path, isTwoSided) {
    // Need some housekeeping to setup fake optical disk.
    
    
    
}

EmulatedOpticalDisk::~EmulatedOpticalDisk() {
}

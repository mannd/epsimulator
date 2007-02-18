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

#include "disklabeldialog.h"
#include "opticaldisk.h"
#include "options.h"
#include "settings.h"

#include <qbuttongroup.h>
#include <qfile.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qradiobutton.h>

const QString OpticalDisk::labelFileName_ = "label.eps";

OpticalDisk::OpticalDisk() : isTwoSided_(false) {
    // read last disk label and side
    Settings settings;
    label_ = settings.readEntry("/lastDiskLabel", QObject::QObject::tr("1"));
    side_ = settings.readEntry("/lastDiskSide", QObject::QObject::tr("A")); 
    options_ = Options::instance();
    path_ = options_->opticalStudyPath();
    /// TODO check if disk is present
    
}


bool OpticalDisk::load(const QString& fileName) {
    QFile file(fileName);
    // create a studies file if it doesn't exist already
    if (!file.exists()) 
        save(fileName);
    if (!file.open(IO_ReadOnly)) {
        ioError(file, QObject::tr("Cannot open file %1 for reading"));
        return false;
    }
    QDataStream in(&file);
    in.setVersion(5);
    Q_UINT32 magic;
    in >> magic;
    if (magic != MagicNumber) {
        error(file, QObject::tr("File %1 is not a EP Simulator file"));
        return false;
    }
    in >> label_;
    if (file.status() != IO_Ok) {
        ioError(file, QObject::tr("Error reading from file %1"));
        return false;
    }
    return true;
}

bool OpticalDisk::save(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(IO_WriteOnly)) {
        ioError(file, QObject::tr("Cannot open file %1 for writing"));
        return false;
    }
    QDataStream out(&file);
    out.setVersion(5);
    out << (Q_UINT32)MagicNumber;
    out << label_;
    if (file.status() != IO_Ok) {
        ioError(file, QObject::tr("Error writing to file %1"));
        return false;
    }
    return true;
}

void OpticalDisk::error(const QFile& file, const QString& message) {
    QMessageBox::warning(0, QObject::tr("EP Simulator"), message.arg(file.name()));
}

void OpticalDisk::ioError(const QFile& file, const QString& message) {
    error(file, message + ": " + file.errorString());
}


void OpticalDisk::getLabel() {
    DiskLabelDialog* diskLabelDialog = new DiskLabelDialog;
    diskLabelDialog->diskLabelLineEdit->setText(label_);
    diskLabelDialog->diskSideButtonGroup->setEnabled(isTwoSided_);
    if (isTwoSided_) {
        if (side_ == QObject::tr("A"))
            diskLabelDialog->sideAButton->setChecked(true);
        else
            diskLabelDialog->sideBButton->setChecked(true);
    }
    if (diskLabelDialog->exec()) {
        label_ = diskLabelDialog->diskLabelLineEdit->text();
        if (isTwoSided_)
            side_ = diskLabelDialog->sideAButton->isChecked() ? 
                QObject::tr("A") : QObject::tr("B");
    }
    delete diskLabelDialog;
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

void OpticalDisk::eject() {
    // pseudocode:
    // if a disk is loaded
    //      eject it
    //      prompt to insert a new disk
    //      detect disk by hardware
    //      if a disk is loaded read it else 
    //          diskLoaded = false
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
    Settings settings;
    settings.writeEntry("/lastDiskLabel", label_);
    settings.writeEntry("/lastDiskSide", side_);
}

EmulatedOpticalDisk::EmulatedOpticalDisk() : OpticalDisk() {
    // Need some housekeeping to setup fake optical disk.
    
    
    
}

EmulatedOpticalDisk::~EmulatedOpticalDisk() {
}

OpticalDisk* OpticalDiskFactory::instance() {
    Options* options = Options::instance();
    if (options->emulateOpticalDrive())
        return new EmulatedOpticalDisk;
    else
        return new OpticalDisk;
}

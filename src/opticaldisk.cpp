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

#include <qobject.h>

const QString OpticalDisk::labelFileName_ = "label.eps";

OpticalDisk::OpticalDisk() : isTwoSided_(false) {
    // read last disk label and side
    Settings settings;
    label_ = settings.readEntry("/lastDiskLabel", QObject::tr("1"));
    side_ = settings.readEntry("/lastDiskSide", QObject::tr("A")); 
    options_ = Options::instance();
    path_ = options_->opticalStudyPath();
    /// TODO check if disk is present
    
}

QString OpticalDisk::getLabel() {
    DiskLabelDialog* diskLabelDialog = new DiskLabelDialog;
    if (diskLabelDialog->exec())
        //
        ;
    delete diskLabelDialog;
    
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
        side_ = QObject::tr("A");
    else if (side == QObject::tr("A") || side == QObject::tr("a"))
        side_ = QObject::tr("A");
    else if (side == QObject::tr("B") || side == QObject::tr("b"))
        side_ = QObject::tr("B");
    else
        side_ = QObject::tr("A");
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

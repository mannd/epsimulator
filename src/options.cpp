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


#include "options.h"
#include "settings.h"

#include <qapplication.h>
#include <qdir.h>

const char* Options::catalogFileName_ = "catalog.dat";

Options* Options::instance_ = 0;

Options* Options::instance() {
    if (instance_ == 0)
        instance_ = new Options;
    return instance_;
}

Options::Options() : tempStudyPath_(""),
    systemCatalogPath_(QDir::cleanDirPath(qApp->applicationDirPath() + "/../System")) {
    readSettings();
}

/**
 * Reads options from QSetting (platform-dependent location).
 */
void Options::readSettings() {
    Settings settings;
    enableAcquisition_ = settings.readBoolEntry("/enableAcquisition", true);
    enableFileExport_ = settings.readBoolEntry("/enableFileExport", false);
    enableNetworkStorage_ = settings.readBoolEntry("/enableNetworkStorage", false);
    QString defaultOpticalPath = 
        emulateOpticalDrive_ ? QDir::homeDirPath() + "/MyStudies" : "";   
    opticalStudyPath_ = settings.readEntry("/opticalStudyPath", defaultOpticalPath);
    networkStudyPath_ = settings.readEntry("/networkStudyPath", "");
    exportFilePath_ = settings.readEntry("/exportFilePath", "");
    emulateOpticalDrive_ = settings.readBoolEntry("/emulateOpticalDrive", true);
    emulateDualSidedDrive_ = 
        settings.readBoolEntry("/emulateDualSidedDrive", true);
    emulatedOpticalDriveCapacity_ 
        =settings.readNumEntry("/emulatedOpticalDriveCapacity", 0);
    oldStyleNavigator_ = settings.readBoolEntry("/oldStyleNavigator", false);
    administratorAccountRequired_ = 
        settings.readBoolEntry("/administratorAccountRequired", false);
    hideSimulatorMenu_ = 
        settings.readBoolEntry("/hideSimulatorMenu", false);
    passwordHash_ = settings.readNumEntry("/passwordHash", 0);
    /// TODO other options here...
}

/**
 * Writes options to QSetting (platform-dependent location).
 */
void Options::writeSettings() {
    Settings settings;
    settings.writeEntry("/enableAcquisition", enableAcquisition_);
    settings.writeEntry("/enableFileExport", enableFileExport_);
    settings.writeEntry("/enableNetworkStorage", enableNetworkStorage_);
    settings.writeEntry("/opticalStudyPath", opticalStudyPath_);
    settings.writeEntry("/networkStudyPath", networkStudyPath_);
    settings.writeEntry("/exportFilePath", exportFilePath_);
    settings.writeEntry("/emulateOpticalDrive", emulateOpticalDrive_);
    settings.writeEntry("/emulateDualSidedDrive", emulateDualSidedDrive_);
    settings.writeEntry("/emulatedOpticalDriveCapacity", emulatedOpticalDriveCapacity_);
    settings.writeEntry("/oldStyleNavigator", oldStyleNavigator_);
    settings.writeEntry("/administratorAccountRequired", administratorAccountRequired_);
    settings.writeEntry("/hideSimulatorMenu", hideSimulatorMenu_);
    settings.writeEntry("/passwordHash", static_cast<int>(passwordHash_));
    /// TODO add other options here...
}

Options::~Options() {
}

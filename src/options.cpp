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

#include <qapplication.h>
#include <qdir.h>
#include <qsettings.h>

const char* Options::catalogFileName_ = "catalog.eps";

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
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");
    enableAcquisition_ = settings.readBoolEntry("/enableAcquisition", true);
    emulateOpticalDrive_ = settings.readBoolEntry("/emulateOpticalDrive", true);
    enableFileExport_ = settings.readBoolEntry("/enableFileExport", false);
    enableNetworkStorage_ = settings.readBoolEntry("/enableNetworkStorage", false);
    QString defaultOpticalPath = 
        emulateOpticalDrive_ ? QDir::homeDirPath() + "/MyStudies" : "";   
    opticalStudyPath_ = settings.readEntry("/opticalStudyPath", defaultOpticalPath);
    networkStudyPath_ = settings.readEntry("/networkStudyPath", "");
    exportFilePath_ = settings.readEntry("/exportFilePath", "");
    /// TODO other options here...
    settings.endGroup();
}

/**
 * Writes options to QSetting (platform-dependent location).
 */
void Options::writeSettings() {
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");
    settings.writeEntry("/enableAcquisition", enableAcquisition_);
    settings.writeEntry("/emulateOpticalDrive", emulateOpticalDrive_);
    settings.writeEntry("/enableFileExport", enableFileExport_);
    settings.writeEntry("/enableNetworkStorage", enableNetworkStorage_);
    settings.writeEntry("/opticalStudyPath", opticalStudyPath_);
    settings.writeEntry("/networkStudyPath", networkStudyPath_);
    settings.writeEntry("/exportFilePath", exportFilePath_);
    /// TODO add other options here...
    settings.endGroup();
}

Options::~Options() {
}

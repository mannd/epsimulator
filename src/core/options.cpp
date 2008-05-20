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

#include <QCoreApplication>
#include <QDir>

#include <QtDebug>

using EpCore::Options;

Options* Options::instance_ = 0;

Options* Options::instance() {
    if (instance_ == 0)
        instance_ = new Options;
    return instance_;
}

Options::Options() :  tempStudyPath(""),
                      systemCatalogPath(QDir::cleanPath(
                      QCoreApplication::applicationDirPath() 
                      + "/../System")) {
    readSettings();
}

/**
 * Reads options from QSetting (platform-dependent location).
 */
void Options::readSettings() {
    QSettings settings;
    settings.beginGroup("options");

//     enableFileExport_ = settings.value("enableFileExport", 
//                                        false).toBool();
//     enableNetworkStorage_ = settings.value("enableNetworkStorage", 
//                                            false).toBool();
    // Non-emulated drive is the default for the program.
    emulateOpticalDrive_ = settings.value("emulateOpticalDrive", 
                                          false).toBool();
    QString defaultOpticalPath = QDir::cleanPath(QDir::homePath() 
                                 + "/MyStudies");   
    opticalStudyPath = settings.value("opticalStudyPath",
                                       defaultOpticalPath).toString();
    networkStudyPath = settings.value("networkStudyPath", 
                                       "").toString();
    exportFilePath = settings.value("exportFilePath", 
                                     "").toString();
    labName_ = settings.value("labName", "").toString();
    dualSidedDrive_ = settings.value("dualSidedDrive", 
                                     false).toBool();
    emulatedOpticalDriveCapacity_ = settings.value(
                                    "emulatedOpticalDriveCapacity", 
                                    0).toInt();
    oldStyleNavigator_ = settings.value("oldStyleNavigator", 
                                        false).toBool();

    useLabName_ = settings.value("useLabName", 
                                 false).toBool();
    administratorAccountRequired_ = settings.value(
                                    "administratorAccountRequired", 
                                    false).toBool();
    hideSimulatorMenu_ = settings.value("hideSimulatorMenu", 
                                        false).toBool();
    permanentDelete_ = settings.value("permanentDelete", false).toBool();
    /// FIXME This should probably be deviously named, like "/signalFrameCountOffset",
    /// so that the password can't be set back to blank by putting 0 in here.  Other
    /// things that could be done would be to store this in a config file not in the user
    /// directory that the user can't access, or store settings in binary, or encrypt the
    /// settings file, etc.  One problem is all this seems like overkill to prevent users
    /// from having access to some minor setup functions.
    passwordHash_ = settings.value("passwordHash", "0").toString();
    autoSaveDiskIcon_ = settings.value("autoSaveDiskIcon", true).toBool();
    immovablePatientStatusBar_ = 
        settings.value("immovablePatientStatusBar", true).toBool();
    patientStatusBarHasTitle_ = 
        settings.value("patientStatusBarHasTitle", false).toBool();
    emulateTwoScreens_ =
        settings.value("emulateTwoScreens", false).toBool();
    emulateWindowsManager_ = 
        settings.value("emulateWindowsManager", true).toBool();
    recorderStatusBar_ = settings.value("recorderStatusBar", true).toBool();
    numChannels_ = settings.value("numChannels", 48).toUInt();
    /// TODO other options here...

    screenFlags = readFlags<ScreenFlags>("screenFlags", 
        NoScreenEmulation, settings);
    bluePanelStyle = readFlags<BluePanelStyle>("bluePanelStyle", 
        OpaqueButtons, settings);
    opticalDiskFlags = readFlags<OpticalDiskFlags>("opticalDiskFlags",
        NoOpticalDiskFlags, settings);

    filePathFlags = readFlags<FilePathFlags>("filePathFlags",
        EnableAcquisition, settings);

    settings.endGroup();
}

/**
 * Writes options to QSetting (platform-dependent location).
 */
void Options::writeSettings() {
    QSettings settings;
    settings.beginGroup("options");
    
    settings.setValue("opticalStudyPath", opticalStudyPath);
    settings.setValue("networkStudyPath", networkStudyPath);
    settings.setValue("exportFilePath", exportFilePath);
    settings.setValue("labName", labName_);
    settings.setValue("emulateOpticalDrive", emulateOpticalDrive_);
    settings.setValue("dualSidedDrive", dualSidedDrive_);
    settings.setValue("emulatedOpticalDriveCapacity", emulatedOpticalDriveCapacity_);
    settings.setValue("oldStyleNavigator", oldStyleNavigator_);
    settings.setValue("useLabName", useLabName_);
    settings.setValue("administratorAccountRequired", administratorAccountRequired_);
    settings.setValue("hideSimulatorMenu", hideSimulatorMenu_);
    settings.setValue("permanentDelete", permanentDelete_);
    settings.setValue("passwordHash", passwordHash_);
    settings.setValue("autoSaveDiskIcon", autoSaveDiskIcon_);
    settings.setValue("immovablePatientStatusBar", immovablePatientStatusBar_);
    settings.setValue("patientStatusBarHasTitle", patientStatusBarHasTitle_);
    settings.setValue("emulateTwoScreens", emulateTwoScreens_);
    settings.setValue("emulateWindowsManager", emulateWindowsManager_);
    settings.setValue("recorderStatusBar", recorderStatusBar_);
    settings.setValue("numChannels", numChannels_);
    /// TODO add other options here...

    settings.setValue("screenFlags", int(screenFlags));
    settings.setValue("bluePanelStyle", bluePanelStyle);
    settings.setValue("opticalDiskFlags", int(opticalDiskFlags));
    settings.setValue("filePathFlags", int(filePathFlags));

    settings.endGroup();
}

Options::~Options() {
    writeSettings();
}

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

#include "fileutilities.h"
#include "localstorage.h"
#include "systemstorage.h"

#include <QCoreApplication>
#include <QDir>

#include <QtDebug>

// for testing and development, don't allow real optical disk
#define epNoRemovableMediaAllowed

using EpCore::Options;

Options::Options() :  screenFlags(DefaultScreenFlags),
		      recorderFlags(DefaultRecorderFlags),
		      bluePanelStyle(OpaqueButtons),
		      bluePanelTweak(false),
		      oldStyleNavigator(false),
                      opticalDiskFlags(AllowRealOpticalDisk),
		      emulatedOpticalDiskCapacity(0),
		      networkStudyPath(),
		      exportFilePath(),
		      tempStudyPath(),
		      labName(),
		      useLabName(false),
		      filePathFlags(EnableAcquisition),
		      administratorAccountRequired(false),
		      hideSimulatorMenu(false),
		      permanentDelete(false),
		      simulationControlFlags(DefaultUserControl),
                      passwordHash(),
		      useCache(AutoCache),
		      numChannels(48)
{
    EpCore::SystemStorage systemStorage;
    systemCatalogPath = systemStorage.path();
    cachePath = systemStorage.cachePath();
    EpCore::LocalStorage localStorage;
    opticalStudyPath = localStorage.opticalDiskPath();
}

/**
 * Reads options from QSettings (platform-dependent location).
 */
void Options::readSettings() {
    QSettings settings;
    settings.beginGroup("options");
    opticalStudyPath = settings.value("opticalStudyPath",

                                      opticalStudyPath).toString();
    networkStudyPath = settings.value("networkStudyPath", 
                                       networkStudyPath).toString();
    exportFilePath = settings.value("exportFilePath", 
                                     exportFilePath).toString();
    labName = settings.value("labName", labName).toString();
    emulatedOpticalDiskCapacity = settings.value(
                                    "emulatedOpticalDiskCapacity", 
                                    emulatedOpticalDiskCapacity).toInt();
    oldStyleNavigator = settings.value("oldStyleNavigator", 
                                        oldStyleNavigator).toBool();
    bluePanelTweak = settings.value("bluePanelTweak",
                                     bluePanelTweak).toBool();
    useLabName = settings.value("useLabName", 
                                 useLabName).toBool();
    administratorAccountRequired = settings.value(
                                    "administratorAccountRequired", 
                                    administratorAccountRequired).toBool();
    hideSimulatorMenu = settings.value("hideSimulatorMenu", 
                                        hideSimulatorMenu).toBool();
    permanentDelete = settings.value("permanentDelete", 
                                     permanentDelete).toBool();
    simulationControlFlags = readFlags<SimulationControlFlags>(
            "simulationControlFlags", simulationControlFlags, settings);
    passwordHash = settings.value("passwordHash", "0").toString();
    numChannels = settings.value("numChannels", numChannels).toInt();
    screenFlags = readFlags<ScreenFlags>("screenFlags", 
        screenFlags, settings);
    bluePanelStyle = readFlags<BluePanelStyle>("bluePanelStyle", 
        bluePanelStyle, settings);
    opticalDiskFlags = readFlags<OpticalDiskFlags>("opticalDiskFlags",
        opticalDiskFlags, settings);
    filePathFlags = readFlags<FilePathFlags>("filePathFlags",
        filePathFlags, settings);
    recorderFlags = readFlags<RecorderFlags>("recorderFlags",
        recorderFlags, settings);
    useCache = readFlags<UseCache>("useCache", useCache, settings);
    /// TODO other options here...

    settings.endGroup();
#ifdef epNoRemovableMediaAllowed
    opticalDiskFlags = opticalDiskFlags & ~AllowRealOpticalDisk;
    disallowOpticalDisk();
#endif
}

void Options::disallowOpticalDisk() {
    if (isRemovableMedia(QDir(opticalStudyPath))) {
        LocalStorage localStorage;
        opticalStudyPath = localStorage.hardDrivePath();
    }
}

/**
 * Writes options to QSettings (platform-dependent location).
 */
void Options::writeSettings() {
#ifdef epNoRemovableMediaAllowed
    opticalDiskFlags = opticalDiskFlags ^ AllowRealOpticalDisk;
    disallowOpticalDisk();
#endif
    QSettings settings;
    settings.beginGroup("options");

    settings.setValue("opticalStudyPath", opticalStudyPath);
    settings.setValue("networkStudyPath", networkStudyPath);
    settings.setValue("exportFilePath", exportFilePath);
    settings.setValue("labName", labName);
    settings.setValue("emulatedOpticalDiskCapacity", emulatedOpticalDiskCapacity);
    settings.setValue("oldStyleNavigator", oldStyleNavigator);
    settings.setValue("bluePanelTweak", bluePanelTweak);
    settings.setValue("useLabName", useLabName);
    settings.setValue("administratorAccountRequired", administratorAccountRequired);
    settings.setValue("hideSimulatorMenu", hideSimulatorMenu);
    settings.setValue("permanentDelete", permanentDelete);
    settings.setValue("simulationControlFlags", int(simulationControlFlags));
    settings.setValue("passwordHash", passwordHash);
    settings.setValue("numChannels", int(numChannels));
    settings.setValue("screenFlags", int(screenFlags));
    settings.setValue("bluePanelStyle", int(bluePanelStyle));
    settings.setValue("opticalDiskFlags", int(opticalDiskFlags));
    settings.setValue("filePathFlags", int(filePathFlags));
    settings.setValue("recorderFlags", int(recorderFlags));
    settings.setValue("useCache", int(useCache));
    /// TODO add other options here...

    settings.endGroup();
}

Options::~Options() {
   // writeSettings();
}

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

#include <QCoreApplication>
#include <QDir>

#include <QtDebug>

using EpCore::Options;

Options::Options() :  screenFlags(DefaultScreenFlags),
		      recorderFlags(DefaultRecorderFlags),
		      bluePanelStyle(OpaqueButtons),
		      bluePanelTweak(false),
		      oldStyleNavigator(false),
		      opticalDiskFlags(NoOpticalDiskFlags),
		      emulatedOpticalDiskCapacity(0),
		      opticalStudyPath(joinPaths(QDir::homePath(), 
						 "MyStudies")),
		      networkStudyPath(),
		      exportFilePath(),
		      tempStudyPath(),
		      systemCatalogPath(systemPath()),
		      labName(),
		      useLabName(false),
		      filePathFlags(EnableAcquisition),
		      administratorAccountRequired(false),
		      hideSimulatorMenu(false),
		      permanentDelete(false),
		      simulationControlFlags(DefaultUserControl),
		      passwordHash(0),
		      diskCache(AutoCache),
		      numChannels(48) {
//    qDebug() << "EP Simulator Directories";
//    qDebug() << "========================";
//    qDebug() << "System path:\t" << systemCatalogPath;
//    qDebug() << "Home path:\t" << QDir::homePath();
//    qDebug() << "Temp path:\t" << QDir::tempPath();
//    qDebug() << "Current path:\t" << QDir::currentPath();
//    qDebug() << "App path:\t" << rootDirectory().absolutePath();

    //readSettings();
}

/**
 * Reads options from QSettings (platform-dependent location).
 */
void Options::readSettings() {
    QSettings settings;
    settings.beginGroup("options");

    QString defaultOpticalPath = joinPaths(QDir::homePath(),"MyStudies");
    opticalStudyPath = settings.value("opticalStudyPath",
                                       defaultOpticalPath).toString();
    networkStudyPath = settings.value("networkStudyPath", 
                                       "").toString();
    exportFilePath = settings.value("exportFilePath", 
                                     "").toString();
    labName = settings.value("labName", "").toString();
    emulatedOpticalDiskCapacity = settings.value(
                                    "emulatedOpticalDiskCapacity", 
                                    0).toInt();
    oldStyleNavigator = settings.value("oldStyleNavigator", 
                                        false).toBool();
    bluePanelTweak = settings.value("bluePanelTweak",
                                     false).toBool();
    useLabName = settings.value("useLabName", 
                                 false).toBool();
    administratorAccountRequired = settings.value(
                                    "administratorAccountRequired", 
                                    false).toBool();
    hideSimulatorMenu = settings.value("hideSimulatorMenu", 
                                        false).toBool();
    permanentDelete = settings.value("permanentDelete", false).toBool();
    simulationControlFlags = readFlags<SimulationControlFlags>(
            "simulationControlFlags", DefaultUserControl, settings);
    passwordHash = settings.value("passwordHash", "0").toString();
    numChannels = settings.value("numChannels", 48).toInt();
    screenFlags = readFlags<ScreenFlags>("screenFlags", 
        DefaultScreenFlags, settings);
    bluePanelStyle = readFlags<BluePanelStyle>("bluePanelStyle", 
        OpaqueButtons, settings);
    opticalDiskFlags = readFlags<OpticalDiskFlags>("opticalDiskFlags",
        NoOpticalDiskFlags, settings);
    filePathFlags = readFlags<FilePathFlags>("filePathFlags",
        EnableAcquisition, settings);
    recorderFlags = readFlags<RecorderFlags>("recorderFlags",
        DefaultRecorderFlags, settings);
    diskCache = readFlags<DiskCache>("diskCache", AutoCache, settings);
    /// TODO other options here...

    settings.endGroup();
}

/**
 * Writes options to QSettings (platform-dependent location).
 */
void Options::writeSettings() {
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
    settings.setValue("diskCache", int(diskCache));
    /// TODO add other options here...

    settings.endGroup();
}

Options::~Options() {
    writeSettings();
}

QString EpCore::systemPath() {
    QString path = osDependentSystemPath();
    QDir dir = QDir(path);
    if (!dir.exists() && !dir.mkdir(path))
        throw SystemDirectoryNotFoundError(path);
    return path;
}

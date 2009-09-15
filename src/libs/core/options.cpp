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
 * Reads options from QSettings (platform-dependent location).
 */
void Options::readSettings() {
    QSettings settings;
    settings.beginGroup("options");

    QString defaultOpticalPath = QDir::cleanPath(QDir::homePath() 
                                 + "/MyStudies");   
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

    useLabName = settings.value("useLabName", 
                                 false).toBool();
    administratorAccountRequired = settings.value(
                                    "administratorAccountRequired", 
                                    false).toBool();
    hideSimulatorMenu = settings.value("hideSimulatorMenu", 
                                        false).toBool();
    permanentDelete = settings.value("permanentDelete", false).toBool();
    /// FIXME This should probably be deviously named, like "/signalFrameCountOffset",
    /// so that the password can't be set back to blank by putting 0 in here.  Other
    /// things that could be done would be to store this in a config file not in the user
    /// directory that the user can't access, or store settings in binary, or encrypt the
    /// settings file, etc.  One problem is all this seems like overkill to prevent users
    /// from having access to some minor setup functions.
    passwordHash = settings.value("passwordHash", "0").toString();
    numChannels = settings.value("numChannels", 48).toUInt();
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
    settings.setValue("useLabName", useLabName);
    settings.setValue("administratorAccountRequired", administratorAccountRequired);
    settings.setValue("hideSimulatorMenu", hideSimulatorMenu);
    settings.setValue("permanentDelete", permanentDelete);
    settings.setValue("passwordHash", passwordHash);
    settings.setValue("numChannels", numChannels);
    settings.setValue("screenFlags", int(screenFlags));
    settings.setValue("bluePanelStyle", bluePanelStyle);
    settings.setValue("opticalDiskFlags", int(opticalDiskFlags));
    settings.setValue("filePathFlags", int(filePathFlags));
    settings.setValue("recorderFlags", int(recorderFlags));
    /// TODO add other options here...

    settings.endGroup();
}

Options::~Options() {
    writeSettings();
}

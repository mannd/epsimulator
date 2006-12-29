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

Options::Options() {
    readSettings();
}

Options* Options::instance_ = 0;

Options* Options::instance() {
    if (instance_ == 0)
        instance_ = new Options;
    return instance_;
}

/**
 * Reads options from QSetting (platform-dependent location).
 */
void Options::readSettings() {
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");
    localStudyPath_ = settings.readEntry("/localStudyPath");
    opticalStudyPath_ = settings.readEntry("/opticalStudyPath");
    otherStudyPath_ = settings.readEntry("/otherStudyPath");
    networkStudyPath_ = settings.readEntry("/networkStudyPath");
    // note that readEntry returns QString::null if nothing found
    if (localStudyPath_.isNull())
        localStudyPath_ = QDir::homeDirPath() + "/MyStudies";
    if (opticalStudyPath_.isNull())
        opticalStudyPath_ = "/dev/cdrom";   /// TODO fix this for Windows, etc.
    if (otherStudyPath_.isNull())
        otherStudyPath_ = localStudyPath_;
    // allow networkStudyPath_ to be null
    settings.endGroup();
}

/**
 * Writes options to QSetting (platform-dependent location).
 */
void Options::writeSettings() {
    QSettings settings;
    settings.setPath("EPStudios", "EPSimulator");
    settings.beginGroup("/EPSimulator");
    settings.writeEntry("/localStudyPath", localStudyPath_);
    settings.writeEntry("/opticalStudyPath", opticalStudyPath_);
    settings.writeEntry("/otherStudyPath", otherStudyPath_);
    settings.writeEntry("/networkStudyPath", networkStudyPath_);
    settings.endGroup();
}


Options::~Options() {
}
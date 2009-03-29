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

#include "versioninfo.h"

#include "epdefs.h"

#define VERSION "0.1"

#include <QStringList>

using EpCore::VersionInfo;

VersionInfo* VersionInfo::instance_ = 0;

const VersionInfo* VersionInfo::instance() {
    if (instance_ == 0)
        instance_ = new VersionInfo;
    return instance_;
}

bool VersionInfo::versionOk(int major, int minor) {
    if (major == BadTestVersion || minor == BadTestVersion)
        return false;
    return (major >= GoodMajorVersion 
        && minor >= GoodMinorVersion);
}

// protected constructor

VersionInfo::VersionInfo(): appName_("epsimulator"), 
                            shortAppName_("epsim"),
                            programName_(tr("EP Simulator")), 
                            copyrightYear_(tr("2006-2009")),
                            version_(VERSION) {
    QStringList list = version_.split(".");
    versionMajor_ = list[0].toInt();
    versionMinor_ = list[1].toInt();
}

/***************************************************************************
 *   Copyright (C) 2010 by EP Studios, Inc.                                *
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

#include "../libs/core/fileutilities.h"

#include <QtCore/QDir>
#include <QtCore/QProcessEnvironment>

#include <windows.h>
#include <winsock2.h>

#include <cstdlib>

bool EpCore::isRemovableMedia(const QDir& dir) {
    QString path = dir.absolutePath();
    path = path.toUpper() + "\\";
    UINT ret;
    const char* pathAnsi = path.toLatin1().constData();
    int lenA = lstrlenA(pathAnsi);
    int lenW;
    BSTR pathUnicode;
    lenW = ::MultiByteToWideChar(CP_ACP, 0, pathAnsi, lenA, 0, 0);
    if (lenW > 0) {
        pathUnicode = ::SysAllocStringLen(0, lenW);
        ::MultiByteToWideChar(CP_ACP, 0, pathAnsi, lenA, pathUnicode, lenW);
    }
    else    // on error assume is removable
        return true;
    ret = GetDriveType(pathUnicode);
    ::SysFreeString(pathUnicode);
    return ret !=  DRIVE_FIXED;
}

QString EpCore::osDependentSystemPath() {
    QString appData = QProcessEnvironment::systemEnvironment().value("APPDATA");
    QString path = joinPaths(appData, "epsimulator");
    return path;
}

QString EpCore::getUserName() {
    return std::getenv("USERNAME");
}

QString EpCore::getMachineName() {
    WSAData cData;
    WSAStartup(MAKEWORD(2, 2), &cData);
    char machineName[1024];
    int result = gethostname(machineName, sizeof(machineName));
    return result == 0 ? machineName : QString();
}

long EpCore::diskFreeSpace(const QString &path) {
    /// TODO windows version of this function
    return 0;
}



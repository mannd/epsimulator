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

#include "fileutilities.h"

#include <QtCore/QDir>

#include <sys/vfs.h>
#include <unistd.h>
#include <cstdlib>

bool EpCore::isRemovableMedia(const QDir& dir) {
    QString path = dir.absolutePath();
    qDebug() << "Path is " << path;
    QStringList elements = path.split("/");
    return elements.contains("media");
}

QString EpCore::osDependentSystemPath() {
    QString path;
#ifdef Q_OS_MAC
    path = systemDirectory();
#else   // unix, linux
    path = joinPaths(QDir::homePath(), ".epsimulator");
#endif
    return path;
}

QString EpCore::getUserName() {
    return std::getenv("USER");
}

QString EpCore::getMachineName() {
    char machineName[1024] = "";
    return gethostname(machineName, sizeof(machineName)) == 0
                       ? machineName : QString();
}

long EpCore::diskFreeSpace(const QString& path) {
    struct statfs s;
    long blocksFree = 0;
    long blockSize = 0;
    if (statfs(path.toLatin1().constData(), &s) == 0) {
        blocksFree = s.f_bavail;    // blocks available to non-su
        blockSize = s.f_bsize;
    }
    long kBytes = blockSize / 1024;
    return blocksFree * kBytes;
}

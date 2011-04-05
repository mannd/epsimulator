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

#include "systemstorage.h"

#include "coreconstants.h"
#include "fileutilities.h"

#include <QDir>

using EpCore::SystemStorage;

SystemStorage::SystemStorage() {
    path_ = EpCore::osDependentSystemPath();
}

bool SystemStorage::init() {
    QDir dir = QDir(path_);
    if (!exists() && !dir.mkpath(path_))
        return false;
    if (isRemovableMedia(path_))
        return false;           // this shouldn't happen
    if (!makeCachePath())
        return false;
    return true;                // path_ exists or was made
}

bool SystemStorage::makeCachePath() {
    QDir dir = QDir(cachePath());
    return dir.exists() || dir.mkpath(cachePath());
}

QString SystemStorage::filePath(const QString& fileName) const {
    return EpCore::joinPaths(path_, fileName);
}

QString SystemStorage::cachePath() const {
    return filePath(Constants::EPSIM_CACHE_DIRNAME);
}

bool SystemStorage::exists() const {
    return QDir(path_).exists();
}

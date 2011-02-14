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
    return true;                // path_ exists or was made
}

QString SystemStorage::filePath(const QString& fileName) const {
    return EpCore::joinPaths(path_, fileName);
}

bool SystemStorage::exists() const {
    return QDir(path_).exists();
}

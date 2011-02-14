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

#ifndef LOCALSTORAGE_H
#define LOCALSTORAGE_H

#include <QString>

namespace EpCore {

class LocalStorage {
public:
    LocalStorage();
    bool init();                // figure out default paths
    QString opticalDiskPath() const {return opticalDiskPath_;}
    QString hardDrivePath() const {return hardDrivePath_;}

    void setOpticalDiskPath(QString path) {opticalDiskPath_ = path;}
    void setHardDrivePath(QString path) {hardDrivePath_ = path;}
private:
    QString opticalDiskPath_;
    QString hardDrivePath_;
};

}

#endif // LOCALSTORAGE_H

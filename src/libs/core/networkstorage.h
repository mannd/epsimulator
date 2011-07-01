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

#ifndef NETWORKSTORAGE_H
#define NETWORKSTORAGE_H

#include <QString>

namespace EpCore {

class NetworkStorage {
public:
    NetworkStorage(const QString& path);
    bool init();                // creates network directory if nonexistant
    QString path() const {return path_;}
    QString filePath(const QString& fileName) const; // returns full network
                                                     // path of fileName,
                                                     // does not create file

    QString studiesPath() const; // full path of studies dir
    bool exists() const;

    void setPath(QString path) {path_ = path;} // mostly for testing
private:
    QString path_;
};

} // namespace EpCore

#endif // NETWORKSTORAGE_H

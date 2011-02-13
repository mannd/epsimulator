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

#ifndef SYSTEMPATH_H
#define SYSTEMPATH_H

#include <QString>

namespace EpCore {

class SystemPath {
public:
    SystemPath();
    bool init();                // creates system directory if nonexistant
    QString operator()() const {return path_;}
    QString path() const {return path_;}
    QString filePath(const QString& fileName) const; // returns full system
                                                     // path of fileName,
                                                     // does not create file
    bool exists() const;

    void setPath(QString path) {path_ = path;} // mostly for testing
private:
    QString path_;
};

} // namespace EpCore

#endif // SYSTEMPATH_H

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
#ifndef EPSIMVERSIONINFO_H
#define EPSIMVERSIONINFO_H

#include <QString>

/**
 * Provides version and application information for a program.
 *
 *	@author David Mann <mannd@epstudiossoftware.com>
 */
class VersionInfo{
public:
    static VersionInfo* instance();

    QString appName() {return appName_;}
    QString shortAppName() {return shortAppName_;}
    QString programName() {return programName_;}
    int versionMajor() {return versionMajor_;}
    int versionMinor() {return versionMinor_;}
    QString version() {return QString("%1.%2").arg(versionMajor_)
                       .arg(versionMinor_);}
    QString copyrightYear() {return copyrightYear_;}
    static bool versionOk(int major, int minor);

    ~VersionInfo() {}

    void destroy() {delete instance_; instance_ = 0;}

private:
    /// Major or minor version number == BadTestVersion then version not OK.
    /// Version numbers >= the Good... versions are OK.  This system is to 
    /// make sure file formats are compatible when the program is updated.
    enum {BadTestVersion = 9999, GoodMajorVersion = 0, 
        GoodMinorVersion = 0};


    VersionInfo(); 

    static VersionInfo* instance_;
    QString appName_;
    QString shortAppName_;
    QString programName_;
    QString copyrightYear_;
    int versionMajor_;
    int versionMinor_;
};

#endif


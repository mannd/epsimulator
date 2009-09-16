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

#ifndef VERSIONINFO_H
#define VERSIONINFO_H

#include <QCoreApplication>

namespace EpCore {

/**
 * Provides version and application information for a program.
 * This is a Singleton class, and instances are forced to be
 * const due to a factory method that returns a const instance.
 * It should be ok to let this be a Singleton; there can
 * only be one instance, it is const, and we'll let it
 * be globally available.
 *
 *	@author David Mann <mannd@epstudiossoftware.com>
 */

class VersionInfo {
    Q_DECLARE_TR_FUNCTIONS(VersionInfo)

public:
    static const VersionInfo* instance();
    // destroy() is static so it can be called as VersionInfo::destroy()
    // without an object.
    static void destroy() {delete instance_; instance_ = 0;}

    static bool versionOk(int major, int minor);

    QString appName() const {return appName_;}
    QString shortAppName() const {return shortAppName_;}
    QString programName() const {return programName_;}
    int versionMajor() const {return versionMajor_;}
    int versionMinor() const {return versionMinor_;}
    QString version() const {return version_;}
    QString copyrightYear() const {return copyrightYear_;}

protected:
    VersionInfo(); 
    ~VersionInfo() {}

private:
    /// Major or minor version number == BadTestVersion then version not OK.
    /// Version numbers >= the Good... versions are OK.  This system is to 
    /// make sure file formats are compatible when the program is updated.
    enum {BadTestVersion = 9999,
          GoodMajorVersion = 0,
          GoodMinorVersion = 0};

    static VersionInfo* instance_;

    const QString appName_;
    const QString shortAppName_;
    const QString programName_;
    const QString copyrightYear_;
    const QString version_;
    int versionMajor_;
    int versionMinor_;
};

} // namespace EpCore

#endif
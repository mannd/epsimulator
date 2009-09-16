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

#include "user.h"

#include <cstdlib>

// NB: this file is operating system dependent and won't compile on an
// non-unix system.  
/// TODO When migrating to Windows, will need to modify this file.
#ifdef Q_OS_LINUX
#   include <unistd.h>
#endif

// so far can't get gethostname to work with Windows
#ifdef Q_OS_WIN32
#   include "Winsock2.h"
#endif

using EpCore::User;

/**
 * A singleton instance of User.
 * @return pointer to User.
 */
User* User::instance() {
    return new User;
}

/**
 * The name of the computer running the program.
 * @return the computer (machine) name.
 */
QString User::machineName() const {
    return machineName_;
}

/**
 * The user name.
 * @return either ADMINISTRATOR or the user's login name.
 */
QString User::name() const {
    return isAdministrator_ ? tr("ADMINISTRATOR") 
        :  name_;
}

/**
 * The role of the user.  Used in old style Navigator.
 * @return ADMINSTRATOR or EPSIMUSER.
 */
QString User::role() const {
    return isAdministrator_ ? tr("ADMINISTRATOR") 
        :  tr("EPSIMUSER");
}

User::User() : isAdministrator_(false), name_(std::getenv("USER")) {
#   ifdef Q_OS_WIN32
    machineName_ = "Windows";
#   else
    const size_t length = 255;
    char name[length] = "";
    machineName_ = gethostname(name, length) == 0 ? QString(name) : QString();
#   endif
}
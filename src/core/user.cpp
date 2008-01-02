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

#include <qobject.h>

#include <cstdlib>
// NB: this file is operating system dependent and won't compile on an
// non-unix system.  
/// TODO When migrating to Windows, will need to modify this file.
#include <unistd.h>

User* User::instance_ = 0;

User* User::instance() {
    if (instance_ == 0)
        instance_ = new User;
    return instance_;
}

User::User() : isAdministrator_(false), name_(std::getenv("USER")) {
    const size_t length = 255;
    char name[length] = "";
    machineName_ = gethostname(name, length) == 0 ? QString(name) : QString();
}

User::~User() {}

QString User::machineName() const {
    return machineName_;
}

/**
 * The user name.
 * @return either ADMINISTRATOR or the user's login name.
 */
QString User::name() const {
    return isAdministrator_ ? QObject::tr("ADMINISTRATOR") 
        :  name_;
}

/**
 * The role of the user.  Used in old style Navigator.
 * @return ADMINSTRATOR or EPSIMUSER.
 */
QString User::role() const {
    return isAdministrator_ ? QObject::tr("ADMINISTRATOR") 
        :  QObject::tr("EPSIMUSER");
}


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

#ifndef CORECONSTANTS_H
#define CORECONSTANTS_H

namespace EpCore {
namespace Constants {

#define APP_VERSION_MAJOR 0
#define APP_VERSION_MINOR 1
#define APP_VERSION_RELEASE 1

#define STRINGIFY_INTERNAL(x) #x
#define STRINGIFY(x) STRINGIFY_INTERNAL(x)

#define APP_VERSION STRINGIFY(APP_VERSION_MAJOR) \
    "." STRINGIFY(APP_VERSION_MINOR) \
    "." STRINGIFY(APP_VERSION_RELEASE)

const char* const EPSIM_VERSION     = APP_VERSION;
const char* const EPSIM_AUTHOR      = "EP Studios, Inc.";
const char* const EPSIM_YEAR        = "2010";

// Database -- only define 1 of the below
//#define MYSQL
#define SQLITE

#ifdef MYSQL
#   define BACKEND_DB "QMYSQL"
#elif defined SQLITE
#   define BACKEND_DB "QSQLITE"
#endif

const char* const EPSIM_BACKEND_DB  = BACKEND_DB;
const char* const EPSIM_DB_FILENAME = "epsimulator.db";
const char* const EPSIM_DB_HOSTNAME = "localhost";
const char* const EPSIM_DB_USERNAME = "epsimuser";
const char* const EPSIM_DB_PASSWORD = "epsimpassword";
const char* const EPSIM_CATALOG_DB_FILENAME = "catalog.db";
const char* const EPSIM_SYSTEM_DIRNAME = ".epsimulator";

#undef APP_VERSION
#undef APP_VERSION_MAJOR
#undef APP_VERSION_MINOR
#undef APP_VERSION_RELEASE
#undef STRINGIFY
#undef STRINGIFY_INTERNAL
#undef MYSQL
#undef SQLITE
#undef BACKEND_DB

#ifdef APP_VERSION_BUILD
const char* const APP_VERSION_BUILD_STR = APP_VERSION_BUILD;
#else
const char* const APP_VERSION_BUILD_STR = "";
#endif
// don't #undef APP_VERSION_BUILD

} // Constants
} // EpCore

#endif // CORECONSTANTS_H

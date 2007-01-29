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

/** \file epsim.h
 *  Defines language and app name and version.
*/

#ifndef EPSIM_H
#define EPSIM_H

// APP_NAME is internal program name.  PROGRAM_NAME is the polished program name
/// TODO Consider using epsim instead of epsimulator as APP_NAME
#define APP_NAME "epsimulator"  
#define SHORT_APP_NAME "epsim"
#define PROGRAM_NAME qApp->translate("Global", "EP Simulator")
#define VERSION qApp->translate("Global", "0.1")

// Languages
// Only define 1 of the below
//#define GERMAN
//#define FRENCH
#define ENGLISH

#endif

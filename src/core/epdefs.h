/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

/**
 * @file epdefs.h
 * Contains basic electrophysiologic typedefs, enums, etc.
 */

/// Keep these #defines up to date with the version of the code.
#define VERSION_MAJOR 0
#define VERSION_MINOR 1

// It seems ok for an application like this to leave these typedefs
// at global scope, rather than use using declarations all over the place.
// On the other hand, all could go into EpRecorder namespace, or EpCore,
// or, finally, there could be a Ep namespace for stuff like this.

typedef unsigned int msec;
typedef msec CI;
typedef msec CL;

typedef double Volt;
typedef double MAmp;

typedef unsigned int Interval;  // time in msec resolution
typedef double Duration;  // time in finer resolution, e.g. stimulus PW
typedef double Amplitude;   // amplitude in mAmp







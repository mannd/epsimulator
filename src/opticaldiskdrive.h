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
#ifndef OPTICALDISKDRIVE_H
#define OPTICALDISKDRIVE_H

#include <qstring.h>

/**
Contains OpticalDiskDrive and EmulatedOpticalDiskDrive.  See comments
in class OpticalDisk re the advisability of actually implementing
Optical disk storage.  Although not an ABC, the OpticalDiskDrive
base class will not be used for now.  It will provide the interfaces
we need to emulate in the EmulateOpticalDiskDrive class.

	@author David Mann <mannd@epstudiossoftware.com>
*/

class OpticalDisk;

class OpticalDiskDrive{
public:
    OpticalDiskDrive(const QString& path);

    // checkDrive will be false until we actually create
    // methods to use a real optical disk drive.
    virtual bool checkDrive() {return false;}
    virtual bool testDrive();
    virtual bool setupDrive() {return true;}

    virtual ~OpticalDiskDrive();

protected:
    QString path_;
    OpticalDisk* disk_;  // disk in drive

};

class EmulatedOpticalDiskDrive : public OpticalDiskDrive {
public:
    EmulatedOpticalDiskDrive(const QString& path);

    virtual bool checkDrive();
    virtual bool setupDrive() {return true;}

    virtual ~EmulatedOpticalDiskDrive();

};

#endif

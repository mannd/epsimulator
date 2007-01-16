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
Contains OpticalDiskDrive and EmulatedOpticalDiskDrive.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class OpticalDiskDrive{
public:
    OpticalDiskDrive(const QString& path);

    virtual bool checkDrive();

    virtual ~OpticalDiskDrive();

protected:
    QString path_;

};

class EmulatedOpticalDiskDrive : public OpticalDiskDrive {
public:
    EmulatedOpticalDiskDrive();

    virtual bool checkDrive() {return true;}

    virtual ~EmulatedOpticalDiskDrive();

};

#endif

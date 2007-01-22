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
#include "opticaldisk.h"
#include "options.h"

#include <qsettings.h>

const char* OpticalDisk::labelFileName_ = "label.eps";

OpticalDisk::OpticalDisk() : label_(QString::null), 
                             twoSided_(true), side_("A") {
    /// TODO check if disk is present
}

void OpticalDisk::setSide(QString& side) {
    if (!twoSided_)
        side_ = QString::null;
    else if (side == tr("A") || side == tr("a"))
        side_ = "A";
    else if (side == tr("B") || side == tr("b"))
        side_ = "B";
    else
        side_ = QString::null;
}

OpticalDisk::~OpticalDisk() {
}

EmulatedOpticalDisk::EmulatedOpticalDisk() {
    
}

EmulatedOpticalDisk::~ EmulatedOpticalDisk() {
}



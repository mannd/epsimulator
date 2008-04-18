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

#include "amplifier.h"

#include <cassert>

Amplifier::Amplifier(int n) : numChannels_(n) {
    assert(n == 48 || n == 64 || n == 96 || n == 128);
    switch (numChannels_) {
        case 64 : numCIMConnections_ = 3; break;
        case 96 : numCIMConnections_ = 5; break;
        case 128 : numCIMConnections_ = 7; break;
        case 48 : 
        default: numCIMConnections_ = 2;
    }
}

Amplifier::~Amplifier() {}

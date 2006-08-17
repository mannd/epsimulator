/***************************************************************************
 *   Copyright (C) 2006 by David Mann   *
 *   mannd@epstudiossoftware.com   *
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
#ifndef PATIENT_H
#define PATIENT_H

#include "demographics.h"
class Heart;
class AutonomicTone;

/**
The Patient class includes demographics, baseline autonomic tone, and the heart classes.
Drugs and sedation are part of the specific Study for each patient.
 
	@author David Mann <mannd@epstudiossoftware.com>
*/
class Patient
{
public:
    Patient();
    Demographics demographics;
    
    ~Patient();

private:
    Heart *heart;
    AutonomicTone *autonomicTone;
};

#endif

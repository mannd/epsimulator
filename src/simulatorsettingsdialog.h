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

#ifndef SIMULATORSETTINGSDIALOG_H
#define SIMULATORSETTINGSDIALOG_H

#include "simulatorsettingsdialogbase.h"

class SimulatorSettingsDialog : public SimulatorSettingsDialogBase
{
    Q_OBJECT

public:
    SimulatorSettingsDialog(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

    bool emulateOpticalDrive() const;
    bool emulateDualSidedDrive() const;
    int emulatedOpticalDriveCapacity() const;
    bool oldStyleNavigator() const;

    void setEmulateOpticalDrive(bool);
    void setEmulateDualSidedDrive(bool);
    void setEmulatedOpticalDriveCapacity(int);
    void setOldStyleNavigator(bool);

    ~SimulatorSettingsDialog();
    
public slots:
    virtual void enableDriveEmulation();

};

#endif

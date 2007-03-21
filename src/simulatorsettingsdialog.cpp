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


#include "simulatorsettingsdialog.h"

#include <qcheckbox.h>
#include <qspinbox.h>

SimulatorSettingsDialog::SimulatorSettingsDialog(QWidget* parent, const char* name, WFlags fl)
: SimulatorSettingsDialogBase(parent,name,fl) {
}

bool SimulatorSettingsDialog::emulateOpticalDrive() const {
    return emulateOpticalDriveCheckBox->isChecked();
}

bool SimulatorSettingsDialog::emulateDualSidedDrive() const {
    return emulateDualSidedDriveCheckBox->isEnabled() && 
	    emulateDualSidedDriveCheckBox->isChecked();
}

int SimulatorSettingsDialog::emulatedOpticalDriveCapacity() const {
    if (emulatedOpticalDriveCapacitySpinBox->isEnabled())
        return emulatedOpticalDriveCapacitySpinBox->value();
    else
	return 0;
}

bool SimulatorSettingsDialog::oldStyleNavigator() const {
    return oldStyleNavigatorCheckBox->isChecked();
}

void SimulatorSettingsDialog::setEmulateOpticalDrive(bool emulate) {
    emulateOpticalDriveCheckBox->setChecked(emulate);
    enableDriveEmulation();
}

void SimulatorSettingsDialog::setEmulateDualSidedDrive(bool emulate) {
    if (emulateOpticalDrive())
        emulateDualSidedDriveCheckBox->setChecked(emulate);
}

void SimulatorSettingsDialog::setEmulatedOpticalDriveCapacity(int capacity) {
    if (emulateOpticalDrive())
        emulatedOpticalDriveCapacitySpinBox->setValue(capacity);
}

void SimulatorSettingsDialog::setOldStyleNavigator(bool useOldStyle) {
    oldStyleNavigatorCheckBox->setChecked(useOldStyle);
}

void SimulatorSettingsDialog::enableDriveEmulation()
{
    bool driveEmulation = emulateOpticalDriveCheckBox->isChecked();
    emulateDualSidedDriveCheckBox->setEnabled(driveEmulation);
    emulatedOpticalDriveCapacitySpinBox->setEnabled(driveEmulation);
}

SimulatorSettingsDialog::~SimulatorSettingsDialog() {
}



/*$SPECIALIZATION$*/



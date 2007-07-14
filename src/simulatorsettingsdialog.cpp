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

#include "options.h"

#include <qcheckbox.h>
#include <qlineedit.h>
#include <qspinbox.h>

SimulatorSettingsDialog::SimulatorSettingsDialog(Options* options, 
                                                 QWidget* parent, const char* name, 
                                                 WFlags fl)
                                                 : SimulatorSettingsDialogBase(
                                                   parent,name,fl),
                                                   options_(options) {
    administratorAccountRequiredCheckBox->setChecked(
        options_->administratorAccountRequired());
    hideSimulatorMenuCheckBox->setChecked(
        options_->hideSimulatorMenu());
    setEmulateOpticalDrive(options_->emulateOpticalDrive());
    setDualSidedDrive(options_->dualSidedDrive());
    setEmulatedOpticalDriveCapacity(options_->emulatedOpticalDriveCapacity());
    oldStyleNavigatorCheckBox->setChecked(options_->oldStyleNavigator());
    useLabNameCheckBox->setChecked(options_->useLabName());
    labNameLineEdit->setText(options_->labName());
    permanentDeleteCheckBox->setChecked(options_->permanentDelete());
}

void SimulatorSettingsDialog::setOptions() {
        options_->setAdministratorAccountRequired(
            administratorAccountRequiredCheckBox->isChecked());
        options_->setHideSimulatorMenu(
            hideSimulatorMenuCheckBox->isChecked());
        options_->setEmulateOpticalDrive(emulateOpticalDrive());
        options_->setDualSidedDrive(dualSidedDrive());
        // we will force the drive capacity to be a multiple of 16,
        // it's more computery that way.
        options_->setEmulatedOpticalDriveCapacity((
            emulatedOpticalDriveCapacity() / 16) * 16); 
        options_->setOldStyleNavigator(oldStyleNavigatorCheckBox->isChecked());
        options_->setUseLabName(useLabNameCheckBox->isChecked());
        options_->setLabName(labNameLineEdit->text());
        options_->setPermanentDelete(permanentDeleteCheckBox->isChecked());

        options_->writeSettings();
}

// Keep this function, rather than directly accessing data member.  
// It is used multiple times.  It is inline.
bool SimulatorSettingsDialog::emulateOpticalDrive() const {
    return emulateOpticalDriveCheckBox->isChecked();
}

bool SimulatorSettingsDialog::dualSidedDrive() const {
    return dualSidedDriveCheckBox->isEnabled() && 
	    dualSidedDriveCheckBox->isChecked();
}

int SimulatorSettingsDialog::emulatedOpticalDriveCapacity() const {
    if (emulatedOpticalDriveCapacitySpinBox->isEnabled())
        return emulatedOpticalDriveCapacitySpinBox->value();
    else
	return 0;
}

void SimulatorSettingsDialog::setEmulateOpticalDrive(bool emulate) {
    emulateOpticalDriveCheckBox->setChecked(emulate);
    enableDriveEmulation();
}

void SimulatorSettingsDialog::setDualSidedDrive(bool emulate) {
    if (emulateOpticalDrive())
        dualSidedDriveCheckBox->setChecked(emulate);
}

void SimulatorSettingsDialog::setEmulatedOpticalDriveCapacity(int capacity) {
    if (emulateOpticalDrive())
        emulatedOpticalDriveCapacitySpinBox->setValue(capacity);
}

void SimulatorSettingsDialog::enableDriveEmulation()
{
    bool driveEmulation = emulateOpticalDriveCheckBox->isChecked();
    dualSidedDriveCheckBox->setEnabled(driveEmulation);
    emulatedOpticalDriveCapacitySpinBox->setEnabled(driveEmulation);
}

SimulatorSettingsDialog::~SimulatorSettingsDialog() {
}




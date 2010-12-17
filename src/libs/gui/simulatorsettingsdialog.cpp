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

using EpCore::Options;
using EpGui::SimulatorSettingsDialog;

SimulatorSettingsDialog::SimulatorSettingsDialog(Options* const options,
                                                 QWidget* parent)
    :  QDialog(parent), options_(options) {
    setupUi(this);
    administratorAccountRequiredCheckBox->setChecked(
        options_->administratorAccountRequired);
    hideSimulatorMenuCheckBox->setChecked(
        options_->hideSimulatorMenu);
    switch(options_->diskCache) {
    case Options::NoCache:
        noCacheRadioButton->setChecked(true);
        break;
    case Options::ForceCache:
        forceCacheRadioButton->setChecked(true);
        break;
    case Options::AutoCache:
        autoCacheRadioButton->setChecked(true);
        break;
    }

    setEmulateOpticalDisk(options_->
        opticalDiskFlags.testFlag(Options::Emulation));
    setDualSidedDisk(options_->
        opticalDiskFlags.testFlag(Options::DualSided));
    setEmulatedOpticalDiskCapacity(options_->emulatedOpticalDiskCapacity);
    oldStyleNavigatorCheckBox->setChecked(options_->oldStyleNavigator);
    newStyleBlueBarCheckBox->setChecked(options_->bluePanelStyle ==
        Options::TransparentButtons);
    bluePanelTweakCheckBox->setChecked(options_->bluePanelTweak);

    useLabNameCheckBox->setChecked(options_->useLabName);
    labNameLineEdit->setText(options_->labName);
    permanentDeleteCheckBox->setChecked(options_->permanentDelete);
    showTileCascadeCheckBox->setChecked(options_->screenFlags.testFlag(
            Options::ShowTileCascade));
    emulateWindowsManagerCheckBox->setChecked(options_->screenFlags.testFlag(
        Options::EmulateWindowsManager));
    emulatePruckaTilingCheckBox->setChecked(options_->screenFlags.testFlag(
        Options::EmulatePruckaTiling));
    immovablePatientStatusBarCheckBox->
        setChecked(options_->recorderFlags.testFlag(Options::ImmovablePatientStatusBar));
    patientStatusBarHasTitleCheckBox->
        setChecked(options_->recorderFlags.testFlag(Options::PatientStatusBarHasTitle));
    recorderHasStatusBarCheckBox->setChecked(options_->recorderFlags.testFlag(
        Options::RecorderHasStatusBar));
    int index = 0;
    switch (options_->numChannels) {
        case 48 :
            index = 1;
            break;
        case 64 : 
            index = 2;
            break;
        case 96 : 
            index = 3;
            break;
        case 128 : 
            index = 4;
            break;
        case 32 :
        default : 
            index = 0;
    }
    amplifierTypeComboBox->setCurrentIndex(index);

    tabWidget->setCurrentIndex(0);

    connect(emulateOpticalDiskCheckBox, SIGNAL(stateChanged(int)), 
        this, SLOT(enableDiskEmulation())); 
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

SimulatorSettingsDialog::~SimulatorSettingsDialog() {}

void SimulatorSettingsDialog::setOptions() {
    options_->administratorAccountRequired =
        administratorAccountRequiredCheckBox->isChecked();
    options_->hideSimulatorMenu =
        hideSimulatorMenuCheckBox->isChecked();
    if (diskCacheButtonGroup->checkedButton() == noCacheRadioButton)
        options_->diskCache = Options::NoCache;
    else if (diskCacheButtonGroup->checkedButton() == forceCacheRadioButton)
        options_->diskCache = Options::ForceCache;
    else
        options_->diskCache = Options::AutoCache;
    setFlag(options_->opticalDiskFlags, Options::Emulation,
        emulateOpticalDisk());;
    setFlag(options_->opticalDiskFlags, Options::DualSided,
        dualSidedDisk());
    // we will force the drive capacity to be a multiple of 16,
    // it's more computery that way.
    options_->emulatedOpticalDiskCapacity =
        (emulatedOpticalDiskCapacity() / 16) * 16; 
    options_->oldStyleNavigator = oldStyleNavigatorCheckBox->
        isChecked();
    options_->bluePanelStyle = newStyleBlueBarCheckBox->isChecked() ?
        Options::TransparentButtons : Options::OpaqueButtons;
    options_->bluePanelTweak = bluePanelTweakCheckBox->isChecked();
    
    options_->useLabName = useLabNameCheckBox->isChecked();
    options_->labName = labNameLineEdit->text();
    options_->permanentDelete = permanentDeleteCheckBox->isChecked();
    setFlag(options_->screenFlags, Options::ShowTileCascade,
            showTileCascadeCheckBox->isChecked());
    setFlag(options_->screenFlags, Options::EmulateWindowsManager,
        emulateWindowsManagerCheckBox->isChecked());
    setFlag(options_->screenFlags, Options::EmulatePruckaTiling,
        emulatePruckaTilingCheckBox->isChecked());
    setFlag(options_->recorderFlags, Options::ImmovablePatientStatusBar,
        immovablePatientStatusBarCheckBox->isChecked());
    setFlag(options_->recorderFlags, Options::PatientStatusBarHasTitle,
        patientStatusBarHasTitleCheckBox->isChecked());
    setFlag(options_->recorderFlags, Options::RecorderHasStatusBar,
        recorderHasStatusBarCheckBox->isChecked());
    int index = amplifierTypeComboBox->currentIndex();
    switch (index) {
        case 1 :
            options_->numChannels = 48;
        case 2 :
            options_->numChannels = 64;
            break;
        case 3 :
            options_->numChannels = 96;
            break;
        case 4 :
            options_->numChannels = 128;
            break;
        case 0 :
        default :
            options_->numChannels = 32;
    }
}

void SimulatorSettingsDialog::removeNavigatorTab() {
    tabWidget->removeTab(1);
}

void SimulatorSettingsDialog::setEmulateOpticalDisk(bool emulate) {
    emulateOpticalDiskCheckBox->setChecked(emulate);
    enableDiskEmulation();
}

void SimulatorSettingsDialog::setDualSidedDisk(bool emulate) {
    if (emulateOpticalDisk())
        dualSidedDiskCheckBox->setChecked(emulate);
}

void SimulatorSettingsDialog::setEmulatedOpticalDiskCapacity(int capacity) {
    if (emulateOpticalDisk())
        emulatedOpticalDiskCapacitySpinBox->setValue(capacity);
}

bool SimulatorSettingsDialog::dualSidedDisk() const {
    return dualSidedDiskCheckBox->isEnabled() && 
	    dualSidedDiskCheckBox->isChecked();
}

int SimulatorSettingsDialog::emulatedOpticalDiskCapacity() const {
    if (emulatedOpticalDiskCapacitySpinBox->isEnabled())
        return emulatedOpticalDiskCapacitySpinBox->value();
    else
        return 0;
}

void SimulatorSettingsDialog::enableDiskEmulation() {
    bool diskEmulation = emulateOpticalDiskCheckBox->isChecked();
    dualSidedDiskCheckBox->setEnabled(diskEmulation);
    emulatedOpticalDiskCapacitySpinBox->setEnabled(diskEmulation);
    if (!diskEmulation)
         setEmulatedOpticalDiskCapacity(0);
}

void SimulatorSettingsDialog::on_emulateWindowsManagerCheckBox_stateChanged(int state) {
    // Prucka tiling happens with Prucka windows manager emulation
    // so show it and make it unchangable.
    if (state == Qt::Checked) {
        emulatePruckaTilingCheckBox->setChecked(true);
        emulatePruckaTilingCheckBox->setEnabled(false);
    }
    else 
        // enable Prucka tiling check box, should be whatever it last was set as. 
        emulatePruckaTilingCheckBox->setEnabled(true);
}

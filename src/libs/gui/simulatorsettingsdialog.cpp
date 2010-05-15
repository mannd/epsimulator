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

SimulatorSettingsDialog::SimulatorSettingsDialog(QWidget* parent)
    :  QDialog(parent) {
    setupUi(this);
    administratorAccountRequiredCheckBox->setChecked(
        epOptions->administratorAccountRequired);
    hideSimulatorMenuCheckBox->setChecked(
        epOptions->hideSimulatorMenu);
    switch(epOptions->diskCache) {
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

    setEmulateOpticalDisk(epOptions->
        opticalDiskFlags.testFlag(Options::Emulation));
    setDualSidedDisk(epOptions->
        opticalDiskFlags.testFlag(Options::DualSided));
    setEmulatedOpticalDiskCapacity(epOptions->emulatedOpticalDiskCapacity);
    oldStyleNavigatorCheckBox->setChecked(epOptions->oldStyleNavigator);
    newStyleBlueBarCheckBox->setChecked(epOptions->bluePanelStyle ==
        Options::TransparentButtons);
    bluePanelTweakCheckBox->setChecked(epOptions->bluePanelTweak);

    useLabNameCheckBox->setChecked(epOptions->useLabName);
    labNameLineEdit->setText(epOptions->labName);
    permanentDeleteCheckBox->setChecked(epOptions->permanentDelete);
    emulateTwoScreensCheckBox->setChecked(epOptions->screenFlags.testFlag(
        Options::EmulateTwoScreens));
    emulateOneScreenCheckBox->setChecked(epOptions->screenFlags.testFlag(
        Options::EmulateOneScreen));
    useTwoRecorderWindowsCheckBox->setChecked(epOptions->screenFlags.testFlag(
            Options::TwoRecorderWindows));
    emulateWindowsManagerCheckBox->setChecked(epOptions->screenFlags.testFlag(
        Options::EmulateWindowsManager));
    emulatePruckaTilingCheckBox->setChecked(epOptions->screenFlags.testFlag(
        Options::EmulatePruckaTiling));
    immovablePatientStatusBarCheckBox->
        setChecked(epOptions->recorderFlags.testFlag(Options::ImmovablePatientStatusBar));
    patientStatusBarHasTitleCheckBox->
        setChecked(epOptions->recorderFlags.testFlag(Options::PatientStatusBarHasTitle));
    recorderHasStatusBarCheckBox->setChecked(epOptions->recorderFlags.testFlag(
        Options::RecorderHasStatusBar));
    int index = 0;
    switch (epOptions->numChannels) {
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
    epOptions->administratorAccountRequired =
        administratorAccountRequiredCheckBox->isChecked();
    epOptions->hideSimulatorMenu =
        hideSimulatorMenuCheckBox->isChecked();
    if (diskCacheButtonGroup->checkedButton() == noCacheRadioButton)
        epOptions->diskCache = Options::NoCache;
    else if (diskCacheButtonGroup->checkedButton() == forceCacheRadioButton)
        epOptions->diskCache = Options::ForceCache;
    else
        epOptions->diskCache = Options::AutoCache;
    setFlag(epOptions->opticalDiskFlags, Options::Emulation,
        emulateOpticalDisk());;
    setFlag(epOptions->opticalDiskFlags, Options::DualSided,
        dualSidedDisk());
    // we will force the drive capacity to be a multiple of 16,
    // it's more computery that way.
    epOptions->emulatedOpticalDiskCapacity =
        (emulatedOpticalDiskCapacity() / 16) * 16; 
    epOptions->oldStyleNavigator = oldStyleNavigatorCheckBox->
        isChecked();
    epOptions->bluePanelStyle = newStyleBlueBarCheckBox->isChecked() ?
        Options::TransparentButtons : Options::OpaqueButtons;
    epOptions->bluePanelTweak = bluePanelTweakCheckBox->isChecked();
    
    epOptions->useLabName = useLabNameCheckBox->isChecked();
    epOptions->labName = labNameLineEdit->text();
    epOptions->permanentDelete = permanentDeleteCheckBox->isChecked();
    setFlag(epOptions->screenFlags, Options::EmulateTwoScreens,
        emulateTwoScreensCheckBox->isChecked());
    setFlag(epOptions->screenFlags, Options::EmulateOneScreen,
        emulateOneScreenCheckBox->isChecked());
    setFlag(epOptions->screenFlags, Options::TwoRecorderWindows,
            useTwoRecorderWindowsCheckBox->isChecked());
    setFlag(epOptions->screenFlags, Options::EmulateWindowsManager,
        emulateWindowsManagerCheckBox->isChecked());
    setFlag(epOptions->screenFlags, Options::EmulatePruckaTiling,
        emulatePruckaTilingCheckBox->isChecked());
    setFlag(epOptions->recorderFlags, Options::ImmovablePatientStatusBar,
        immovablePatientStatusBarCheckBox->isChecked());
    setFlag(epOptions->recorderFlags, Options::PatientStatusBarHasTitle,
        patientStatusBarHasTitleCheckBox->isChecked());
    setFlag(epOptions->recorderFlags, Options::RecorderHasStatusBar,
        recorderHasStatusBarCheckBox->isChecked());
    int index = amplifierTypeComboBox->currentIndex();
    switch (index) {
        case 1 :
            epOptions->numChannels = 48;
        case 2 :
            epOptions->numChannels = 64;
            break;
        case 3 :
            epOptions->numChannels = 96;
            break;
        case 4 :
            epOptions->numChannels = 128;
            break;
        case 0 :
        default :
            epOptions->numChannels = 32;
    }

    epOptions->writeSettings();
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

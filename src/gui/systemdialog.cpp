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
#include "systemdialog.h"

#include "actions.h"
#include "options.h"

#include <QCheckBox>
#include <QCoreApplication>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

/// TODO below is Linux/Unix specific, needs generalization.
#include <sys/vfs.h>

#ifndef NDEBUG
#include <iostream>
#endif

SystemDialog::SystemDialog(Options* options, const QString& path,
                           const QString& label, 
                           const QString& side,
                           QWidget *parent)
                           : QDialog(parent),
                           options_(options), path_(path) {
    setupUi(this);
    enableExportFilePathLineEdit();
    enableNetworkStudyPathLineEdit();
    // set up dialog here, from system settings on disk
    opticalStudyPathLineEdit->setText(options_->opticalStudyPath());
    networkStudyPathLineEdit->setText(options_->networkStudyPath());
    exportFilePathLineEdit->setText(options_->exportFilePath());
    enableAcquisitionCheckBox->setChecked(options_->enableAcquisition());
    setEnableFileExport(options_->enableFileExport());
    setEnableNetworkStorage(options_->enableNetworkStorage());
    studyPathLabel->setText(studyPathLabel->text().arg(path));
    diskLabel->setText(diskLabel->text().arg(label));
    long kBytes = diskFreeSpace(options_->opticalStudyPath());
    QString space;
    space.setNum(kBytes);
    space += "K";
    QString time;
    time.setNum(timeRemaining(kBytes));
    time += " min";
    spaceTimeLabel->setText(spaceTimeLabel->text().arg(space + " " + time));
    sideLabel->setText(sideLabel->text().arg(side.isEmpty() ? 
        tr("None") : side)); 
    connect(opticalPathBrowsePushButton, SIGNAL(clicked()), this,
        SLOT(opticalStudyPathBrowse()));
    connect(networkPathBrowsePushButton, SIGNAL(clicked()), this,
        SLOT(networkStudyPathBrowse()));
    connect(exportPathBrowsePushButton, SIGNAL(clicked()), this,
        SLOT(exportFilePathBrowse()));
    connect(enableNetworkStorageCheckBox, SIGNAL(stateChanged(int)), 
        this, SLOT(enableNetworkStudyPathLineEdit()));
    connect(enableFileExportCheckBox, SIGNAL(stateChanged(int)), 
        this, SLOT(enableExportFilePathLineEdit()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

SystemDialog::~SystemDialog() {}

// This is deprecated, shouldn't be needed.
void SystemDialog::removeFilePathsTab() {
    filePathsTabWidget->removeTab(0);
}

void SystemDialog::setOptions() {
    options_->setOpticalStudyPath(opticalStudyPathLineEdit->text());
    options_->setNetworkStudyPath(networkStudyPathLineEdit->text());
    options_->setExportFilePath(exportFilePathLineEdit->text());
    options_->setEnableAcquisition(enableAcquisitionCheckBox->isChecked());
    options_->setEnableFileExport(enableFileExportCheckBox->isChecked());
    options_->setEnableNetworkStorage(enableNetworkStorageCheckBox->
                                      isChecked()); 

    options_->writeSettings();
}


void SystemDialog::opticalStudyPathBrowse() {
    EpGui::browseFilePaths(this, opticalStudyPathLineEdit);
}

void SystemDialog::networkStudyPathBrowse() {
    EpGui::browseFilePaths(this, networkStudyPathLineEdit);
}

void SystemDialog::exportFilePathBrowse() {
    EpGui::browseFilePaths(this, exportFilePathLineEdit);
}

void SystemDialog::enableExportFilePathLineEdit() {
    exportFilePathLineEdit->setEnabled(enableFileExportCheckBox->isChecked());
    exportPathBrowsePushButton->setEnabled(enableFileExportCheckBox->
                                           isChecked());
}

void SystemDialog::enableNetworkStudyPathLineEdit() {
    networkStudyPathLineEdit->setEnabled(enableNetworkStorageCheckBox->
                                         isChecked());
    networkPathBrowsePushButton->setEnabled(enableNetworkStorageCheckBox->
                                            isChecked());
}

void SystemDialog::setEnableNetworkStorage(bool enabled) {
    enableNetworkStorageCheckBox->setChecked(enabled);
    enableNetworkStudyPathLineEdit();
}

void SystemDialog::setEnableFileExport(bool enabled) {
    enableFileExportCheckBox->setChecked(enabled);
    enableExportFilePathLineEdit();
}

long SystemDialog::diskFreeSpace(const QString& path) const {
    int emulatedDiskMBytes = options_->emulatedOpticalDriveCapacity();
    if (emulatedDiskMBytes  > 0) 
        return emulatedDiskMBytes * 1024;
    struct statfs s;
    long blocksFree = 0;
    long blockSize = 0;
    if (statfs(path.toLatin1().constData(), &s) == 0) {
        blocksFree = s.f_bavail;    // blocks available to non-su
        blockSize = s.f_bsize;
    }
    long kBytes = blockSize / 1024;
    return blocksFree * kBytes;
}

long SystemDialog::timeRemaining(long kBytes) const {
    /// TODO This value below is totally arbitrary, will need to 
    /// revise it once we get a better idea what it should be.
    /// In the meantime, well this is just a simulator!
    const double secPerKbyte = 0.1;
    return static_cast<long>(secPerKbyte * kBytes / 60);
}



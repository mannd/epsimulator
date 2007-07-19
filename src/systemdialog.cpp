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

#include "options.h"

#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>

SystemDialog::SystemDialog(Options* options, const QString& path,
                           const QString& label, const QString& spaceTime,
                           const QString& side,
                           QWidget *parent, const char *name)
                           : SystemDialogBase(parent, name),
                           options_(options) {
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
    spaceTimeLabel->setText(spaceTimeLabel->text().arg(spaceTime));
    sideLabel->setText(sideLabel->text().arg(side.isEmpty() ? tr("None") : side)); 
}

void SystemDialog::setOptions() {
    options_->setOpticalStudyPath(opticalStudyPathLineEdit->text());
    options_->setNetworkStudyPath(networkStudyPathLineEdit->text());
    options_->setExportFilePath(exportFilePathLineEdit->text());
    options_->setEnableAcquisition(enableAcquisitionCheckBox->isChecked());
    options_->setEnableFileExport(enableFileExportCheckBox->isChecked());
    options_->setEnableNetworkStorage(
	enableNetworkStorageCheckBox->isChecked()); 
    options_->writeSettings();
}

void SystemDialog::opticalStudyPathBrowse() {
    browseFilePaths(opticalStudyPathLineEdit);
}

void SystemDialog::networkStudyPathBrowse() {
    browseFilePaths(networkStudyPathLineEdit);
}

void SystemDialog::exportFilePathBrowse() {
    browseFilePaths(exportFilePathLineEdit);
}

void SystemDialog::enableExportFilePathLineEdit() {
    exportFilePathLineEdit->setEnabled(enableFileExportCheckBox->isChecked());
    exportPathBrowsePushButton->setEnabled(enableFileExportCheckBox->isChecked());
}

void SystemDialog::enableNetworkStudyPathLineEdit() {
    networkStudyPathLineEdit->setEnabled(enableNetworkStorageCheckBox->isChecked());
    networkPathBrowsePushButton->setEnabled(enableNetworkStorageCheckBox->isChecked());
}

void SystemDialog::setEnableNetworkStorage(bool enabled) {
    enableNetworkStorageCheckBox->setChecked(enabled);
    enableNetworkStudyPathLineEdit();
}

void SystemDialog::setEnableFileExport(bool enabled) {
    enableFileExportCheckBox->setChecked(enabled);
    enableExportFilePathLineEdit();
}

void SystemDialog::browseFilePaths(QLineEdit* lineEdit) {
    QFileDialog *fd = new QFileDialog(this, 0, true);
    fd->setMode(QFileDialog::Directory);
    fd->setDir(lineEdit->text());
    if (fd->exec() == QDialog::Accepted) {
        lineEdit->setText(fd->selectedFile());
    }
}

SystemDialog::~SystemDialog() {
}

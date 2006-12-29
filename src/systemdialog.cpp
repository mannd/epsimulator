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

#include <qfiledialog.h>

SystemDialog::SystemDialog(QWidget *parent, const char *name)
    :SystemDialogBase(parent, name) {
    // set up dialog here, from system settings on disk
}

void SystemDialog::localStudyPathBrowse() {
    browseFilePaths(localStudyPathLineEdit);
}

void SystemDialog::opticalStudyPathBrowse() {
    browseFilePaths(opticalStudyPathLineEdit);
}

void SystemDialog::networkStudyPathBrowse() {
    browseFilePaths(networkStudyPathLineEdit);
}

void SystemDialog::browseFilePaths(QLineEdit* lineEdit) {
    QFileDialog *fd = new QFileDialog(this, 0, true);
    fd->setMode(QFileDialog::Directory);
    fd->setDir(lineEdit->text());
    if (fd->exec() == QDialog::Accepted) {
        lineEdit->setText(fd->selectedFile());
    }
}

void SystemDialog::setLocalStudyPath(QString path) {
    localStudyPathLineEdit->setText(path);
}

void SystemDialog::setOpticalStudyPath(QString path) {
    opticalStudyPathLineEdit->setText(path);
}

void SystemDialog::setNetworkStudyPath(QString path) {
    networkStudyPathLineEdit->setText(path);
}

SystemDialog::~SystemDialog() {
}


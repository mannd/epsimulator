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

#include "changepassworddialog.h"

#include "passwordhandler.h"
#include "versioninfo.h"

#include <QMessageBox>

using EpCore::PasswordHandler;
using EpGui::ChangePasswordDialog;

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent)
    : QDialog(parent) {
    setupUi(this);
    pwHandler_ = new PasswordHandler;
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

ChangePasswordDialog::~ChangePasswordDialog() {
    delete pwHandler_;
}

bool ChangePasswordDialog::testPasswordsEqual() const {
    return newLineEdit->text() == confirmNewLineEdit->text();
}

void ChangePasswordDialog::clear() {
    oldLineEdit->clear();
    newLineEdit->clear();
    confirmNewLineEdit->clear();
    oldLineEdit->setFocus();
}

void ChangePasswordDialog::accept() {
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(EpCore::VersionInfo::instance()->programName());
    if (!pwHandler_->testPassword(oldLineEdit->text())) {
        msgBox.setText(tr("Wrong Password"));
        msgBox.setInformativeText(tr("Old password is wrong. "
                                "Please try again."));
        msgBox.exec();
        clear();
    }
    else if (newLineEdit->text().isEmpty()) {
        msgBox.setText(tr("Blank Password Not Allowed"));
        msgBox.setInformativeText(tr("A blank password is not allowed. "
                                 "Please try again."));
        msgBox.exec();
        clear();
    }
    else if (testPasswordsEqual())    
        QDialog::accept();
    else {
        msgBox.setText(tr("Passwords Don't Match"));
        msgBox.setInformativeText(tr("The new passwords don't match. "
                                "You may have made a typing error. "
                                "Please try again."));
        msgBox.exec();
        clear();
    }
}

void ChangePasswordDialog::changePassword() const {
    pwHandler_->setPassword(newLineEdit->text());
}

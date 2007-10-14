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

#include <QDialog>
#include <qlineedit.h>
#include <qmessagebox.h>

ChangePasswordDialog::ChangePasswordDialog(Options* options, QWidget *parent)
    : QDialog(parent) {
    setupUi(this);
    pwHandler_ = new PasswordHandler(options);
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
    if (!pwHandler_->testPassword(oldLineEdit->text())) {
        QMessageBox::warning(this, tr("Wrong Password"),
                             tr("Old password is wrong. "
                                "Please try again."));
        clear();
    }
    else if (testPasswordsEqual())    
        accept();
    else {
        QMessageBox::warning(this, tr("Passwords Don't Match"),
                             tr("The new passwords don't match. "
                                "You may have made a typing error. "
                                "Please try again."));
        clear();
    }
}

void ChangePasswordDialog::changePassword() const {
    pwHandler_->setPassword(newLineEdit->text());
}

ChangePasswordDialog::~ChangePasswordDialog() {
    delete pwHandler_;
}


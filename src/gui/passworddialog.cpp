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

#include "passworddialog.h"

#include "passwordhandler.h"

#include <QMessageBox>

using EpGui::PasswordDialog;

PasswordDialog::PasswordDialog(QWidget* parent)
    : QDialog(parent) {
    setupUi(this);
    passwordLineEdit->setText("");
    pwHandler_ = new PasswordHandler();
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

PasswordDialog::~PasswordDialog() {
    delete pwHandler_;
}

void PasswordDialog::setPassword(const QString& pw) {
    passwordLineEdit->setText(pw);
}

bool PasswordDialog::testPassword() {
    return pwHandler_->testPassword(passwordLineEdit->text());
}

void PasswordDialog::accept() {
    if (testPassword())
        QDialog::accept();
    else {
        QMessageBox::warning(this, tr("Wrong Password"),
                             tr("Please retry your password."));
        passwordLineEdit->clear();
    }
}

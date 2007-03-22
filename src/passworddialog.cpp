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

#include "GeneralHashFunctions.h"
#include "options.h"

#include <qlineedit.h>
#include <qmessagebox.h>

PasswordDialog::PasswordDialog(Options* options, QWidget* parent, const char* name)
    : PasswordDialogBase(parent,name, true), options_(options) {
    // must set default password of blank
    passwordLineEdit->setText("");
}

bool PasswordDialog::testPassword() {
    const char* pw = passwordLineEdit->text();
    unsigned int hash = RSHash(pw);
    return hash == options_->passwordHash();
}

void PasswordDialog::accept() {
    if (testPassword())    
        PasswordDialogBase::accept();
    else
        QMessageBox::warning(this, tr("Wrong Password"),
                             tr("Please retry your password."));
}

PasswordDialog::~PasswordDialog() {
}

/*$SPECIALIZATION$*/



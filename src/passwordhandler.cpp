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
#include "passwordhandler.h"

#include "options.h"

#include <qstring.h>

PasswordHandler::PasswordHandler(Options* options) :
    options_(options) {
    hash_ = RSHash;
}

void PasswordHandler::setPassword(const QString& pw) {    
    //const char* pw = newLineEdit->text();
    // Don't change this without changing the same function in passworddialog!
    QString h;
    h.setNum(hash_(pw));
    options_->setPasswordHash(h);
    options_->writeSettings();
}

bool PasswordHandler::testPassword(const QString& pw) {
 //   const char* str = pw;
    unsigned int h = hash_(pw);
    return h == options_->passwordHash().toUInt();
}

PasswordHandler::~PasswordHandler() {
}



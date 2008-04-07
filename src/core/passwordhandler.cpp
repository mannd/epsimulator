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

#include <QObject>

PasswordHandler::PasswordHandler() :
    options_(Options::instance()), hash_(QCryptographicHash::Sha1){
    // set original password to "admin"
    if (options_->passwordHash() == "0")
        setPassword(QObject::tr("admin"));
}

PasswordHandler::~PasswordHandler() {}

void PasswordHandler::setPassword(const QString& pw) {    
    hash_.reset();
    hash_.addData(pw.toAscii());
    options_->setPasswordHash(hash_.result());
    options_->writeSettings();
}

bool PasswordHandler::testPassword(const QString& pw) {
    hash_.reset();
    hash_.addData(pw.toAscii());
    return hash_.result() == options_->passwordHash().toAscii();
}


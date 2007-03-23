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
#ifndef PASSWORDHANDLER_H
#define PASSWORDHANDLER_H

#include "GeneralHashFunctions.h"

class Options;

class QString;


/**
Encapsulates password alorithm and processing.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class PasswordHandler {
public:
    PasswordHandler(Options* o);

    void setPassword(const QString& pw);
    bool testPassword(const QString& pw);

    ~PasswordHandler();

private:
    Options* options_;
    HashFunction hash_;

};

#endif

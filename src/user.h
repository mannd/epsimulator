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
#ifndef USER_H
#define USER_H

#include <qstring.h>

/**
Encapsulates info on the program user.  Is singleton class, only 1 user obviously.
Also, perhaps overambitiously contains host machine info.

	@author David Mann <mannd@epstudiossoftware.com>
*/
class User {
public:
    static User* instance();
    void setAdministrator(bool isAdministrator) 
        {isAdministrator_ = isAdministrator;}

    void makeAdministrator(bool isAdministrator) {
        isAdministrator_ = isAdministrator;}

    bool isAdministrator() const {return isAdministrator_;}
    QString machineName() const;
    QString name() const;
    QString role() const;  // returns EPSIMUSER or ADMINISTRATOR

    void destroy() {delete instance_; instance_ = 0;}

private:
    User();
    ~User();
    static User* instance_;

    bool isAdministrator_;
    QString name_;

};

#endif

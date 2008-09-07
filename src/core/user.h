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

#include <QCoreApplication>
#include <QString>

namespace EpCore {

/**
 * The user of the program.  In CardioLab V5 there are 2 users, the
 * regular user and the administrator.  In V6, there are apparently
 * multiple users, with different roles, and probably simultaneous
 * logins from multiple terminals in a single study.  Given this,
 * and the disadvantages of Singletons, User is no longer a 
 * Singleton class, though I am still using the factory method.
 * Also, perhaps contains host machine info.
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class User {
    Q_DECLARE_TR_FUNCTIONS(User)

public:
    static User* instance();
    ~User();

    void makeAdministrator(bool isAdministrator) {
        isAdministrator_ = isAdministrator;}

    bool isAdministrator() const {return isAdministrator_;}
    QString machineName() const;
    QString name() const;
    QString role() const;  // returns EPSIMUSER or ADMINISTRATOR


private:
    User();

    bool isAdministrator_;
    QString name_;
    QString machineName_;
};

}

#endif

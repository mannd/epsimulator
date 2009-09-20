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

#include <QCoreApplication>
#include <QCryptographicHash>

namespace EpCore {

class Options;

/**
 * Encapsulates password alorithm and processing.
 *
 * @author David Mann <mannd@epstudiossoftware.com>
 */
class PasswordHandler {
    Q_DECLARE_TR_FUNCTIONS(PasswordHandler)

public:
    PasswordHandler();
    ~PasswordHandler();

    void setPassword(const QString&);
    bool testPassword(const QString&);

private:
    enum {MagicNumber = 0x33f788f6};  // for study.dat file

    Options* options_;
    QCryptographicHash hash_;
};

} // namespace EpCore

#endif

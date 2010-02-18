/***************************************************************************
 *   Copyright (C) 2010 by EP Studios, Inc.                                *
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

#ifndef GUIUTILITIES_H
#define GUIUTILITIES_H

#ifdef Q_OS_UNIX
#include <QtGui/QX11Info>
#endif

namespace EpGui {
    int appDpiX() {
#ifdef Q_OS_UNIX
        QX11Info x;
        return x.appDpiX();
#endif
#ifdef Q_OS_WIN32
        // int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
        // return dpiX;
#endif
        return 0;
    }

    int appDpiY() {
#ifdef Q_OS_UNIX
        QX11Info x;
        return x.appDpiY();
#endif
#ifdef Q_OS_WIN32
        // int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
        // return dpiY;
#endif
        return 0;
    }
}

#endif // GUIUTILITIES_H

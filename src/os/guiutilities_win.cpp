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

#include "guiutilities.h"

#include <QtCore/QSettings>
#include <QtGui/QWidget>

namespace EpGui {

int appDpiX() {
    // int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    // return dpiX;
    return 0;
}

int appDpiY() {
    // int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
    // return dpiY;
    return 0;
}

void osDependentRestoreGeometry(QWidget* window,
                                QSettings& settings,
                                const QString& subkey) {
    if (!subKey.isEmpty())
        settings.beginGroup(subKey);
    window->restoreGeometry(settings.value("geometry")
                            .toByteArray());
    if (!subKey.isEmpty())
        settings.endGroup();
}

} // namespace EpGui


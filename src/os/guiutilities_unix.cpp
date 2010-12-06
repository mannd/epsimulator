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
#include <QtGui/QX11Info>

#include <QtDebug>

namespace EpGui {

int appDpiX() {
    QX11Info x;
    return x.appDpiX();
}

int appDpiY() {
    QX11Info x;
    return x.appDpiY();
}

void osDependentRestoreGeometry(QWidget* window,
                                QSettings& settings,
                                const QString& subKey) {
    // restoreGeometry doesn't work on X11
    // but resize() and move() work ok.
    // see Window Geometry section of Qt Reference Doc
    if (!subKey.isEmpty())
        settings.beginGroup(subKey);
    window->resize(settings.value("size").toSize());
    window->move(settings.value("pos").toPoint());
    if (!subKey.isEmpty())
        settings.endGroup();
}

} // namespace EpGui

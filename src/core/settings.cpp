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
#include "settings.h"

/**
 * Constructor that sets up QSettings.
 */
Settings::Settings() : QSettings("EPStudios", "EPSimulator") {
    beginGroup("/EPSimulator");
}


/**
 * Stores settings to a specific file.  Useful for saving specific
 * settings, e.g. window layouts.
 * @param fileName If format is QSettings::IniFormat this is a full
 * file path, otherwise (QSettings::NativeFormat) system specific
 * location.
 * @param format IniFormat or NativeFormat.
 * @param parent parent of this settings object.
 */
Settings::Settings(const QString& fileName, Format format, 
    QObject* parent) : QSettings(fileName, format, parent) {}



Settings::~Settings() {
    endGroup();
}



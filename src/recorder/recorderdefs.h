/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

#ifndef RECORDERDEFS_H
#define RECORDERDEFS_H

#include <QString>

namespace EpRecorder {

    enum SaveStatus {NoSave, ManualSave, AutoSave, ExitSave};
    enum DisplayWindowType {RealTime, Review1, Review2, Log, Ablation, Alignment,
                        Map, Holter, Plot, Macro, Image1, Image2, ImageLibrary, 
                        Other, LastDisplayWindow = Other};

    // settings keys for the various display windows
    const QString realTimeWindowKey = "realTimeWindow";
    const QString review1WindowKey = "review1Window";
    const QString review2WindowKey = "review2Window";
    const QString logWindowKey = "logWindow";
    /// TODO add other windows
}

#endif

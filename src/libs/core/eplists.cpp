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

#include "eplists.h"

#include "fileutilities.h"

using EpCore::EpLists;

const QString EpLists::fileName_ = "eplists.dat";

EpLists::EpLists() {
    load();
    if (map_.isEmpty()) {
        makeDefaultEpLists();
        save();
    }
}

void EpLists::load() {
    try {
        EpCore::loadSystemData(MagicNumber, fileName_,
                               map_, EpCore::Options::instance());
    }
    catch (EpCore::IoError&) {
        // ignore failure to read, just leave map_ empty
    }

}

void EpLists::save() {
    EpCore::saveSystemData(MagicNumber, fileName_,
                           map_, EpCore::Options::instance());
}

void EpLists::makeDefaultEpLists() {
    map_.clear();
    QStringList pacingSites;
    pacingSites << tr("HRA") << tr("CS") << tr("RVa") << tr("RVot")
            << tr("ABL") << tr("HIS") <<tr("None");
    map_["PacingSites"] = pacingSites;
}

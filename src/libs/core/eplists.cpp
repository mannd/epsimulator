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
#include "options.h"

using EpCore::EpLists;

const QString EpLists::fileName_ = "eplists.dat";

EpLists::EpLists() {
    lookupMap_[PacingSites] = "PacingSites";
    lookupMap_[Phases] = "Phases";
    lookupMap_[ArrhythmiaTypes] = "ArrhythmiaTypes";
    lookupMap_[ArrhythmiaTolerances] = "ArrhythmiaTolerances";
    lookupMap_[BlockDescriptions] = "BlockDescriptions";
    lookupMap_[RefractoryLocations] = "RefractoryLocations";
    load();
    if (map_.isEmpty()) {
        makeDefaultEpLists();
        save();
    }
}

void EpLists::load() {
    try {
        EpCore::loadSystemData(MagicNumber, fileName_,
                               map_, epOptions);
    }
    catch (EpCore::IoError&) {
        // ignore failure to read, just leave map_ empty
    }

}

void EpLists::save() {
    EpCore::saveSystemData(MagicNumber, fileName_,
                           map_, epOptions);
}

void EpLists::makeDefaultEpLists() {
    map_.clear();
    QStringList pacingSites;
    pacingSites << tr("HRA") << tr("CS") << tr("RVa") << tr("RVot")
            << tr("ABL") << tr("HIS") <<tr("None");
    map_[lookupMap_[PacingSites]] = pacingSites;
    QStringList phases;
    phases << tr("BASELINE") << tr("Post-Drug") << tr("Post-Ablation")
            << tr("Isoproterenol");
    map_[lookupMap_[Phases]] = phases;
    QStringList arrhythmiaTypes;
    arrhythmiaTypes << tr("SVT") << tr("Sinus Node Reentry")
            << tr("Inappropriate Sinus Tachycardia")
            << tr("Ectopic Reentrant Tachycardia")
            << tr("Multifocal Atrial Tachycardia")
            << tr("Type 1 Atrial Flutter, CW")
            << tr("Type 1 Atrial Flutter, CCW")
            << tr("Type 2 Atrial Flutter")
            << tr("Chronic Atrial Fibrillation")
            << tr("Paroxysmal Atrial Fibrillation")
            << tr("Premature Atrial Complexes")
            << tr("Premature Ventricular Complexes")
            << tr("Nonsustained Ventricular Tachycardia")
            << tr("Sustained Monomorphic Ventricular Tachycardia")
            << tr("Sustained Polymorphic Ventricular Tachycardia")
            << tr("Ventricular Fibrillation")
            << tr("2nd Degree AV Block Mobitz I")
            << tr("2nd Degree AV Block Mobitz II")
            << tr("2nd Degree AV Block 2:1")
            << tr("2nd Degree AV Block High Grade")
            << tr("3rd Degree AV Block")
            << tr("Other");
    map_[lookupMap_[ArrhythmiaTypes]] = arrhythmiaTypes;
    QStringList arrhythmiaTolerances;
    arrhythmiaTolerances << tr("Stable") << tr("Unconscious")
            << tr("Unstable") << tr("Chest Pain")
            << tr("Consious") << tr("SOB")
            << tr("Vagal");
    map_[lookupMap_[ArrhythmiaTolerances]] = arrhythmiaTolerances;
    QStringList blockDescriptions;
    blockDescriptions << tr("AV Nodal Block")
            << tr("1 Degree AV Block")
            << tr("Wenckebach")
            << tr("AV Mobitz II")
            << tr("2:1 AV Block")
            << tr("AV Dissociation")
            << tr("Complete Heart Block")
            << tr("Antegrade Wenckebach")
            << tr("VA Block")
            << tr("Retrograde");
    map_[lookupMap_[BlockDescriptions]] = blockDescriptions;
    QStringList refractoryLocations;
    refractoryLocations << tr("Atrial") << tr("AV Nodal")
            << tr("Retrograde") << tr("Ventricular");
    map_[lookupMap_[RefractoryLocations]] = refractoryLocations;
}

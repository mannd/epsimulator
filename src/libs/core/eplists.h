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

#ifndef EPLISTS_H
#define EPLISTS_H

#include <QtCore/QCoreApplication>
#include <QtCore/QMap>
#include <QtCore/QStringList>

namespace EpCore {

class EpLists {
    Q_DECLARE_TR_FUNCTIONS(EpLists)

public:
    enum EpListType {PacingSites, Phases, ArrhythmiaTypes,
                     ArrhythmiaTolerances, BlockDescriptions,
                     RefractoryLocations};
    EpLists();

    QStringList& operator[](EpListType key) {
        return map_[lookupMap_[key]];}
    void update() {save();}

    static unsigned int magicNumber() {return MagicNumber;}
    static QString fileName() {return fileName_;}

private:
    enum {MagicNumber = 0x99f818f0};

    const static QString fileName_;

    void load();
    void save();

    void makeDefaultEpLists();

    QMap<EpListType, QString> lookupMap_;
    QMap<QString, QStringList> map_;
};

}

#endif // EPLISTS_H

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

#ifndef ITEMLIST_H
#define ITEMLIST_H

// Class template for various lists

#include "fileutilities.h"
#include "options.h"

#include <QtCore/QList>

namespace EpCore {

    template<typename T>
    class ItemList {
    public:
        ItemList();

        T& operator[](const int i) {
            return list_[i];
        }

        void removeAt(int i) {
            list_.removeAt(i);
        }
        void append(const T& value) {
            list_.append(value);
        }

        bool nameIsPresent(const QString& name);
        int size() {return list_.size();}
        void update() {save();}

    private:
        void load();
        void save();

        void makeDefault();

        QList<T> list_;
    };

    template<typename T>
    ItemList<T>::ItemList() {
        load();
        if (list_.isEmpty()) {
            makeDefault();
            save();
        }
    }

    template<typename T>
    bool ItemList<T>::nameIsPresent(const QString &name) {
        for (int i = 0; i < list_.size(); ++i) {
            if (list_.at(i).name() == name)
                return true;
        }
        return false;
    }

    template<typename T>
    void ItemList<T>::load() {
        try {
            EpCore::loadSystemData(T::magicNumber(), T::fileName(),
                                   list_, EpCore::Options::instance());
        }
        catch (EpCore::IoError&) {
            // ignore failure to read, leave list empty
        }
    }

    template<typename T>
    void ItemList<T>::save() {
        EpCore::saveSystemData(T::magicNumber(), T::fileName(),
                               list_, EpCore::Options::instance());
    }

    template<typename T>
    void ItemList<T>::makeDefault() {
        QStringList names = T::defaultNames();
        list_.clear();
        QMutableStringListIterator iter(names);
        while (iter.hasNext()) {
            list_.append(T(iter.next()));
        }
    }
}

#endif

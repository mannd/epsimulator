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
    ItemList(Options* const options)
        : list_(), options_(options) {}

    T& operator[](int i) {
        return list_[i];
    }

    T& operator[](const QString& name);

    T value(int i) const {
        return list_.value(i);
    }

    bool contains(const T& value) const {
        return list_.contains(value);
    }

    void removeAt(int i) {
        list_.removeAt(i);
    }

    void remove(const QString&);

    void append(const T& value) {
        list_.append(value);
    }

    int index(const T& value) const;
    int index(const QString& name) const;

    bool nameIsPresent(const QString& name) const;
    bool duplicate(const T&) const;
    int size() const {return list_.size();}
    void loadList();
    void update() {save();}
    QList<T> list() const {return list_;}

private:
    void load();
    void save();

    void makeDefault();

    QList<T> list_;
    Options* options_;
};

// definitions

template<typename T>
ItemList<T>::ItemList()
    : list_(), options_(0) {}

template<typename T>
void ItemList<T>::loadList() {
    load();
    if (list_.isEmpty()) {
        makeDefault();
        save();
    }
}

template<typename T>
T& ItemList<T>::operator[](const QString& name) {
    for (int i = 0; i < list_.size(); ++i) {
        if (list_.at(i).name() == name)
            return list_[i];
    }
    // return 1st in list if not found
    return list_.front();

}

template<typename T>
void ItemList<T>::remove(const QString& name) {
    for (int i = 0; i < list_.size(); ++i) {
        if (list_.at(i).name() == name)
            list_.removeAt(i);
    }
}

template<typename T>
int ItemList<T>::index(const T& item) const {
    for (int i = 0; i < list_.size(); ++i) {
        if (list_.at(i).name() == item.name())
            return i;
    }
    return -1;
}

template<typename T>
int ItemList<T>::index(const QString &name) const {
    for (int i = 0; i < list_.size(); ++i) {
        if (list_.at(i).name() == name)
            return i;
    }
    return -1;
}

template<typename T>
bool ItemList<T>::duplicate(const T & item) const {
    return nameIsPresent(item.name());
}

template<typename T>
bool ItemList<T>::nameIsPresent(const QString &name) const {
    for (int i = 0; i < list_.size(); ++i) {
        if (list_.at(i).name() == name)
            return true;
    }
    return false;
}

template<typename T>
void ItemList<T>::load() {
    try {
        loadSystemData(T::magicNumber(), T::fileName(),
                               list_, options_);
    }
    catch (IoError&) {
        // ignore failure to read, leave list empty
    }
}


template<typename T>
void ItemList<T>::save() {
    saveSystemData(T::magicNumber(), T::fileName(),
                           list_, options_);
}

template<typename T>
void ItemList<T>::makeDefault() {
    QList<T> defaults = T::defaultItems();
    list_.clear();
    list_ = defaults;
}

}

#endif

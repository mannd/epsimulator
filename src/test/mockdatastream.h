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

#ifndef MOCKDATASTREAM_H
#define MOCKDATASTREAM_H

#include "fileutilities.h"

#include <QDir>

namespace EpCore {

template<typename T>
class MockDataStream : public DataStream<T> {
public:
    MockDataStream();
    virtual void save(const T& data);
    virtual void load(T& data);
private:
    QString tempPath_;
};

// definitions

template<typename T>
MockDataStream<T>::MockDataStream() : tempPath_(QDir::tempPath()) {}

template<typename T>
void MockDataStream<T>::save(const T& data) {
    saveData(joinPaths(tempPath_, T::fileName()), T::magicNumber(), data);
}

template<typename T>
void MockDataStream<T>::load(T& data) {
    loadData(joinPaths(tempPath_, T::fileName()), T::magicNumber(), data);
}

}
#endif // MOCKDATASTREAM_H

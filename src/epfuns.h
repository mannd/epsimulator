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

#ifndef EPFUNS_H
#define EPFUNS_H

#include "error.h"

#include <qdatastream.h>
#include <qfile.h>
#include <qstring.h>

class QLineEdit;
class QWidget;

namespace EpFuns {

template<typename T> 
void loadData(const QString& filePath, unsigned int magicNumber, T& data); 

template<typename T>
void saveData(const QString& filePath, unsigned int magicNumber, const T& data);

void saveMagicNumber(unsigned int magicNumber, QDataStream& out);

/// Major or minor version number == BadTestVersion then version not OK.
/// Version numbers >= the Good... versions are OK.  This system is to 
/// make sure file formats are compatible when the program is updated.
enum {BadTestVersion = 9999, GoodMajorVersion = 0, 
    GoodMinorVersion = 0};

bool versionOk(int major, int minor);

template<typename T> 
void loadData(const QString& filePath, unsigned int magicNumber, T& data) {
    QFile file(filePath);
    if (!file.exists()) {
        if (!file.open(IO_WriteOnly))
            throw EpSim::OpenWriteError(file.name());
        QDataStream out(&file);
        out.setVersion(5);
        saveMagicNumber(magicNumber, out);
        file.close();
    }
    if (!file.open(IO_ReadOnly))
        throw EpSim::OpenReadError(file.name());
    QDataStream in(&file);
    in.setVersion(5);
    Q_UINT32 magic;
    in >> magic;
    if (magic != magicNumber)
        throw EpSim::WrongFileTypeError(file.name());
    Q_UINT32 versionMajor, versionMinor;
    in >> versionMajor >> versionMinor;
    if (!versionOk(versionMajor, versionMinor))
        throw EpSim::WrongEpSimVersionError();
    if (!in.atEnd())
        in >> data;
    if (file.status() != IO_Ok)
        throw EpSim::ReadError(file.name());
    file.close();
}

template<typename T>
void saveData(const QString& filePath, unsigned int magicNumber, const T& data) {
    QFile file(filePath);
    if (!file.open(IO_WriteOnly))
        throw EpSim::OpenWriteError(file.name());
    QDataStream out(&file);
    out.setVersion(5);
    saveMagicNumber(magicNumber, out);
    out << data;
    if (file.status() != IO_Ok)
        throw EpSim::WriteError(file.name());
    file.close();
}

void browseFilePaths(QWidget*, QLineEdit*);

}

#endif



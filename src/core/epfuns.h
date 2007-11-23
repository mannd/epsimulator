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
#include "versioninfo.h"

#include <qdatastream.h>
#include <qfile.h>
#include <QtCore/QString>


namespace EpCore {

template<typename T> 
void loadData(const QString& filePath, unsigned int magicNumber, T& data); 

template<typename T>
void saveData(const QString& filePath, unsigned int magicNumber, const T& data);

void saveMagicNumber(unsigned int magicNumber, QDataStream& out);

template<typename T> 
void loadData(const QString& filePath, unsigned int magicNumber, T& data) {
    QFile file(filePath);
    if (!file.exists()) {
        if (!file.open(QIODevice::WriteOnly))
            throw EpCore::OpenWriteError(file.fileName());
        QDataStream out(&file);
        out.setVersion(5);
        saveMagicNumber(magicNumber, out);
        file.close();
    }
    if (!file.open(QIODevice::ReadOnly))
        throw EpCore::OpenReadError(file.fileName());
    QDataStream in(&file);
    in.setVersion(5);
    quint32 magic;
    in >> magic;
    if (magic != magicNumber)
        throw EpCore::WrongFileTypeError(file.fileName());
    quint32 versionMajor, versionMinor;
    in >> versionMajor >> versionMinor;
    if (!VersionInfo::versionOk(versionMajor, versionMinor))
        throw EpCore::WrongEpSimVersionError();
    if (!in.atEnd())
        in >> data;
    if (file.error() != QFile::NoError)
        throw EpCore::ReadError(file.fileName());
    file.close();
}

template<typename T>
void saveData(const QString& filePath, unsigned int magicNumber, const T& data) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        throw EpCore::OpenWriteError(file.fileName());
    QDataStream out(&file);
    out.setVersion(5);
    saveMagicNumber(magicNumber, out);
    out << data;
    if (file.error() != QFile::NoError)
        throw EpCore::WriteError(file.fileName());
    file.close();
}


}  // namespace EpCore

#endif



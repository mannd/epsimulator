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

#ifndef FILEUTILITIES_H
#define FILEUTILITIES_H

#include "error.h"
#include "options.h"
#include "versioninfo.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>

class QDir;
class QString;

namespace EpCore {

/**
 * Loads data from QDataStream identified by filePath.  File must be proper
 * type, matching magicNumber.
 */
template<typename T> 
void loadData(const QString& filePath, unsigned int magicNumber, T& data); 

/**
 * Save data to QDataStream identified by filePath.  magicNumber is saved to
 * stream for identification purposes.
 */
template<typename T> 
void saveData(const QString& filePath, unsigned int magicNumber, const T& data);

void saveMagicNumber(unsigned int magicNumber, QDataStream& out);

/**
 * Saves data to both Network and local System paths, if network storage
 * enabled, otherwise only to local System path.
 */
template<typename T>
void saveSystemData(unsigned int magicNumber, const QString& fileName, const T& data, 
    Options* options);

/**
 * Loads data from the Network path if network storage enabled, otherwise
 * loads from the System path.
 */
template<typename T>
void loadSystemData(unsigned int magicNumber, const QString& fileName, 
    T& data, Options* options);

void deleteDir(const QString& path);
void copyDir(const QString& sourcePath, const QString& destinationPath);
QDir directoryOf(const QString& subdir);
QDir rootDirectory();
QDir systemDirectory();

// definitions

template<typename T> 
void loadData(const QString& filePath, unsigned int magicNumber, T& data) {
    QFile file(filePath);
    if (!file.exists()) {
        if (!file.open(QIODevice::WriteOnly))
            throw OpenWriteError(file.fileName());
        QDataStream out(&file);
        saveMagicNumber(magicNumber, out);
        file.close();
    }
    if (!file.open(QIODevice::ReadOnly))
        throw OpenReadError(file.fileName());
    QDataStream in(&file);
    quint32 magic;
    in >> magic;
    if (magic != magicNumber)
        throw EpCore::WrongFileTypeError(file.fileName());
    quint32 versionMajor, versionMinor;
    in >> versionMajor >> versionMinor;
    if (!VersionInfo::versionOk(versionMajor, versionMinor))
        throw WrongEpSimVersionError(file.fileName());
    quint16 streamVersion;
    in >> streamVersion;
    if (streamVersion > in.version())
        throw WrongQtVersionError(file.fileName());
    if (!in.atEnd())
        in >> data;
    if (file.error() != QFile::NoError)
        throw ReadError(file.fileName());
    file.close();
}

template<typename T>
void saveData(const QString& filePath, unsigned int magicNumber, const T& data) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        throw OpenWriteError(file.fileName());
    QDataStream out(&file);
    saveMagicNumber(magicNumber, out);
    out << data;
    if (file.error() != QFile::NoError)
        throw WriteError(file.fileName());
    file.close();
}

// System data is always saved to both the local System dir and
// the Network study path, if network storage is enabled.
template<typename T>
void saveSystemData(unsigned int magicNumber, const QString& fileName,
    const T& data, Options* options) {
    saveData(options->systemCatalogPath + "/" + fileName, magicNumber, data);
    if (options->filePathFlags.testFlag(Options::EnableNetworkStorage))
        saveData(options->networkStudyPath + "/" + fileName, magicNumber, data);
} 

// System data is read from the Network study path, if network storage
// is enabled, otherwise the System dir is used.
template<typename T>
void loadSystemData(unsigned int magicNumber, const QString& fileName, T& data, Options* options) {
    if (options->filePathFlags.testFlag(Options::EnableNetworkStorage))
        loadData(options->networkStudyPath + "/" + fileName, magicNumber, data);
    else
        loadData(options->systemCatalogPath + "/" + fileName, magicNumber, data);
}

} // namespace EpCore

#endif

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

#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>

class QDir;
class QString;

namespace EpCore {

enum CopyFlag { DoNotOverwrite  = 0x000000,
                Overwrite       = 0x000001
};

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
bool systemFileExists(const QString& fileName);

// gets a MagicNumber from a file
unsigned int magicNumber(const QString& filePath);
// check that file version is readable
bool versionOk(int majorVersion, int MinorVersion);

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
QString activeSystemPath();

void copyFilesToPath(const QStringList& files, const QString& sourcePath,
                     const QString& destPath,
                     CopyFlag = DoNotOverwrite);
void copyFilesToSystem(const QStringList& files, const QString& sourcePath,
                       CopyFlag = DoNotOverwrite);

QString joinPaths(const QString&, const QString&);
QString capitalize(const QString&);

bool isRemovableMedia(const QDir& dir);     // attempts to determine if
                                            // directory is removable media
bool useDiskCache(const QString& path);
QString osDependentSystemPath();
QString getUserName();
QString getMachineName();
long diskFreeSpace(const QString& path);

template<typename T>
class DataStream {
public:
    virtual void save(unsigned int magicNumber,
                      const QString& fileName,
                      const T& data) = 0;
    virtual void load(unsigned int magicNumber,
                      const QString& filNname,
                      T& data) = 0;
};


template<typename T>
class SystemStream : public DataStream<T> {
public:
    SystemStream(const QString& systemPath);

    virtual void save(unsigned int magicNumber,
                      const QString& fileName,
                      const T& data);
    virtual void load(unsigned int magicNumber,
                      const QString& filNname,
                      T& data);
private:
    QString systemPath_;
};

template<typename T>
class NetworkStream : public SystemStream<T> {
public:
    NetworkStream(const QString& networkPath,
                  const QString& systemPath);
    virtual void save(unsigned int magicNumber,
                      const QString& fileName,
                      const T& data);
    virtual void load(unsigned int magicNumber,
                      const QString& fileNname,
                      T& data);
private:
    QString networkPath_;
};


void testCdTools(QObject* = 0);

// definitions

template<typename T>
SystemStream<T>::SystemStream(const QString &systemPath)
    : systemPath_(systemPath) {}

template<typename T>
void SystemStream<T>::save(unsigned int magicNumber,
                       const QString& fileName,
                       const T& data) {
    saveData(joinPaths(systemPath_, fileName), magicNumber, data);
}

template<typename T>
void SystemStream<T>::load(unsigned int magicNumber,
                        const QString& fileName,
                        T& data) {
    loadData(joinPaths(systemPath_, fileName), magicNumber, data);
}

template<typename T>
NetworkStream<T>::NetworkStream(const QString& networkPath,
                             const QString& systemPath)
    : SystemStream<T>(systemPath), networkPath_(networkPath) {}

template<typename T>
void NetworkStream<T>::save(unsigned int magicNumber,
                         const QString& fileName,
                         const T& data) {
    SystemStream<T>::save(magicNumber, fileName, data);
    saveData(joinPaths(networkPath_, fileName), magicNumber, data);
}

template<typename T>
void NetworkStream<T>::load(unsigned int magicNumber,
                            const QString& fileName,
                            T& data) {
    SystemStream<T>::load(magicNumber, fileName, data);
    loadData(joinPaths(networkPath_, fileName), magicNumber, data);
}



inline bool versionOk(int /* versionMajor */,
               int /* versionMinor */) {
    // check for incompatible file versions here
    // at present no such creature
    return true;
}

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
    if (!versionOk(versionMajor, versionMinor))
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
    saveData(joinPaths(options->systemCatalogPath, fileName), magicNumber, data);
    if (options->filePathFlags.testFlag(Options::EnableNetworkStorage))
        saveData(joinPaths(options->networkStudyPath, fileName), magicNumber, data);
} 

// System data is read from the Network study path, if network storage
// is enabled, otherwise the System dir is used.
template<typename T>
void loadSystemData(unsigned int magicNumber, const QString& fileName, T& data, Options* options) {
    if (options->filePathFlags.testFlag(Options::EnableNetworkStorage))
        loadData(joinPaths(options->networkStudyPath, fileName), magicNumber, data);
    else
        loadData(joinPaths(options->systemCatalogPath, fileName), magicNumber, data);
}

inline QString capitalize(const QString& s) {
    QString capitalized = s;
    capitalized[0] = capitalized[0].toUpper();
    return capitalized;
}

} // namespace EpCore

#endif

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

#include "fileutilities.h"

#include <QDir>
#include <QFileInfo>

#include <QtDebug>

/**
 * @namespace EpCore Program functions that only require QtCore, not QtGui.
 */

/**
 * Writes unique integer and version numbers to datastream, to insure only
 * that files are truly epsimulator files.
 * @param magicNumber Unique int for each file type.
 * @param out QDataStream written to.
 */
void EpCore::saveMagicNumber(unsigned int magicNumber, QDataStream& out) {
    out << static_cast<quint32>(magicNumber);
    const VersionInfo* v = VersionInfo::instance();
    out << static_cast<quint32>(v->versionMajor())
        << static_cast<quint32>(v->versionMinor())
        << static_cast<quint16>(out.version());
}

bool EpCore::systemFileExists(const QString &fileName) {
    return QFile(joinPaths(activeSystemPath(), fileName)).exists();
}

unsigned int EpCore::magicNumber(const QString& filePath) {
    QFile file(filePath);
    if (!file.exists())
        throw FileNotFoundError(file.fileName());
    if (!file.open(QIODevice::ReadOnly))
        throw OpenReadError(file.fileName());
    QDataStream in(&file);
    quint32 magic;
    in >> magic;
    // go further and make sure file version is ok
    quint32 versionMajor, versionMinor;
    in >> versionMajor >> versionMinor;
    if (!VersionInfo::versionOk(versionMajor, versionMinor))
        throw WrongEpSimVersionError(file.fileName());
    quint16 streamVersion;
    in >> streamVersion;
    if (streamVersion > in.version())
        throw WrongQtVersionError(file.fileName());
    if (file.error() != QFile::NoError)
        throw ReadError(file.fileName());
    file.close();
    return magic;
}

/**
 * Deletes the directory, and all subdirs and files.
 * @param path Path to dir to be deleted.
 * Note path MUST be a directory, not a file.
 */
void EpCore::deleteDir(const QString& path) {
    QDir d(path);
    if (!d.exists())
        throw FileNotFoundError(path);
    // infinite recursion if filters not set
    QFileInfoList list = d.entryInfoList(QDir::AllEntries 
        | QDir::NoDotAndDotDot);
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir())  {
            deleteDir(fileInfo.filePath());   // recursive
            d.rmdir(fileInfo.path());
        } 
        else
            d.remove(fileInfo.filePath());
    }
    d.rmdir(path);
}

/**
 * Copies sourcePath to destinationPath, including all subdirs.  The directory
 * defined by sourcePath is copied to the directory destinationPath, 
 * note that sourcePath and destinationPath must be directories, NOT files.  Example:
 *      sourcePath = /home/user/MyStudies/studies/studyXXXXXXX
 *      destinationPath = /home/user/tmp/
 * After copyDir() there will be a dir:
 *      /home/user/tmp/studyXXXXXXX
 * If any files already exist in the destinationPath of the same name, they will
 * not be overwritten, and there will be no error.  To ensure this works, first
 * deleteDir(destinationPath + "/studyXXXXXXX").
 * @param sourcePath path to directory that you wish to copy in toto.
 * @param destinationPath the destination directory, where the copied directory
 * will end up.
 */
void EpCore::copyDir(const QString& sourcePath, 
             const QString& destinationPath) {
    QDir source(sourcePath);
    if (!source.exists())
        throw FileNotFoundError(sourcePath);
    QDir destination(destinationPath);
    if (!destination.exists())
        throw FileNotFoundError(destinationPath); 
    destination.mkdir(source.dirName());   
    destination.cd(source.dirName());
    // infinite recursion if filters not set
    QFileInfoList list = source.entryInfoList(QDir::AllEntries 
        | QDir::NoDotAndDotDot);
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir()) {
            copyDir(fileInfo.filePath(), destination.path());
        }
        else
            QFile::copy(fileInfo.filePath(), joinPaths(destination.path(),
                fileInfo.fileName()));
    }
}

QDir EpCore::directoryOf(const QString& subdir) {
    QDir dir(QCoreApplication::applicationDirPath());

#if defined(Q_OS_WIN)
    if (dir.dirName().toLower() == "debug"
        || dir.dirName().toLower() == "release")
        dir.cdUp();
#elif defined(Q_OS_MAC)
    if (dir.dirName() == "MacOS") {
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
     }
#endif
    if (dir.dirName().toLower() == "bin")
        dir.cdUp();
    dir.cd(subdir);
    return dir;
}

QDir EpCore::rootDirectory() {
    return directoryOf(".");
}

QDir EpCore::systemDirectory() {
    return directoryOf("System");
}

// returns the main System path, depending on whether networking is used
QString EpCore::activeSystemPath() {
    if (epOptions->EnableNetworkStorage)
        return epOptions->networkStudyPath;
    else
        return epOptions->systemCatalogPath;
}

// joins together 2 elements of a path, e.g. '/home' and '/src/'
// duplicate, missing '/'s are fixed
QString EpCore::joinPaths(const QString & p1, const QString & p2) {
    return QDir::cleanPath(p1 + "/" + p2);
}

void EpCore::copyFilesToPath(const QStringList& files,
                             const QString& sourcePath,
                             const QString& destPath,
                             EpCore::CopyFlag copyFlag) {
    if (!QDir(sourcePath).exists())
        throw EpCore::IoError(sourcePath, "Directory not found.");
    if (!QDir(destPath).exists())
        throw EpCore::IoError(destPath, "Directory not found.");
    if (QDir(sourcePath) == QDir(destPath))
        throw EpCore::SourceDestinationSameError(sourcePath);
    if (files.isEmpty())
        throw EpCore::FileNotFoundError("", "File list is empty.");
    QStringListIterator iter(files);
    QString fileName;
    QString sourceFilePath;
    QString destFilePath;
    while (iter.hasNext()) {
        fileName = iter.next();
        sourceFilePath = joinPaths(sourcePath, fileName);
        destFilePath = joinPaths(destPath, fileName);
        if (!QFile(sourceFilePath).exists())
            throw FileNotFoundError(fileName);
        if (copyFlag == Overwrite && QFile(destFilePath).exists())
            QFile(destFilePath).remove();
        // below will not overwrite files therefore above
        QFile::copy(sourceFilePath, destFilePath);
        qDebug() << "Copying " << fileName << " from "
                << sourcePath << " to " << destPath;
    }
}

void EpCore::copyFilesToSystem(const QStringList& files,
                               const QString& sourcePath,
                               EpCore::CopyFlag copyFlag) {
    // always copy to System Directory
    copyFilesToPath(files, sourcePath, epOptions->systemCatalogPath,
                    copyFlag);
    if (epOptions->filePathFlags.testFlag(Options::EnableNetworkStorage))
        copyFilesToPath(files, sourcePath, epOptions->networkStudyPath,
                        copyFlag);
}

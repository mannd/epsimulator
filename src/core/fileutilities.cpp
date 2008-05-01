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
    VersionInfo* v = VersionInfo::instance();
    out << static_cast<quint32>(v->versionMajor())
        << static_cast<quint32>(v->versionMinor());
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
            QFile::copy(fileInfo.filePath(), destination.path() + "/"
                + fileInfo.fileName());
    }
}

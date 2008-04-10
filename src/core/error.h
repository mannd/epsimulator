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

#ifndef ERROR_H
#define ERROR_H

#include <QString>

#include <stdexcept>

namespace EpCore {

/**
 * Base class for IO errors.
 */
class IoError : public std::runtime_error {
public:
    IoError(const QString& fileName = "", 
                     const char* msg = "general IOError")
                     : std::runtime_error(msg), fileName_(fileName) {}
    virtual ~IoError() throw() {}
    
    virtual const char* what() const throw() {return std::runtime_error::what();} 
    virtual QString fileName() const {return fileName_;}
private:
    QString fileName_;
};

class OpenReadError : public IoError {
public:
    OpenReadError(const QString& fileName,
                  const char* msg = "open read error")
                  : IoError(fileName, msg) {}
};

class OpenWriteError : public IoError {
public:
    OpenWriteError(const QString& fileName,
                  const char* msg = "open write error")
                  : IoError(fileName, msg) {}
};

class ReadError : public IoError {
public:
    ReadError(const QString& fileName,
                  const char* msg = "read error")
                  : IoError(fileName, msg) {}
};

class WriteError : public IoError {
public:
    WriteError(const QString& fileName,
                  const char* msg = "write error")
                  : IoError(fileName, msg) {}
};

/**
 * Used if magic number doesn't match.
 */
class WrongFileTypeError : public IoError {
public:
    WrongFileTypeError(const QString& fileName,
                  const char* msg = "wrong file type")
                  : IoError(fileName, msg) {}
};


/**
 * Incompatible file from earlier version of epsimulator.
 */
class WrongEpSimVersionError : public IoError {
public:
    WrongEpSimVersionError(const char* msg = "wrong EP Simulator version")
                           : IoError("", msg) {}
};

class DeleteError : public IoError {
public:
    DeleteError(const char* msg = "could not delete files or directories")
                : IoError("", msg) {}
};

class CopyError : public IoError {
public:
    CopyError(const char* msg = "could not copy files or directories")
              : IoError("", msg) {}
};

class FileNotFoundError : public IoError {
public:
    FileNotFoundError(const QString& fileName,
                      const char* msg = "file not found")
                      : IoError(fileName, msg) {}
};

/**
 * Used if source and destination for copying are the same
 */
class SourceDestinationSameError : public IoError {
public:
    SourceDestinationSameError(const QString& pathName,
        const char* msg = "source and destination are identical") 
        : IoError(pathName, msg) {}
};

}

#endif


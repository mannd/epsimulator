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

/**
  * @file catalog.h
  * Contains all catalog related classes, including 
  * Catalog (base class), SystemCatalog,
  * OpticalCatalog, NetworkCatalog, and the group of catalogs: Catalogs.
*/

#ifndef CATALOG_H
#define CATALOG_H

#include "study.h"

#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>

class Options;

/**
 * @author David Mann <mannd@epstudiossoftware.com>
 * Base class for NetworkCatalog, OpticalCatalog, and SystemCatalog.
 */
class Catalog {

public:
//  Might want to use the enum below instead of in epsimdefs.h?
    enum Source {Network, System, Optical, Other};
    Catalog(const QString& path, const QString& fileName);

    virtual void refresh() {}
    virtual void regenerate() {}

    virtual void addStudy(Study&) {};
    virtual void deleteStudy(Study&) {};

    virtual Source type() const {return Other;}
    virtual QString path() const {return path_;}
    virtual QString filePath() const;  // full path including fileName
    virtual QString fileName() const {return fileName_;}

    virtual void setPath(const QString& path) {path_ = path;}

    virtual ~Catalog() {}

protected:
    // don't allow copying or default constructor
    Catalog() {}
    Catalog(const Catalog&) {}

private:
    QString path_;  // path to catalog file, excluding file name
    QString fileName_; // file name of catalog

};

class OpticalCatalog : public Catalog {
public:
    OpticalCatalog(const QString& path, const QString& fileName);

    virtual Source type() const {return Optical;}
    virtual ~OpticalCatalog() {}

private:

};

class SystemCatalog : public Catalog {
public:
    SystemCatalog(const QString& path, const QString& fileName);
    
    virtual Source type() const {return System;}
    virtual ~SystemCatalog() {}
private:
};

class NetworkCatalog : public Catalog {
public:
    NetworkCatalog(const QString& path, const QString& fileName);

    virtual Source type() const {return Network;}
    virtual ~NetworkCatalog() {}

private:

};

// N.B. Catalogs owns the catalog pointers and will delete them.
class Catalogs {
public:
    Catalogs(Options* options);
    
    void refresh();
    void regenerate();
    Catalog* currentCatalog() const {return currentCatalog_;}
    QStringList filePaths() const {return filePaths_;}

    void setCurrentCatalog(Catalog::Source catalog);

    ~Catalogs();

protected:
    Catalogs();
    Catalogs(Catalogs&);

private:
    void appendFilePath(const QString& path);

    Options* options_;
    Catalog* currentCatalog_;
    OpticalCatalog* opticalCatalog_;
    SystemCatalog* systemCatalog_;
    NetworkCatalog* networkCatalog_;
    Catalog* otherCatalog_;
    QStringList filePaths_;
};


#endif

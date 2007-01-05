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
    \file catalog.h
    Contains all catalog related classes, including Catalog (base class), SystemCatalog,
    OpticalCatalog, NetworkCatalog, and the group of catalogs: Catalogs
*/

#ifndef CATALOG_H
#define CATALOG_H

#include "study.h"

#include <qstring.h>
#include <qfile.h>

/**
	@author David Mann <mannd@epstudiossoftware.com>
        Base class for NetworkCatalog, OpticalCatalog, and SystemCatalog.
*/
class Catalog {
public:
    enum CatalogSource {Network, System, Optical, Other};
    Catalog();
    

    virtual void refresh();
    virtual void regenerate();

    void addStudy(Study&) {};
    void deleteStudy(Study&) {};

    QString path() const {return path_;}

    void setPath(const QString& path) {path_ = path;}

    virtual ~Catalog() {}


private:
    QString path_;  // complete path, including file name

};

class OpticalCatalog : public Catalog {
public:
    OpticalCatalog();
    virtual ~OpticalCatalog() {}

private:

};

class SystemCatalog : public Catalog {
public:
    SystemCatalog();
    virtual ~SystemCatalog() {}
private:
};

class NetworkCatalog : public Catalog {
public:
    NetworkCatalog();
    virtual ~NetworkCatalog() {}

private:

};

/// TODO should this be a singleton class, and don't allow instantiation of 
/// any particular Catalog??
class Catalogs {
public:
    Catalogs();
    
    void refresh();
    void regenerate();
    Catalog* currentCatalog() {return currentCatalog_;}

    void setCurrentCatalog(Catalog* catalog) {currentCatalog_ = catalog;}

    ~Catalogs();
private:
    Catalog* currentCatalog_;
    Catalog* opticalCatalog_;
    Catalog* systemCatalog_;
    Catalog* networkCatalog_;
    Catalog* otherCatalog_;
};


#endif

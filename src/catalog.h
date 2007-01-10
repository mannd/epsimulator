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

#include "epsim.h"
#include "study.h"

#include <qstring.h>
#include <qfile.h>

using namespace epsim;  // for CatalogSource enum

/**
	@author David Mann <mannd@epstudiossoftware.com>
        Base class for NetworkCatalog, 
	OpticalCatalog, and SystemCatalog.
*/
class Catalog {

public:
//  Might want to use the enum below instead of in epsim.h?
//    enum CatalogSource {Network, System, Optical, Other};
    Catalog();
    

    virtual void refresh() {}
    virtual void regenerate() {}

    virtual void addStudy(Study&) {};
    virtual void deleteStudy(Study&) {};

    virtual CatalogSource type() const {return Other;}
    virtual QString path() const {return path_;}

    virtual void setPath(const QString& path) {path_ = path;}

    virtual ~Catalog() {}


private:
    QString path_;  // complete path, including file name

};

class OpticalCatalog : public Catalog {
public:
    OpticalCatalog();

    virtual CatalogSource type() const {return Optical;}
    virtual ~OpticalCatalog() {}

private:

};

class SystemCatalog : public Catalog {
public:
    SystemCatalog();
    
    virtual CatalogSource type() const {return System;}
    virtual ~SystemCatalog() {}
private:
};

class NetworkCatalog : public Catalog {
public:
    NetworkCatalog();

    virtual CatalogSource type() const {return Network;}
    virtual ~NetworkCatalog() {}

private:

};

/// TODO should this be a singleton class, and don't allow instantiation of 
/// any particular Catalog??
// N.B. Catalogs owns the catalog pointers and will delete them.
class Catalogs {
public:
    Catalogs(CatalogSource defaultCatalog);
    
    void refresh();
    void regenerate();
    Catalog* currentCatalog() {return currentCatalog_;}

    void setCurrentCatalog(CatalogSource catalog);

    ~Catalogs();
private:
    Catalog* currentCatalog_;
    OpticalCatalog* opticalCatalog_;
    SystemCatalog* systemCatalog_;
    NetworkCatalog* networkCatalog_;
    Catalog* otherCatalog_;
};


#endif

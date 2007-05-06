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

#include <qstring.h>

#include <map>

class Options;
class QDataStream;
class QFile;

/**
 * @author David Mann <mannd@epstudiossoftware.com>
 * Base class for NetworkCatalog, OpticalCatalog, and SystemCatalog.
 * Note that this is set up like an STL container, with its own
 * iterator, and an operator[].
 */
class Catalog {
public:
    enum Source {Network, System, Optical, Other};
    typedef std::map<QString, Study>::const_iterator Iterator;

    Catalog(const QString& path, const QString& fileName);

    Iterator begin() {return catalog_.begin();}
    Iterator end() {return catalog_.end();}
    Study& operator[](QString& key) {return catalog_[key];} 

    virtual void refresh();
    // be default, only optical catalog regenerates.
    /// TODO But, might want to update the other catalogs as well...
    virtual void regenerate() {}

    virtual void addStudy(Study*);
    virtual void deleteStudy(Study*);
    virtual void editStudy(Study*);

    virtual Source type() const {return Other;}
    virtual QString path() const {return path_;}
    virtual QString filePath() const;  // full path including fileName
    virtual QString fileName() const {return fileName_;}
    virtual QString name() const {return "Catalog";}

    virtual void setPath(const QString& path) {path_ = path;}

    virtual ~Catalog(); 

protected:
    // don't allow copying or default constructor
    Catalog() {}
    Catalog(const Catalog&) {}

    // First bytes of EP Simulator catalog files.
    enum {MagicNumber = 0x99c798f2};    

    void readFromStream(QDataStream& in);
    void writeToStream(QDataStream& out);

    virtual void load();
    virtual void save();


    std::map<QString, Study> catalog_;

private:    
    void loadFile(QFile& file);
    void saveFile(QFile& file);

    QString path_;  // path to catalog file, excluding file name
    QString fileName_; // file name of catalog

};

class OpticalCatalog : public Catalog {
public:
    OpticalCatalog(const QString& path, const QString& fileName);

    virtual QString name() const {return "OpticalCatalog";}
    virtual Source type() const {return Optical;}
    virtual void addStudy(Study*);
    virtual void regenerate();
    virtual ~OpticalCatalog() {}

private:

};

class SystemCatalog : public Catalog {
public:
    SystemCatalog(const QString& path, const QString& fileName);

    virtual QString name() const {return "SystemCatalog";}
    virtual Source type() const {return System;}
    virtual ~SystemCatalog() {}
private:
};

class NetworkCatalog : public Catalog {
public:
    NetworkCatalog(const QString& path, const QString& fileName);

    virtual QString name() const {return "NetworkCatalog";}
    virtual Source type() const {return Network;}
    virtual ~NetworkCatalog() {}

private:

};


// N.B. Catalogs owns the catalog pointers and will delete them.
class Catalogs {
public:
    Catalogs(Options* options);

    // Functions below work on all active catalogs.
    void addStudy(Study*);
    void deleteStudy(Study*);
    void replaceStudy(Study*);
    
    void refresh();
    void regenerate();
    Catalog* currentCatalog() const {return currentCatalog_;}

    void setCurrentCatalog(Catalog::Source catalog);
    void setCatalogPath(Catalog::Source catalog, const QString& path);
        

    ~Catalogs();

protected:
    Catalogs();
    Catalogs(Catalogs&);

private:

    Catalog* currentCatalog_;
    OpticalCatalog* opticalCatalog_;
    SystemCatalog* systemCatalog_;
    NetworkCatalog* networkCatalog_;
    Catalog* otherCatalog_;

    typedef std::map<Catalog::Source, Catalog*>::const_iterator Iterator;
    std::map<Catalog::Source, Catalog*> catalogs_;
};


#endif

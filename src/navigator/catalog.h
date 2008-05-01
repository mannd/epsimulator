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

#include <QMap>
#include <QString>

#include <map>
#include <vector>

class QDataStream;
class QFile;

namespace EpCore { class Options; }

namespace EpNavigator {

using EpCore::Options;
using EpPatient::Study;

struct StudyData {
    friend QDataStream& operator<<(QDataStream&, const StudyData&);
    friend QDataStream& operator>>(QDataStream&, StudyData&);
    Study study;
    QString location;      // location == disk label
    QString side;          // disk side, null for single sided disks
    QString labName;       // name of lab for Network catalog
    QString machineName;   // name of machine for Network catalog
};

/**
 * @author David Mann <mannd@epstudiossoftware.com>
 * Base class for NetworkCatalog, OpticalCatalog, and SystemCatalog.
 * Note that this is set up like an STL container, with its own
 * iterator, and an operator[].
 */
class Catalog {

public:
    enum Source {Network, System, Optical, Other};
    typedef QMap<QString, StudyData> CatalogMap;
    typedef std::vector<QString> Keys;

    Catalog(const QString& path, const QString& fileName = defaultFileName_);
    virtual ~Catalog(); 

    static const QString defaultFileName() {return defaultFileName_;}

    CatalogMap::const_iterator begin() {return catalog_.begin();}
    CatalogMap::const_iterator end() {return catalog_.end();}
    StudyData& operator[](const QString& key) {return catalog_[key];} 

    virtual void refresh();
    virtual void regenerate(Keys& keys, Catalog*); 
    virtual void relabel(const QString& label, const QString& side, 
        const QString& key);

 
    virtual void addStudy(const Study* study, const QString& location,
                        const QString& side, const QString& labName,
                        const QString& machineName);
    virtual void deleteStudy(const Study*);

    void setPath(const QString& path) {path_ = path;}
    void setLabel(const QString& label) {studyData_.location = label;}
    void setSide(const QString& side) {studyData_.side = side;}

    QString path() const {return path_;}
    QString filePath() const;  // full path including fileName
    QString fileName() const {return fileName_;}

    // generates appropriate location format
    // overriden by specific catalog types
    virtual QString location(const StudyData&); 
    bool isEmpty() {return catalog_.empty();}
    virtual bool studyPresent(const Study*);

protected:
    // don't allow copying or default constructor
    Catalog() {}
    Catalog(const Catalog&) {}

    // First bytes of EP Simulator catalog files.
    enum {MagicNumber = 0x99c798f2};    

    void load();
    void save();
 
    CatalogMap catalog_;
    static const QString defaultFileName_;

private:
    QString path_;          // path to catalog file, excluding file name
    QString fileName_;      // file name of catalog
    StudyData studyData_;   // study and location data

};

class OpticalCatalog : public Catalog {

public:
    OpticalCatalog(const QString& path, const QString& fileName = defaultFileName_);
    ~OpticalCatalog() {}

    virtual void addStudy(const Study* study, const QString& location,
                        const QString& side, const QString& labName,
                        const QString& machineName);
    void create(const QString& location, const QString& side,
                    const QString& labName, const QString& machineName);
    void regenerate(Keys&, Catalog*) {} 
    void relabel(const QString&, const QString&, 
        const QString&) {}  // this relabel does nothing
    // relabel below is nonvirtual, only in this class
    void relabel(const QString& label, const QString& side);
    Catalog::Keys getKeys();

};

class SystemCatalog : public Catalog {

public:
    SystemCatalog(const QString& path, const QString& fileName = defaultFileName_);
    ~SystemCatalog() {}

};

class NetworkCatalog : public Catalog {

public:
    NetworkCatalog(const QString& path, const QString& fileName = defaultFileName_);
    ~NetworkCatalog() {}

    virtual QString location(const StudyData&);

};

// OtherCatalog is a read-only catalog
class OtherCatalog : public Catalog {

public:
    OtherCatalog(const QString& path, const QString& fileName = defaultFileName_);
    ~OtherCatalog() {}

    virtual void addStudy(const Study*, const QString&,
                        const QString&, const QString&,
                        const QString&) {}  // don't add studies to Other catalog
    virtual void regenerate(Keys&, Catalog*) {} // don't regenerate Other catalog
    virtual void deleteStudy(const Study*) {}

};


// N.B. Catalogs owns the catalog pointers and will delete them.
class Catalogs {

public:
    Catalogs(Options* options, const QString& opticalDiskPath);
    ~Catalogs();

    // Functions below work on all active catalogs.
    void addStudy(const Study* study, 
                  const QString& location = QString(),
                  const QString& side = QString(), 
                  const QString& labName = QString(),
                  const QString& machineName = QString());
    void deleteStudy(const Study*);
    
    void refresh();
    void regenerate(const QString& location, const QString& side,
                    const QString& labName, const QString& machineName);
    void relabel(const QString& label, const QString& side);

    Catalog* currentCatalog() const {return currentCatalog_;}

    void setCurrentCatalog(Catalog::Source);
    void setCatalogPath(Catalog::Source, const QString& path);

    bool studyPresentOnOpticalDisk(const Study*) const;


protected:
    Catalogs();
    Catalogs(Catalogs&);

private:
    Catalog* currentCatalog_;
    OpticalCatalog* opticalCatalog_;
    SystemCatalog* systemCatalog_;
    NetworkCatalog* networkCatalog_;
    Catalog* otherCatalog_;

    typedef std::map<Catalog::Source, Catalog*> CatalogsMap;
    CatalogsMap catalogs_;

};

}

#endif

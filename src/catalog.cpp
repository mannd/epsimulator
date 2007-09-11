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

#include "catalog.h"
#include "epfuns.h"
#include "error.h"
#include "options.h"
#include "study.h"

#include <qdatastream.h>
#include <qdir.h>
#include <qfile.h>
#include <qstringlist.h>

#include <vector>

#ifndef NDEBUG
#include <iostream>
#endif

/**
   \file catalog.cpp

   \brief Contains catalog classes and Catalogs class.
*/

QDataStream& operator<<(QDataStream& out, const StudyData& studyData) {
    out << studyData.study << studyData.location << studyData.side 
        << studyData.labName << studyData.machineName;
    return out;
}

QDataStream& operator>>(QDataStream& in, StudyData& studyData) {
    in >> studyData.study >> studyData.location >> studyData.side 
        >> studyData.labName >> studyData.machineName;
    return in;
}

/**
   \class Catalog

   \brief A catalog contains a group of studies.
*/

const QString Catalog::defaultFileName_ = "catalog.dat";

Catalog::Catalog(const QString& path, 
                 const QString& fileName) : path_(path), fileName_(fileName) {
        refresh();
}

Catalog::~Catalog() {
}

void Catalog::addStudy(const Study* study, const QString& location,
                       const QString& side, const QString& labName,
                       const QString& machineName) {
    catalog_[study->key()].study = *study;
    catalog_[study->key()].location = location;
    catalog_[study->key()].side = side;
    catalog_[study->key()].labName = labName;
    catalog_[study->key()].machineName = machineName;
    save();
}

void Catalog::deleteStudy(const Study* study) {
    catalog_.erase(study->key());
    save();
}

// void Catalog::editStudy(Study* study) {
//     addStudy(study);
// }

void Catalog::refresh() {
    load();
}

void Catalog::regenerate(Keys& keys, Catalog* c) {
    for (Keys::const_iterator p = keys.begin();
        p != keys.end(); ++p) {
        catalog_[*p] = (*c)[*p];
    }
    save();
}

void Catalog::relabel(const QString& label, const QString& side, const QString& key) {
    for (CatalogMap::iterator p = catalog_.begin();
        p != catalog_.end(); ++p) {
        // with optical catalog, all studies must be relabeled
        if (isOptical() || p.data().study.key() == key ) {
            p.data().location = label;
            p.data().side = side;
        }
    }
    save();
}

bool Catalog::studyPresent(const Study* s) {
    for (CatalogMap::iterator p = catalog_.begin();
        p != catalog_.end(); ++p) {
        if (p.data().study.key() == s->key())
            return true;
    }
    return false;
}

QString Catalog::location(const StudyData& sd) {
    if (sd.study.isPreregisterStudy())
        return QString::null;
    return sd.location + 
            (!sd.side.isEmpty() ? " - " + sd.side : QString::null);
}

void Catalog::load() {
    try {
        EpFuns::loadData(filePath(), MagicNumber, catalog_);
    }
    catch (EpSim::IoError&) {
        // ignore failure to read, leave catalog empty
    }
}

void Catalog::save() {
    EpFuns::saveData(filePath(), MagicNumber, catalog_);
}

QString Catalog::filePath() const {
    return QDir::cleanDirPath(path_ + "/" + fileName_);
}

OpticalCatalog::OpticalCatalog(const QString& path, 
                 const QString& fileName) : Catalog(path, fileName) {
}


void OpticalCatalog::addStudy(const Study* study, const QString& location,
                      const QString& side, const QString& labName,
                      const QString& machineName) {
    // only add real studies to the optical drive
    if (!study->isPreregisterStudy())
        Catalog::addStudy(study, location, side, labName, machineName);
}

void OpticalCatalog::regenerate(const QString& location, const QString& side,
                    const QString& labName, const QString& machineName) {
    QDir studiesDir(path() + "/studies");
    QStringList studyList = studiesDir.entryList("study_*");
    catalog_.clear();   // Start from scratch, as some studies might have
                        // been deleted on disk but not in the catalog.
    for (QStringList::Iterator it = studyList.begin(); 
            it != studyList.end(); ++it) {
        Study s;
        s.setPath(studiesDir.path() + "/" + *it);
        s.load();
        StudyData sd = {s, location, side, labName, machineName};
        catalog_[s.key()] = sd; 
    }
    save();
}

Catalog::Keys OpticalCatalog::getKeys() {
    Keys keys;
    for (CatalogMap::iterator p = catalog_.begin();
        p != catalog_.end(); ++p) 
        keys.push_back(p.data().study.key());
    return keys;
}

SystemCatalog::SystemCatalog(const QString& path, 
                 const QString& fileName) : Catalog(path, fileName) {
}

NetworkCatalog::NetworkCatalog(const QString& path, 
                 const QString& fileName) : Catalog(path, fileName){
}

/**
 * 
 * @param s Study 
 * @return location of study, including machine name or lab name
 */
QString NetworkCatalog::location(const StudyData& sd) {
    if (sd.study.isPreregisterStudy())
        return QString::null;
    if (Options::instance()->useLabName()) {
        if (!sd.labName.isEmpty())
            return sd.labName + " - " + Catalog::location(sd);
    }
    return sd.machineName + " - " + Catalog::location(sd);
}

// const char* Catalogs::fileName_ = "catalog.dat";

/**
 * Catalogs constructor.  Catalogs keeps all the catalogs, keeps track of
 * the current catalog, and initializes them all from their file paths.
 * @param options = Pointer to Epsim Options.
 * @param path = path to optical disk.
 */
Catalogs::Catalogs(Options* options, const QString& path) {
    systemCatalog_ = new SystemCatalog(options->systemCatalogPath());
    opticalCatalog_ = new OpticalCatalog(path);
    otherCatalog_ = new Catalog(options->systemCatalogPath());
    catalogs_[Catalog::System] = systemCatalog_;
    catalogs_[Catalog::Optical] = opticalCatalog_;
    catalogs_[Catalog::Other] = otherCatalog_;
    if (options->enableNetworkStorage()) {
        networkCatalog_ = new NetworkCatalog(options->networkStudyPath());
        catalogs_[Catalog::Network] = networkCatalog_;
        currentCatalog_ = networkCatalog_;
    }
    else {
        networkCatalog_ = 0;
        currentCatalog_ = systemCatalog_;
    }
}

void Catalogs::setCatalogPath(Catalog::Source source, const QString& path) {
    catalogs_[source]->setPath(path);
}

// QString Catalogs::fileName() const {
//     return fileName_;
// }

/// TODO need an update Catalogs function to be called after systemDialog execs

void Catalogs::setCurrentCatalog(Catalog::Source source) {
    currentCatalog_ = catalogs_[source];
}

void Catalogs::addStudy(const Study* study, const QString& location,
                        const QString& side, const QString& labName,
                        const QString& machineName) {
    for (CatalogsMap::iterator it = catalogs_.begin(); it != catalogs_.end(); ++it)
        (*it).second->addStudy(study, location, side, labName, machineName);
}

void Catalogs::deleteStudy(const Study* study) {
    for (CatalogsMap::iterator it = catalogs_.begin(); it != catalogs_.end(); ++it)
        (*it).second->deleteStudy(study);
}


void Catalogs::refresh() {
   for (CatalogsMap::iterator it = catalogs_.begin(); it != catalogs_.end(); ++it)
        (*it).second->refresh();
}

void Catalogs::regenerate(const QString& location, const QString& side,
                    const QString& labName, const QString& machineName) {
    opticalCatalog_->regenerate(location, side, labName, machineName);
    Catalog::Keys keys = opticalCatalog_->getKeys();
    for (CatalogsMap::const_iterator it = catalogs_.begin(); it != catalogs_.end(); ++it) 
        if (!(*it).second->isOptical())   // already did the optical catalog
            (*it).second->regenerate(keys, opticalCatalog_);
}

void Catalogs::relabel(const QString& label, const QString& side) {
    // relabel optical catalog first
    opticalCatalog_->relabel(label, side);
    std::vector<QString> keys = opticalCatalog_->getKeys();
     for (CatalogsMap::const_iterator it = catalogs_.begin(); it != catalogs_.end(); ++it) 
         if (!(*it).second->isOptical()) // already did the optical catalog
            for (std::vector<QString>::iterator p = keys.begin();
                p != keys.end(); ++p)
                (*it).second->relabel(label, side, *p);
}

bool Catalogs::studyPresentOnOpticalDisk(const Study* s) const {
    return opticalCatalog_->studyPresent(s);
}

Catalogs::~ Catalogs() {
    // don't delete currentCatalog_ it is just a copy of
    // one of the below!
    delete opticalCatalog_;
    delete systemCatalog_;
    delete networkCatalog_;
    delete otherCatalog_;
}


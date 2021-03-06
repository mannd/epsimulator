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
#include "fileutilities.h"
#include "error.h"
#include "opticaldisk.h"
#include "options.h"
#include "study.h"

#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QStringList>

#include <vector>

using EpCore::Options;
using EpHardware::EpOpticalDisk::OpticalDisk;
using EpNavigator::Catalog;
using EpNavigator::OpticalCatalog;
using EpNavigator::OtherCatalog;
using EpNavigator::SystemCatalog;
using EpNavigator::NetworkCatalog;
using EpNavigator::Catalogs;
using EpStudy::Study;

namespace EpNavigator {

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

}

// class Catalog

const QString Catalog::defaultFileName_ = "catalog.dat";

Catalog::Catalog(const QString& path, const QString& fileName) :
                 path_(path), fileName_(fileName) {
        load();
}

Catalog::~Catalog() {}

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
    catalog_.remove(study->key());
    save();
}

void Catalog::editStudy(const Study* study) {
    catalog_[study->key()].study = *study;
    save();
}

void Catalog::refresh() {
    catalog_.clear();
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
        if (p.value().study.key() == key ) {
            p.value().location = label;
            p.value().side = side;
        }
    }
    save();
}

bool Catalog::studyPresent(const Study* s) {
    for (CatalogMap::iterator p = catalog_.begin();
        p != catalog_.end(); ++p) {
        if (p.value().study.key() == s->key())
            return true;
    }
    return false;
}

QString Catalog::location(const StudyData& sd) {
    if (sd.study.isPreregisterStudy())
        return QString();
    return sd.location + 
            (!sd.side.isEmpty() ? " - " + sd.side : QString());
}

void Catalog::load() {
    try {
     EpCore::loadData(filePath(), MagicNumber, catalog_);
    }
    catch (EpCore::IoError&) {
        // ignore failure to read, leave catalog empty
    }
}

void Catalog::save() {
    EpCore::saveData(filePath(), MagicNumber, catalog_);
}

QString Catalog::filePath() const {
    return EpCore::joinPaths(path_, fileName_);
}

// class OpticalCatalog

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

/**
 * This is an overloaded nonvirtual version of relabel, eliminating the key
 * parameter in the virtual relabel.  All studies in an optical catalog
 * are relabeled with the new label.  System catalogs are a mix of different
 * studies from different optical drives, so they are relabeled for a specific
 * key.  See Catalogs::relabel() for more.
 * @param label new label for the disk.
 * @param side new side for the disk.
 */
void OpticalCatalog::relabel(const QString& label, const QString& side) {
    for (CatalogMap::iterator p = catalog_.begin();
        p != catalog_.end(); ++p) {
        // with optical catalog, all studies must be relabeled
        p.value().location = label;
        p.value().side = side;
    }
    save();
}

void OpticalCatalog::create(const QString& location, const QString& side,
                    const QString& labName, const QString& machineName) {
    QDir studiesDir(OpticalDisk::makeStudiesPath(path()));
    QStringList studyList = studiesDir.entryList(QStringList() << "study_*");
    catalog_.clear();   // Start from scratch, as some studies might have
                        // been deleted on disk but not in the catalog.
    for (QStringList::Iterator it = studyList.begin(); 
            it != studyList.end(); ++it) {
        Study s;
        s.setPath(EpCore::joinPaths(studiesDir.path(), *it));
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
        keys.push_back(p.value().study.key());
    return keys;
}

// class SystemCatalog

SystemCatalog::SystemCatalog(const QString& path, 
                 const QString& fileName) : Catalog(path, fileName) {
}

// class NetworkCatalog

NetworkCatalog::NetworkCatalog(bool useLabName, const QString& path, 
                 const QString& fileName) 
    : Catalog(path, fileName), useLabName_(useLabName) {}

/**
 * 
 * @param sd StudyData 
 * @return location of study, including machine name or lab name
 */
QString NetworkCatalog::location(const StudyData& sd) {
    if (sd.study.isPreregisterStudy())
        return QString();
    if (useLabName_) {
        if (!sd.labName.isEmpty())
            return sd.labName + " - " + Catalog::location(sd);
    }
    return sd.machineName + " - " + Catalog::location(sd);
}

// class OtherCatalog

OtherCatalog::OtherCatalog(const QString& path, const QString& fileName)
    : Catalog(path, fileName) {
}

// class Catalogs

/**
 * Catalogs constructor.  Catalogs keeps all the catalogs, keeps track of
 * the current catalog, and initializes them all from their file paths.
 * @param options = Pointer to Epsim Options.
 * @param path = path to optical disk.
 */
Catalogs::Catalogs(Options* options, const QString& path) {
    systemCatalog_ = new SystemCatalog(options->systemCatalogPath);
    opticalCatalog_ = new OpticalCatalog(path);
    otherCatalog_ = new OtherCatalog(options->systemCatalogPath);
    catalogs_[Catalog::System] = systemCatalog_;
    catalogs_[Catalog::Optical] = opticalCatalog_;
    catalogs_[Catalog::Other] = otherCatalog_;
    if (options->filePathFlags.testFlag(Options::EnableNetworkStorage)) {
        networkCatalog_ = new NetworkCatalog(options->useLabName,
                                             options->networkStudyPath);
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

void Catalogs::setCurrentCatalog(Catalog::Source source) {
    currentCatalog_ = catalogs_[source];
}

void Catalogs::addStudy(const Study* study, const QString& location,
                        const QString& side, const QString& labName,
                        const QString& machineName) {
    for (CatalogsMap::iterator it = catalogs_.begin(); 
        it != catalogs_.end(); ++it)
        (*it).second->addStudy(study, location, side, labName, machineName);
}

void Catalogs::deleteStudy(const Study* study) {
    for (CatalogsMap::iterator it = catalogs_.begin(); 
        it != catalogs_.end(); ++it)
        (*it).second->deleteStudy(study);
}

void Catalogs::editStudy(const Study* study) {
    for (CatalogsMap::iterator it = catalogs_.begin();
        it != catalogs_.end(); ++it)
        (*it).second->editStudy(study);
}

void Catalogs::refresh() {
   for (CatalogsMap::iterator it = catalogs_.begin(); 
        it != catalogs_.end(); ++it)
        (*it).second->refresh();
}

void Catalogs::regenerate(const QString& location, const QString& side,
                    const QString& labName, const QString& machineName) {
    opticalCatalog_->create(location, side, labName, machineName);
    Catalog::Keys keys = opticalCatalog_->getKeys();
    for (CatalogsMap::const_iterator it = catalogs_.begin(); 
        it != catalogs_.end(); ++it) 
        (*it).second->regenerate(keys, opticalCatalog_);
}

/**
 * Relabels the current optical disk catalog, then updates the system 
 * catalogs.  Uses the special overloaded nonvirtual relabel of 
 * OpticalCatalog, then extracts the keys from the OpticalCatalog
 * and goes through the current system catalogs, relabeling if
 * the keys match.
 * @param label new label for the disk.
 * @param side new side for the disk.
 */
void Catalogs::relabel(const QString& label, const QString& side) {
    // relabel optical catalog first
    opticalCatalog_->relabel(label, side);
    std::vector<QString> keys = opticalCatalog_->getKeys();
     for (CatalogsMap::const_iterator it = catalogs_.begin(); 
        it != catalogs_.end(); ++it) 
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

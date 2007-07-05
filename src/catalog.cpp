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
#include "error.h"
#include "opticaldisk.h"
#include "options.h"
#include "study.h"

#include <qdatastream.h>
#include <qdir.h>
#include <qfile.h>

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

void Catalog::relabel(const QString& label, const QString& side, const QString& key) {
    for (CatalogMap::iterator p = catalog_.begin();
        p != catalog_.end(); ++p) {
        // with optical catalog, all studies must be relabeled
        if (isOptical() || p->second.study.key() == key ) {
            p->second.location = label;
            p->second.side = side;
        }
    }
    save();
}

bool Catalog::studyPresent(const Study* s) {
    for (CatalogMap::iterator p = catalog_.begin();
        p != catalog_.end(); ++p) {
        if (p->second.study.key() == s->key())
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
        QFile f(filePath());
        loadFile(f);
    }
    catch (EpSim::IoError&) {
        // ignore failure to read, leave catalog empty
    }
}

void Catalog::save() {
    QFile f(filePath());
    saveFile(f);
}

void Catalog::loadFile(QFile& file) {
    // create a studies file if it doesn't exist already
    if (!file.exists()) 
        saveFile(file);
    if (!file.open(IO_ReadOnly)) 
        throw EpSim::OpenReadError(file.name());
    QDataStream in(&file);
    in.setVersion(5);
    Q_UINT32 magic;
    in >> magic;
    if (magic != MagicNumber) 
        throw EpSim::WrongFileTypeError(file.name());
    readFromStream(in);
    if (file.status() != IO_Ok) 
        throw EpSim::ReadError(file.name());
    file.close();
}

void Catalog::saveFile(QFile& file) {
    if (!file.open(IO_WriteOnly)) 
        throw EpSim::OpenWriteError(file.name());
    QDataStream out(&file);
    out.setVersion(5);
    out << (Q_UINT32)MagicNumber;
    writeToStream(out);
    if (file.status() != IO_Ok) 
        throw EpSim::WriteError(file.name());
    // must close the file so it can be reopened IO_ReadOnly
    file.close();
}




void Catalog::readFromStream(QDataStream& in) {
    catalog_.clear();
    while (!in.atEnd()) {
        StudyData studyData;
        in >> studyData;
        catalog_[studyData.study.key()] = studyData;
    }
}

void Catalog::writeToStream(QDataStream& out) {
    for (Iterator p = catalog_.begin(); p != catalog_.end(); ++p)
        out << p->second;
}

QString Catalog::filePath() const {
    return QDir::cleanDirPath(path_ + "/" + fileName_);
}

/// FIXME The Optical catalog doesn't necessarily use the straight options path,
/// since an emulated optical disk has a different path.  This needs to be
/// addressed in this ctor.  The path will depend on opticalDisk_ in Navigator.
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

void OpticalCatalog::regenerate() {
    // open studies directory of the "opticaldisk"
    // read each study into temp catalog
    // if no errors replace catalog
    // save catalog.dat file
    // else display error, or throw error
}

std::vector<QString> OpticalCatalog::getKeys() {
    std::vector<QString> keys;
    for (CatalogMap::iterator p = catalog_.begin();
        p != catalog_.end(); ++p) 
        keys.push_back(p->second.study.key());
    return keys;
}


// void OpticalCatalog::relabel(const QString& oldLabel, const QString& newLabel) {
//     // all labels are set to new label in the optical catalog
//     (void) oldLabel;    // get rid of unused variable warning
//     for (CatalogMap::iterator p = catalog_.begin(); 
//     p != catalog_.end(); ++p)
//         p->second.location = newLabel;
//     save();
// }

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

const char* Catalogs::fileName_ = "catalog.dat";

/**
 * Catalogs constructor.  Catalogs keeps all the catalogs, keeps track of
 * the current catalog, and initializes them all from their file paths.
 * @param options = Pointer to Epsim Options.
 * @param path = path to optical disk.
 */
Catalogs::Catalogs(Options* options, const QString& path) {
    networkCatalog_ = new NetworkCatalog(options->networkStudyPath(), fileName_);
    systemCatalog_ = new SystemCatalog(options->systemCatalogPath(), fileName_);
    opticalCatalog_ = new OpticalCatalog(path, fileName_);
    otherCatalog_ = new Catalog(options->systemCatalogPath(), fileName_);
    catalogs_[Catalog::Network] = networkCatalog_;
    catalogs_[Catalog::System] = systemCatalog_;
    catalogs_[Catalog::Optical] = opticalCatalog_;
    catalogs_[Catalog::Other] = otherCatalog_;
    if (options->enableNetworkStorage())
        currentCatalog_ = networkCatalog_;
    else
        currentCatalog_ = systemCatalog_;
}

void Catalogs::setCatalogPath(Catalog::Source source, const QString& path) {
    catalogs_[source]->setPath(path);
}

QString Catalogs::fileName() const {
    return fileName_;
}

/// TODO need an update Catalogs function to be called after systemDialog execs

void Catalogs::setCurrentCatalog(Catalog::Source source) {
    currentCatalog_ = catalogs_[source];
}

void Catalogs::addStudy(const Study* study, const QString& location,
                        const QString& side, const QString& labName,
                        const QString& machineName) {
    for (Iterator it = catalogs_.begin(); it != catalogs_.end(); ++it)
        (*it).second->addStudy(study, location, side, labName, machineName);
}

void Catalogs::addStudy(const Study* study) {
    addStudy(study, QString::null, QString::null, QString::null, QString::null);
}

void Catalogs::deleteStudy(const Study* study) {
    for (Iterator it = catalogs_.begin(); it != catalogs_.end(); ++it)
        (*it).second->deleteStudy(study);
}


void Catalogs::refresh() {
   for (Iterator it = catalogs_.begin(); it != catalogs_.end(); ++it)
        (*it).second->refresh();
}

void Catalogs::regenerate() {
  for (Iterator it = catalogs_.begin(); it != catalogs_.end(); ++it)
        (*it).second->regenerate();
    
}

void Catalogs::relabel(const QString& label, const QString& side) {
    // relabel optical catalog first
    opticalCatalog_->relabel(label, side);
    std::vector<QString> keys = opticalCatalog_->getKeys();
     for (Iterator it = catalogs_.begin(); it != catalogs_.end(); ++it) 
         if (!(*it).second->isOptical()) 
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


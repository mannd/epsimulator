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

 void Catalog::addStudy(Study* study) {
    catalog_[study->key()] = *study;
    save();
}

void Catalog::deleteStudy(Study* study) {
    catalog_.erase(study->key());
    save();
}

void Catalog::editStudy(Study* study) {
    addStudy(study);
}

void Catalog::refresh() {
    load();
}

void Catalog::relabel(const QString& oldLabel, const QString& newLabel) {
    for (std::map<QString, Study>::iterator p = catalog_.begin(); 
        p != catalog_.end(); ++p) {
        if (p->second.location() == oldLabel)
            p->second.setLocation(newLabel);
    }
    save();
}

bool Catalog::studyPresent(Study* s) {
    for (std::map<QString, Study>::iterator p = catalog_.begin();
        p != catalog_.end(); ++p) {
        if (p->second.key() == s->key())
            return true;
    }
    return false;
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
        Study study;
        in >> study;
        catalog_[study.key()] = study;
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

void OpticalCatalog::addStudy(Study* study) {
    // only add real studies to the optical drive
    if (!study->isPreregisterStudy())
        Catalog::addStudy(study);
}

void OpticalCatalog::regenerate() {
    // open studies directory of the "opticaldisk"
    // read each study into temp catalog
    // if no errors replace catalog
    // save catalog.dat file
    // else display error, or throw error
}


void OpticalCatalog::relabel(const QString& oldLabel, const QString& newLabel) {
    // all labels are set to new label in the optical catalog
    (void) oldLabel;    // get rid of unused variable warning
    for (std::map<QString, Study>::iterator p = catalog_.begin(); 
    p != catalog_.end(); ++p)
        p->second.setLocation(newLabel);
    save();
}

SystemCatalog::SystemCatalog(const QString& path, 
                 const QString& fileName) : Catalog(path, fileName) {
}

NetworkCatalog::NetworkCatalog(const QString& path, 
                 const QString& fileName) : Catalog(path, fileName){
}


/**
 * Catalogs constructor.  Catalogs keeps all the catalogs, keeps track of
 * the current catalog, and initializes them all from their file paths.
 * @param options = Pointer to Epsim Options.
 * @param path = path to optical disk.
 */
Catalogs::Catalogs(Options* options, const QString& path) {
    QString fileName = options->catalogFileName();
    networkCatalog_ = new NetworkCatalog(options->networkStudyPath(), fileName);
    systemCatalog_ = new SystemCatalog(options->systemCatalogPath(), fileName);
    opticalCatalog_ = new OpticalCatalog(path, fileName);
    otherCatalog_ = new Catalog(options->systemCatalogPath(), fileName);
    catalogs_[Catalog::Network] = networkCatalog_;
    catalogs_[Catalog::System] = systemCatalog_;
    catalogs_[Catalog::Optical] = opticalCatalog_;
    catalogs_[Catalog::Other] = otherCatalog_;
    if (options->enableNetworkStorage())
        currentCatalog_ = networkCatalog_;
    else
        currentCatalog_ = systemCatalog_;
}

void Catalogs::setCatalogPath(Catalog::Source catalog, const QString& path) {
    catalogs_[catalog]->setPath(path);
}


/// TODO need an update Catalogs function to be called after systemDialog execs

void Catalogs::setCurrentCatalog(Catalog::Source catalog) {
    currentCatalog_ = catalogs_[catalog];
}

void Catalogs::addStudy(Study* study) {
    for (Iterator it = catalogs_.begin(); it != catalogs_.end(); ++it)
        (*it).second->addStudy(study);
}

void Catalogs::deleteStudy(Study* study) {
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

void Catalogs::relabel(const QString& oldLabel, const QString& newLabel) {
      for (Iterator it = catalogs_.begin(); it != catalogs_.end(); ++it)
        (*it).second->relabel(oldLabel, newLabel);
}

bool Catalogs::studyPresentOnOpticalDisk(Study* s) {
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


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
#include <qmessagebox.h>
#include <qobject.h>


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

void Catalog::load() {
    QFile f(filePath());
    loadFile(f);
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
        throw EpSim::IoError(file.name(), 
              QObject::tr("Cannot open file %1 for reading"));
    QDataStream in(&file);
    in.setVersion(5);
    Q_UINT32 magic;
    in >> magic;
    if (magic != MagicNumber) 
        throw EpSim::IoError(file.name(), 
              QObject::tr("File %1 is not a EP Study file"));
    readFromStream(in);
    if (file.status() != IO_Ok) 
        throw EpSim::IoError(file.name(), 
              QObject::tr("Error reading from file %1"));
}

void Catalog::saveFile(QFile& file) {
    if (!file.open(IO_WriteOnly)) 
        throw EpSim::IoError(file.name(), 
              QObject::tr("Cannot open file %1 for writing"));
    QDataStream out(&file);
    out.setVersion(5);
    out << (Q_UINT32)MagicNumber;
    writeToStream(out);
    if (file.status() != IO_Ok) 
        throw EpSim::IoError(file.name(), 
              QObject::tr("Error writing to file %1"));
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

// void OpticalCatalog::addStudy(Study* study) {
//     
// 
// }

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
 */
Catalogs::Catalogs(Options* options) {
    QString fileName = options->catalogFileName();
    networkCatalog_ = new NetworkCatalog(options->networkStudyPath(), fileName);
    systemCatalog_ = new SystemCatalog(options->systemCatalogPath(), fileName);
    opticalCatalog_ = new OpticalCatalog(options->opticalStudyPath(), fileName);
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
}

Catalogs::~ Catalogs() {
    // don't delete currentCatalog_ it is just a copy of
    // one of the below!
    delete opticalCatalog_;
    delete systemCatalog_;
    delete networkCatalog_;
    delete otherCatalog_;
}


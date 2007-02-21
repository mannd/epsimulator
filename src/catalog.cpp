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
#include "opticaldisk.h"
#include "options.h"

#include <qdir.h>

Catalog::Catalog(const QString& path, 
                 const QString& fileName) : path_(path), fileName_(fileName) {
}

QString Catalog::filePath() const {
    return QDir::cleanDirPath(path_ + "/" + fileName_);
}

OpticalCatalog::OpticalCatalog(const QString& path, 
                 const QString& fileName) : Catalog(path, fileName) {
}

SystemCatalog::SystemCatalog(const QString& path, 
                 const QString& fileName) : Catalog(path, fileName) {
}

NetworkCatalog::NetworkCatalog(const QString& path, 
                 const QString& fileName) : Catalog(path, fileName){
}

Catalogs::Catalogs(Options* options) {
    QString fileName = options->catalogFileName();
    networkCatalog_ = new NetworkCatalog(options->networkStudyPath(), fileName);
    systemCatalog_ = new SystemCatalog(options->systemCatalogPath(), fileName);
    opticalCatalog_ = new OpticalCatalog(options->opticalStudyPath(), fileName);
    otherCatalog_ = new Catalog(options->systemCatalogPath(), fileName);
    if (options->enableNetworkStorage())
        currentCatalog_ = networkCatalog_;
    else
        currentCatalog_ = systemCatalog_;
//    filePaths_ = new QStringList();
    if (options->enableNetworkStorage())
        appendFilePath(networkCatalog_->filePath());
    appendFilePath(systemCatalog_->filePath());
    appendFilePath(opticalCatalog_->filePath());
    appendFilePath(otherCatalog_->filePath());
}

void Catalogs::appendFilePath(const QString& path) {
    if (!path.isEmpty()) 
        filePaths_.append(path);
}

/// TODO need an update Catalogs function to be called after systemDialog execs

void Catalogs::setCurrentCatalog(Catalog::Source catalog) {
    switch (catalog) {
        case Catalog::Network:
            currentCatalog_ = networkCatalog_;
            break;
        case Catalog::System:
            currentCatalog_ = systemCatalog_;
            break;
        case Catalog::Optical:
            currentCatalog_ = opticalCatalog_;
            break;
        case Catalog::Other:
            currentCatalog_ = otherCatalog_;
            break;
        default:
            currentCatalog_ = systemCatalog_;
    }
}



void Catalogs::refresh() {
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


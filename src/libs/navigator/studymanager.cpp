/***************************************************************************
 *   Copyright (C) 2010 by EP Studios, Inc.                                *
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

#include "studymanager.h"

#include "coreconstants.h"
#include "error.h"
#include "fileutilities.h"
#include "opticaldisk.h"
#include "study.h"
#include "studywriter.h"
#include "systemstorage.h"

using EpCore::joinPaths;
using EpCore::makePath;
using EpCore::SystemStorage;
using EpHardware::EpOpticalDisk::OpticalDisk;
using EpNavigator::Catalog;
using EpStudy::Study;
using EpStudy::StudyManager;

StudyManager::StudyManager(OpticalDisk* disk,
                           StudyWriter* studyWriter,
                           const QString& networkPath,
                           Catalog::Source activeCatalog)
    : networkPath_(networkPath),
      opticalDisk_(disk),
      studyWriter_(studyWriter),
      activeCatalog_(activeCatalog) {
    init();
}

void StudyManager::init() {
    SystemStorage systemStorage;
    systemPath_ = systemStorage.path();
    // if (useNetwork_) {
    //     NetworkStorage networkStorage;
    //     networkPath_ = networkStorage.path();
    // }
}    

QString StudyManager::studiesPath(const QString& path) const {
    return EpCore::joinPaths(path, EpCore::Constants::EPSIM_STUDIES_DIRNAME);
}

QString StudyManager::opticalStudiesPath() const {
    return studiesPath(opticalDisk_->workingPath());
}

QString StudyManager::activeCatalogStudiesPath() const {
    QString path;
    switch (activeCatalog_) {
        case Catalog::Network:
            path = networkPath_;
            break;
        case Catalog::System:
            path = systemPath_;
            break;
        case Catalog::Optical:
            path = opticalPath_;
            break;
        default:
            Q_ASSERT(false);
    }
    return path;
}

Study* StudyManager::getPreregisterStudy(const QString& key) {
    Study* study = new Study;
    study->setKey(key);
    study->setPath(activeCatalogStudiesPath());
    study->load();
    return study;
}

void StudyManager::addStudy(Study* study) {
    if (!study)
        return;
    // StudyWriter::study() checks type of study (preregister or not)
    // and returns locations where study data will be written.
    StudyWriter::WriteLocations locations = studyWriter_->study(study);
    study->setPath(systemPath_);  // always write to System
    if (!study->makeStudyPath())
        throw EpCore::WriteError(study->filePath());
    study->save();
    if (locations & StudyWriter::WriteNetwork) {
        study->setPath(networkPath_);
        if (!study->makeStudyPath())
            throw EpCore::WriteError(study->filePath());
        study->save();
    }
    if (locations & StudyWriter::WriteOptical) {
        study->setPath(opticalPath_);
        if (!study->makeStudyPath())
            throw EpCore::WriteError(study->filePath());
        study->save();
    }
}

Study* StudyManager::study(const QString& key) {
    if (key.isEmpty())
        return 0;
    // lookup key, get new Study and set it to study.dat
    Study* study = new Study;
    study->setKey(key);
    study->setPath(activeCatalogStudiesPath());
    study->load();
    return study;
}

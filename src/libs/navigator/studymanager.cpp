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

// StudyManager knows how to read and write Studies to the 3
// catalog locations -- so that Study doesn't have to.
StudyManager::StudyManager() 
    : opticalDisk_(0),
      studyWriter_(0),
      activeCatalog_(Catalog::System), 
      useNetwork_(false), 
      study_(0) {
    init();
}

StudyManager::StudyManager(OpticalDisk* disk,
                           StudyWriter* studyWriter,
                           Catalog::Source activeCatalog,
                           bool useNetwork) 
    : opticalDisk_(disk), studyWriter_(studyWriter),
      activeCatalog_(activeCatalog), useNetwork_(useNetwork) {
    init();
}

void StudyManager::init() {
    SystemStorage systemStorage;
    systemPath_ = systemStorage.path();
}    

QString StudyManager::studiesPath(const QString& path) const {
    return EpCore::joinPaths(path, EpCore::Constants::EPSIM_STUDIES_DIRNAME);
}

QString StudyManager::networkStudiesPath() const {
    return studiesPath(networkPath_);
}

QString StudyManager::systemStudiesPath() const {
    return studiesPath(systemPath_);
}

QString StudyManager::opticalStudiesPath() const {
    //return studiesPath(opticalDisk_->workingPath());
    /// TODO fix OpticalDisk!
    return QString();
}

Study* StudyManager::getPreregisterStudy(const QString& key) {
    Study* study = new Study;
    study->setKey(key);
    if (useNetwork_)
        study->setPath(networkPath_);
    else
        study->setPath(systemPath_);
    study->load();
    return study;
}

void StudyManager::addStudy(Study* study) {
    if (!study)
        return;
    if (study->isPreregisterStudy())
        addPreregisterStudy(study);
    else addFullStudy(study);
}

void StudyManager::addPreregisterStudy(Study* study) {
    if (!study || !study->isPreregisterStudy())
        return;
    StudyWriter::WriteLocations locations = studyWriter_->study(study);
    QString studyPath(joinPaths(systemStudiesPath(), study->dirName()));
    if (!makePath(studyPath))
        return;
    if (locations & StudyWriter::WriteSystem) {
        study->setPath(studyPath);
        study->save();
    }
}

void StudyManager::addFullStudy(Study* study) {
    if (!study || study->isPreregisterStudy())
        return;
    return;
}


Study* StudyManager::study(const QString& /*key*/) {
    return 0;
}

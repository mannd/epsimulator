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

#include "fileutilities.h"
#include "study.h"
#include "systemstorage.h"

using EpCore::joinPaths;
using EpCore::SystemStorage;
using EpHardware::EpOpticalDisk::OpticalDisk;
using EpNavigator::Catalog;
using EpStudy::Study;
using EpStudy::StudyManager;

StudyManager::StudyManager() 
    : opticalDisk_(0),
      catalogSource_(Catalog::System), 
      useNetwork_(false), 
      study_(0) {
    init();
}

StudyManager::StudyManager(const QString& systemPath,
                           const QString& opticalPath,
                           const QString& networkPath,
                           const QString& otherPath)
    : systemPath_(systemPath), opticalPath_(opticalPath),
      networkPath_(networkPath), otherPath_(otherPath) {

}

StudyManager::StudyManager(OpticalDisk* disk,
                           Catalog::Source source,
                           bool useNetwork) 
    : opticalDisk_(disk), catalogSource_(source), useNetwork_(useNetwork) {
    init();
}

void StudyManager::init() {
    SystemStorage systemStorage;
    systemPath_ = systemStorage.path();
}    

Study* StudyManager::getPreregisterStudy(const QString& key) {
    Study* study = new Study;
    if (useNetwork_)
        study->setPath(networkPath_);
    else
        study->setPath(systemPath_);
    study->setKey(key);
    study->load();
    return study;
}

void StudyManager::addStudy(Study* study) {
    if (!study)
        return;
    if (study->isPreregisterStudy())
        addPreregisterStudy(study);
    // else addFullStudy();
}

void StudyManager::addPreregisterStudy(Study* study) {
    if (!study || !study->isPreregisterStudy())
        return;
}

Study* StudyManager::study(const QString& /*key*/) {
    return 0;
}

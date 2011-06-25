/***************************************************************************
 *   Copyright (C) 2010, 2011 by EP Studios, Inc.                          *
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

#ifndef STUDYMANAGER_H
#define STUDYMANAGER_H

#include "catalog.h"

#include <QString>

namespace EpHardware {
namespace EpOpticalDisk {
class OpticalDisk;
}}

namespace EpStudy {

// manages complex Study file handling
class StudyManager {
public:
    StudyManager();
    StudyManager(const QString& systemPath,
                 const QString& opticalPath,
                 const QString& networkPath,
                 const QString& otherPath = QString());
    StudyManager(EpHardware::EpOpticalDisk::OpticalDisk*,
                 // StudyTable*, ????
                 EpNavigator::Catalog::Source = EpNavigator::Catalog::System,
                 bool useNetwork = false);
                 

    Study* getPreregisterStudy(const QString& key);
    QString systemPath() const {return systemPath_;}

    bool useNetwork() const {return useNetwork_;}
    EpNavigator::Catalog::Source catalog() const {return catalogSource_;}

    void setSystemPath(const QString& systemPath) {
        systemPath_ = systemPath;
    }

    void setOpticalDisk(EpHardware::EpOpticalDisk::OpticalDisk*);
    void setOpticalPath(const QString& opticalPath) {
        opticalPath_ = opticalPath;
    }
    void setNetworkPath(const QString& networkPath) {
        networkPath_ = networkPath;
    }
    void setOtherPath(const QString& otherPath) {
        otherPath_ = otherPath;
    }
    void setUseNetwork(bool value) {useNetwork_ = value;}
    void setCatalog(EpNavigator::Catalog::Source source) {
        catalogSource_ = source;}
    void setStudy(Study*);

    void addStudyToCatalog(Study*);
    void addStudy(Study*);      // handles pre-register, network
    Study* study();
    Study* study(const QString& key);


private:
    void init();
    void addPreregisterStudy(Study*);

    QString systemPath_;
    QString opticalPath_;
    QString networkPath_;
    QString otherPath_;
    EpHardware::EpOpticalDisk::OpticalDisk* opticalDisk_;
    EpNavigator::Catalog::Source catalogSource_;
    bool useNetwork_;
    Study* study_;
};

} // namespace EpStudy

#endif

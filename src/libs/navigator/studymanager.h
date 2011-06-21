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

    QString systemPath() const {return systemPath_;}
    EpNavigator::Catalog::Source source() const {return catalogSource_;}

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
    void setCatalog(EpNavigator::Catalog::Source);
    void setStudy(Study*);

    void addStudyToCatalog(Study*);
    void addStudy(Study*);      // handles pre-register, network
    Study* study();


private:
    QString systemPath_;
    QString opticalPath_;
    QString networkPath_;
    QString otherPath_;
    bool useNetwork_;
    EpNavigator::Catalog::Source catalogSource_;
    EpHardware::EpOpticalDisk::OpticalDisk* opticalDisk_;
    Study* study_;
};

} // namespace EpStudy

#endif

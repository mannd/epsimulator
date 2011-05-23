/***************************************************************************
 *   Copyright (C) 2011 by EP Studios, Inc.                                *
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

#include "studydatabase.h"

#include "study.h"

using EpStudy::StudyDatabase;

StudyDatabase::StudyDatabase() 
    : opticalCatalogPath_(),
      systemCatalogPath_(),
      networkCatalogPath_(),
      updateFlag_(NoUpdate) {
}

StudyDatabase::StudyDatabase(const QString& opticalCatalogPath,
                  const QString& systemCatalogPath,
                  const QString& networkCatalogPath,
                  const UpdateFlag updateFlag) 
    : opticalCatalogPath_(opticalCatalogPath),
      systemCatalogPath_(systemCatalogPath),
      networkCatalogPath_(networkCatalogPath),
      updateFlag_(updateFlag) {
} 

void StudyDatabase::addStudy(const EpStudy::Study* study, 
                             const QString& location,
                             const QString& side,
                             const QString& labName,
                             const QString& machineName) {
    if (updateFlag_ | OpticalUpdate)
        ;

}

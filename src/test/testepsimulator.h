/***************************************************************************
 *   Copyright (C) 2007 by EP Studios, Inc.                                *
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

#include <QtTest/QtTest>

namespace EpStudy { class Study; }
namespace EpGui { class PatientDialog; }

using EpStudy::Study;
using EpGui::PatientDialog;

class TestEpSimulator : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();

    void testStudyConstructor();
    void testStudyCopyConstructor();
    void testStudyOperatorEquals();
    void testStudiesSimilar();    
    void testFullName_data();
    void testFullName();
    void testNameWithSpaces();
    void testName();
    void testFilePath();
    void testStudyKey();
    void testStudyFileName();
    void testStudyLoadSave();
    void testIsPreregisterStudy();
    void testEF();
    void testOptions();
    void testOptionsFlags();
    void testOpticalDisk();
    void testPatientDialog();
    void testPatientDialogCalculations();
    void testGetSetPatientDialogDefaultStudies();
    void testFilterCatalogDialog();
    void testPasswordDialog();
    void testPasswordHandler();
    void testUser();
    void testCatalog();
    void testCatalogs();
    void testCatalogAddStudy();
    void testCatalogComboBox();
    void testDeleteDir();
    void testCopyDir();
    void testSaturation();
    void testAmplifier();
    void testAmplifierStream();
    void testChannel();
    void testStudyConfigurationStream();
    void testBloodPressure();
    void testConversions();
    void testJoinPaths();
    void testIsRemovableMedia();
    void testListSelector();
    void testProtocol();
    void testInterval();
    void testMark();
    void testColumnFormat();
    void testSystemStorage();
    void testRemovableMedia();
    void testPatient();
    void testStudyTable();
    void testLocalStorage();
    void testSystemStorageInit();
    void testRemovableOpticalDisk();
    void testOpticalDiskCache();
    void testHardDriveCache();
    void testEmulatedOpticalDiskCache();
    void testForceCacheEmulatedOpticalDiskCache();
    void testCreateOpticalCatalogDbConnection();
    void testMakePath();
    void testStudyManager();
    void testStudyManagerLoadStudy();

    void cleanupTestCase();

private:
    QString workingPath_;

    void testStudyDefaults(const Study&);
    void testStudiesEqual(const Study&, const Study&);
    void testStudiesSimilar(const Study&, const Study&);
    void completePatientDialog(PatientDialog* p);
};

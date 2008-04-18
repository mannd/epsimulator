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

class Study;
class PatientDialog;

class TestEpSimulator : public QObject {
    Q_OBJECT

private slots:
    // class Study
    void testStudyConstructor();
    void testStudyCopyConstructor();
    void testStudyOperatorEquals();
    void testStudiesSimilar();    
    void testFullName_data();
    void testFullName();
    void testFilePath();
    void testStudyKey();
    void testStudyFileName();
    void testStudyLoadSave();
    // class Options
    void testOptions();
    // class OpticalDisk
    void testOpticalDisk();
    // class PatientDialog
    void testPatientDialog();
    void testPatientDialogCalculations();
    void testGetSetPatientDialogDefaultStudies();
    // class FilterCatalogDialog
    void testFilterCatalogDialog();
    // class PasswordDialog
    void testPasswordDialog();
    // class PasswordHandler
    void testPasswordHandler();
    // class User
    void testUser();
    // class VersionInfo
    void testVersionInfo();
    void testVersionOk();
    // class Catalog, Catalogs
    void testCatalog();
    void testCatalogs();
    void testCatalogAddStudy();
    // class CatalogComboBox
    void testCatalogComboBox();
    // function deleteDir()
    void testDeleteDir();
    // function copyDir()
    void testCopyDir();
    // class Saturation
    void testSaturation();
    // class Amplifier
    void testAmplifier();

    void cleanupTestCase();

private:
    void testStudyDefaults(const Study&);
    void testStudiesEqual(const Study&, const Study&);
    void testStudiesSimilar(const Study&, const Study&);
    void completePatientDialog(PatientDialog* p);
};

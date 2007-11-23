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

#include "testepsimulator.h"

#include "catalog.h"
#include "catalogcombobox.h"
#include "epfuns.h"
#include "filtercatalogdialog.h"
#include "options.h"
#include "passworddialog.h"
#include "passwordhandler.h"
#include "patientdialog.h"
#include "study.h"
#include "user.h"
#include "versioninfo.h"

#include <cmath>

void TestEpSimulator::testStudyConstructor() {
    Study s;
    testStudyDefaults(s);
}

void TestEpSimulator::testStudyCopyConstructor() {
    Study s1;
    Study s2(s1);
    testStudyDefaults(s2);
    testStudiesEqual(s1, s2);
}

void TestEpSimulator::testStudyOperatorEquals() {
    Study s1;
    Study s2 = s1;
    testStudyDefaults(s2);
    testStudiesEqual(s1, s2);
}
    
void TestEpSimulator::testStudiesSimilar() {
    Study s1, s2;
    testStudiesSimilar(s1, s2);
}

void TestEpSimulator::testFullName_data() {
    QTest::addColumn<bool>("lastFirst");
    QTest::addColumn<bool>("useMiddleName");
    QTest::addColumn<QString>("lastName");
    QTest::addColumn<QString>("firstName");
    QTest::addColumn<QString>("middleName");
    QTest::addColumn<QString>("results");

    QTest::newRow("default") << false << false 
        << "Mann" << "David" << "E." << "David Mann"; 
    QTest::newRow("last first") << true << false
        << "Mann" << "David" << "E." << "Mann, David";
    QTest::newRow("use middle") << false << true
        << "Mann" << "David" << "E." << "David E. Mann";
    QTest::newRow("last first use middle") << true << true
        << "Mann" << "David" << "E." << "Mann, David E.";
    QTest::newRow("default first white space") 
        << false << false << "     Mann          " 
        << "         David          "
        << "   E.     " << "David Mann"; 
    QTest::newRow("last first white space") << true << false
        << "     Mann          " 
        << "         David          " 
        << "   E.     " << "Mann, David";
    QTest::newRow("use middle  white space") << false << true
        << "     Mann          " 
        << "         David          " 
        << "   E.     " << "David E. Mann";
    QTest::newRow("last first use middle white space") << true << true
        << "     Mann          " 
        << "         David          " 
        << "   E.     " << "Mann, David E.";
}

void TestEpSimulator::testFullName() {
    Study s;
    QFETCH(bool, lastFirst);
    QFETCH(bool, useMiddleName);
    QFETCH(QString, lastName);
    QFETCH(QString, firstName);
    QFETCH(QString, middleName);
    QFETCH(QString, results);

    Name name;
    name.first = firstName;
    name.middle = middleName;
    name.last = lastName;
    s.setName(name);
    QCOMPARE(s.name().fullName(lastFirst, useMiddleName), results);
}

void TestEpSimulator::testFilePath() {
    Study s;
    s.setPath("/home/");
    QCOMPARE(s.filePath(), QString("/home/study.dat"));
    s.setPath("");
    QCOMPARE(s.filePath(), QString("/study.dat"));
    QCOMPARE(s.filePath(), QString("/" + s.studyFileName()));
}

void TestEpSimulator::testStudyKey() {
    Study s;
    // below won't work in debug mode as there is an assert in Study
    // that intercepts this
    // TS_ASSERT(s.key() == QString::null);
    Name name;
    name.first = "John";
    name.last = "Doe";
    s.setName(name);
    QCOMPARE(s.key(), "Doe_John_" + 
        s.dateTime().toString("ddMMyyyyhhmmsszzz"));
    // add some spaces
    name.first = "    John  ";
    name.last = "   Doe        ";
    s.setName(name);
    QCOMPARE(s.key(), "Doe_John_" + 
            s.dateTime().toString("ddMMyyyyhhmmsszzz"));
}

void TestEpSimulator::testStudyFileName() {
    Study s;
    QVERIFY(s.studyFileName() == "study.dat");
    s.setPath("garbage");
    Name n = {"Smith", "John", ""};
    s.setName(n);
    QVERIFY(s.filePath() == "garbage/study.dat");
}

void TestEpSimulator::testOptions() {
    Options* o = Options::instance();
    QString s = o->opticalStudyPath();
    QVERIFY(s == o->opticalStudyPath());
    o->setOpticalStudyPath(s);
    QVERIFY(s == o->opticalStudyPath());
    o->destroy();
}

void TestEpSimulator::testPatientDialog() {
    Study* s1 = new Study;
    Study* s2 = new Study;
    Name n;
    n.last = "Test3";
    n.first = "John";
    s1->setName(n);
    PatientDialog* p = new PatientDialog;
    p->setFields(s1);
    p->getFields(s2);
    QVERIFY(s2->name().last == "Test3");
    QVERIFY(s2->name().first == "John");
    testStudiesEqual(*s1, *s2);
    delete p;
    delete s1;
    delete s2;
}

void TestEpSimulator::testPatientDialogCalculations() {
    Study* s = new Study;
    double heightIn = 72, weightLbs = 143;
    s->setHeightIn(heightIn);
    s->setWeightLbs(weightLbs);
    PatientDialog* p = new PatientDialog;
    p->setFields(s);
    // Below should do all the calculations by faking
    // tabbing through the fields.
    completePatientDialog(p);
    p->getFields(s);
    double delta = 0.001;
    double result = heightIn * 2.54;
    QVERIFY(result - delta < s->height() < result + delta);
    result = weightLbs * 0.45;
    QVERIFY(result - delta < s->weight() < result + delta);
    result = std::sqrt((s->height() * s->weight()) / 3600);
    QVERIFY(result - delta < s->bsa() < result + delta);
    // Now test manually setting bsa.
    double bsa = 12345.1;
    s->setBsa(bsa);
    s->setBsaManualEdit(true);
    p->setFields(s);
    completePatientDialog(p);
    p->getFields(s);
    QVERIFY(bsa - delta < s->bsa() < bsa + delta);
    delete p;
    delete s;
}

void TestEpSimulator::testFilterCatalogDialog() {
    FilterCatalogDialog* filterCatalog = new FilterCatalogDialog;
    QVERIFY(filterCatalog->lastNameFilter() == "*");
    QVERIFY(filterCatalog->firstNameFilter() == "*");
    QVERIFY(filterCatalog->mrnFilter() == "*");
    QVERIFY(filterCatalog->studyConfigFilter() == "*");
    QVERIFY(filterCatalog->studyNumberFilter() == "*");
    QVERIFY(filterCatalog->studyLocationFilter() == "*");
    delete filterCatalog;
}


void TestEpSimulator::testGetSetPatientDialogDefaultStudies() {
    PatientDialog pd;
    Study *s1 = new Study;
    Study *s2 = new Study;
    Name n1 = {"Doe", "John", "E"};
    s1->setName(n1);
    Name n2 = {"Dale", "Jane", "A"};
    s2->setName(n2);
    QString k1 = s1->key();
    QString k2 = s2->key(); // generate both keys
    QVERIFY(k1 != k2);
    pd.setFields(s1);
    pd.getFields(s2);
    // making study date time the same won't affect keys now
    QVERIFY(s1->key() != s2->key());
    QVERIFY(s1->name().last == s2->name().last);
    QVERIFY(s1->dateTime() == s2->dateTime());
    Study *s3 = new Study;
    Study *s4 = new Study;
    Name n3 = {"Dunn", "James", "F"};
    s3->setName(n3);
    pd.setFields(s3);
    pd.getFields(s4);
    QVERIFY(s3->key() == s4->key()); // here keys are set
                                     // after dateTime is made
                                     // the same
    s3->setName(n3);
    
    delete s1;
    delete s2;
    delete s3;
    delete s4;
}

void TestEpSimulator::testPasswordDialog() {
    Options* o = Options::instance();
    PasswordDialog* d = new PasswordDialog(o);
    // test consistency, will fail if line edit is giving gibberish
    bool result1 = d->testPassword();
    delete d;
    PasswordDialog* d1 = new PasswordDialog(o);
    bool result2 = d1->testPassword();
    QVERIFY(result1 == result2);
    // another consistency test
    QString pw = "abcdefghi";
    d1->setPassword(pw);
    QVERIFY(!d1->testPassword());
    // different password
    d1->setPassword("nonsense");
    QVERIFY(!d1->testPassword());
    // need way to set password to do more testing
    d1->setPassword("");
    QString emptyPasswordHash = o->passwordHash();
    d1->setPassword("admin");   // should be same as empty pw
    QString adminPasswordHash = o->passwordHash();
    QCOMPARE(emptyPasswordHash, adminPasswordHash);   
    delete d1;
    o->destroy();
}

void TestEpSimulator::testPasswordHandler() {
    Options* o = Options::instance();
    PasswordHandler* ph = new PasswordHandler(o);
    QString s = "password1";
    ph->setPassword(s);
    QString testPasswordHash = o->passwordHash();
    QVERIFY(ph->testPassword(s));
    QVERIFY(!ph->testPassword("wrongpassword"));
    // tests for non-zero password hash    
    QVERIFY(!o->passwordHash().isEmpty());
    ph->setPassword("random");
    QVERIFY(testPasswordHash != o->passwordHash());
    delete ph;
    o->destroy();
}

void TestEpSimulator::testUser() {
    User* u = User::instance();
    QVERIFY (!u->machineName().isNull());
    QVERIFY (u->role() == QObject::tr("EPSIMUSER"));
    u->makeAdministrator(true);
    QVERIFY(u->role() == QObject::tr("ADMINISTRATOR"));
    QVERIFY(u->name() == QObject::tr("ADMINISTRATOR"));
    u->makeAdministrator(false);
    QVERIFY(u->role() == QObject::tr("EPSIMUSER"));
    u->destroy();
}
    
void TestEpSimulator::testVersionInfo() {
    VersionInfo* v = VersionInfo::instance();
    QVERIFY(v->programName() == QObject::tr("EP Simulator"));
    QVERIFY(v->appName() == "epsimulator");
    v->destroy();
}
    
void TestEpSimulator::testVersionOk() {
    VersionInfo* v = VersionInfo::instance();
    QVERIFY(VersionInfo::versionOk(v->versionMajor(), v->versionMinor()));
    QVERIFY(!VersionInfo::versionOk(-99999,0));
    QVERIFY(!VersionInfo::versionOk(-99999,-999999));
    QVERIFY(!VersionInfo::versionOk(0,-999999));
    v->destroy();
}

void TestEpSimulator::testCatalog() {
    Catalog c("../System", "catalog.dat");
    QVERIFY(c.filePath() == "../System/catalog.dat");
    c.setPath("/testpath/");
    // make sure no duplicate backslashes
    QVERIFY(c.filePath() == "/testpath/catalog.dat");
    // test Catalog subclasses
//     Catalog* cp = new OpticalCatalog("../System", "catalog.dat");
//     cerr <<   "catalog filepath" << cp->filePath();
//    delete cp;
}

void TestEpSimulator::testCatalogs() {
    Options* o = Options::instance();
    Catalogs* c1 = new Catalogs(o, "");
    c1->setCatalogPath(Catalog::Other, "/tmp/test");
    c1->setCurrentCatalog(Catalog::Other);
    QVERIFY(c1->currentCatalog()->path() == "/tmp/test");
    delete c1;
}

void TestEpSimulator::testCatalogAddStudy() {
    Options* o = Options::instance();
    Catalogs* cats = new Catalogs(o, o->opticalStudyPath());
    Study* s = new Study;
    Name n = {"Doe", "John", "E"};
    s->setName(n);
    // key will be null unless there is a last name
    QString key = s->key();
    cats->addStudy(s);
    Study s1 = (*cats->currentCatalog())[key].study;
    QVERIFY(s1.key() == key);
    cats->deleteStudy(s);
    delete s;
    delete cats;
}


void TestEpSimulator::testCatalogComboBox() {
    CatalogComboBox* c = new CatalogComboBox;
    Options* o = Options::instance();
    bool originalEnableNetwork = o->enableNetworkStorage();
    o->setEnableNetworkStorage(false);
    // must refresh catalogcombobox after system options changed!
    c->refresh();
    QVERIFY(c->source() == Catalog::System);
    QVERIFY(c->currentItem() == 0);   // there should be no Network
    c->setSource(Catalog::Network);  // should not work because of above
    QVERIFY(!o->enableNetworkStorage());
    QVERIFY(c->currentItem() == 0);
    QVERIFY(c->source() != Catalog::Network);
    // should be System
    o->setEnableNetworkStorage(true);
    c->refresh();
    QVERIFY(c->source() == Catalog::System);
    c->setSource(Catalog::Network);
    QVERIFY(c->source() == Catalog::Network);
    c->setSource(Catalog::Optical);
    QVERIFY(c->source() == Catalog::Optical);
    c->setSource(Catalog::Other);
    QVERIFY(c->source() == Catalog::Other);
    c->setSource(Catalog::System);
    QVERIFY(c->source() == Catalog::System);
    // reset options
    o->setEnableNetworkStorage(originalEnableNetwork);
    delete c;
}
    

void TestEpSimulator::cleanupTestCase() {
}

// private functions

void TestEpSimulator::testStudyDefaults(const Study& study) {
    QVERIFY(study.sex() == Male);
    QVERIFY(study.vagalTone() == DEFAULT_VAGAL_TONE);
    QVERIFY(study.sympatheticTone() == DEFAULT_SYMPATHETIC_TONE);
    QVERIFY(study.ef() == DEFAULT_EF);
    QVERIFY(study.ischemia() == false);
    QVERIFY(study.dateOfBirth() == DEFAULT_BIRTH_DATE);
    QVERIFY(study.height() == study.weight() 
            == study.heightIn() == study.weightLbs() 
            == study.bsa() == 0);
    QVERIFY(study.bsaManualEdit() == false);
    QVERIFY(study.dateTime().date() == QDateTime::currentDateTime().date());
}
    

void TestEpSimulator::testStudiesEqual(const Study& s1, const Study& s2) {
    testStudiesSimilar(s1, s2);
    QCOMPARE(s1.dateTime(), s2.dateTime());  
}

// won't test date or time
void TestEpSimulator::testStudiesSimilar(const Study& s1, const Study& s2) {
    QCOMPARE(s1.name().first, s2.name().first);
    QCOMPARE(s1.name().middle, s2.name().middle);
    QCOMPARE(s1.name().last, s2.name().last);
    QCOMPARE(s1.mrn(), s2.mrn());
    QCOMPARE(s1.dateOfBirth(), s2.dateOfBirth());
    QCOMPARE(s1.sex(), s2.sex());
    QCOMPARE(s1.height(), s2.height());
    QCOMPARE(s1.weight(), s2.weight());
    QCOMPARE(s1.bsa(), s2.bsa());
    QCOMPARE(s1.bsaManualEdit(), s2.bsaManualEdit());
    QCOMPARE(s1.ef(), s2.ef());
    QCOMPARE(s1.ischemia(), s2.ischemia());
    QCOMPARE(s1.vagalTone(), s2.vagalTone());
    QCOMPARE(s1.sympatheticTone(), s2.sympatheticTone());
    //QCOMPARE(s1.date(), s2.date());  // there is a slight chance 
    // this will fail around midnight!
    //QCOMPARE(s1.time(), s2.time());  // this will fail for 2 different
    // studies 
    QCOMPARE(s1.number(), s2.number());
    QCOMPARE(s1.path(), s2.path());
}

void TestEpSimulator::completePatientDialog(PatientDialog* p) {
    p->weightKgLineEdit_lostFocus();
    p->weightLbsLineEdit_lostFocus();
    p->heightCmLineEdit_lostFocus();
    p->heightInLineEdit_lostFocus();
}




QTEST_MAIN(TestEpSimulator)
//#include "testepsimulator.moc"

                                                                                
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

#include "amplifier.h"
#include "bloodpressure.h"
#include "catalog.h"
#include "catalogcombobox.h"
#include "changepassworddialog.h"
#include "epdefs.h"
#include "fileutilities.h"
#include "filtercatalogdialog.h"
#include "listselector.h"
#include "mockdatastream.h"
#include "opticaldisk.h"
#include "options.h"
#include "passworddialog.h"
#include "passwordhandler.h"
#include "patientdialog.h"
#include "patientdefs.h"
#include "recorder.h"
#include "saturation.h"
#include "study.h"
#include "studyconfiguration.h"
#include "user.h"

#include <QDir>
#include <QListView>
#include <QtDebug>

#include <cmath>

// note: tests depend on working directory == .../epsimulator

// import all the namespaces, why not?
using namespace Ep;
using namespace EpCore;
using namespace EpGui;
using namespace EpPatient;
using namespace EpNavigator;
using namespace EpHardware;
using namespace EpHardware::EpOpticalDisk;
using namespace EpHardware::EpAmplifier;
using namespace EpRecorder;
using namespace EpStudy;

void TestEpSimulator::initTestCase() {
    QString workingPath = QDir::currentPath();
    QDir workingDir = QDir::current();
    if (workingDir.dirName() == "test") {
        workingDir.cdUp();
        workingDir.cdUp();
    }
    else if (workingDir.dirName() == "bin") 
        workingDir.cdUp();
    QDir::setCurrent(workingDir.path());
    if (workingDir.dirName() != "epsimulator")
        QFAIL("Tests need to be run from the project directory (epsimulator"
              " or epsimulator/bin).");
    QDir dir("tmp");
    if (!dir.exists())
        workingDir.mkdir("tmp");
    QCOMPARE(dir.exists(), true);
    workingPath_ = workingDir.absolutePath();
    //qDebug() << workingPath_;
    // must set below up for QSettings to work right
    qApp->setOrganizationName("EP Studios");
    qApp->setOrganizationDomain("epstudiossoftware.com");
    qApp->setApplicationName("EPSimulator");
}

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

    Name name(lastName, firstName, middleName);
    s.setName(name);
    QCOMPARE(s.name().testFullName(lastFirst, useMiddleName), results);
}

void TestEpSimulator::testName() {
    QString last = "  Mahoney  ";
    QString first = "Ladeda";
    QString middle = "Middle";
    Name n(last, first, middle);
    QVERIFY(n.lastFirstMiddle() == "Mahoney, Ladeda Middle");
    QVERIFY(n.firstMiddleLast() == "Ladeda Middle Mahoney");
    QVERIFY(n.lastFirst() == "Mahoney, Ladeda");
    QVERIFY(n.last() == "Mahoney");
    Name n1(last, first);
    QVERIFY(n1.lastFirstMiddle() == "Mahoney, Ladeda");
    QVERIFY(n1.firstMiddleLast() == "Ladeda Mahoney");
    QVERIFY(n1.lastFirst() == "Mahoney, Ladeda");
    QVERIFY(n1.last() == "Mahoney");
}

void TestEpSimulator::testNameWithSpaces() {
    QString last = "  Mahoney  ";
    QString first = "Ladeda  ";
    QString middle = " M ";
    Name n(last, first, middle);
    QVERIFY(n.lastFirstMiddle() == "Mahoney, Ladeda M");
    QVERIFY(n.firstMiddleLast() == "Ladeda M Mahoney");
    QVERIFY(n.lastFirst() == "Mahoney, Ladeda");
    QVERIFY(n.last() == "Mahoney");
    Name n1(last, first);
    QVERIFY(n1.lastFirstMiddle() == "Mahoney, Ladeda");
    QVERIFY(n1.firstMiddleLast() == "Ladeda Mahoney");
    QVERIFY(n1.lastFirst() == "Mahoney, Ladeda");
    QVERIFY(n1.last() == "Mahoney");

}

void TestEpSimulator::testSystemPath() {
    // obsolete.  systemPath() replaces systemDirectory()
    QVERIFY (EpCore::systemDirectory().absolutePath() ==
             EpCore::rootDirectory().absolutePath() + "/" + "System");
}

void TestEpSimulator::testFilePath() {
    Study s;
    s.setPath("/home/");
    QCOMPARE(s.filePath(), QString("/home/study.dat"));
    s.setPath(".");
    QCOMPARE(s.filePath(), QString("study.dat"));
    QCOMPARE(s.filePath(), QString(s.fileName()));
}

void TestEpSimulator::testStudyKey() {
    Study s;
    Name name("Doe", "John");
    s.setName(name);
    QCOMPARE(s.key(), "Doe_John_" + 
        s.dateTime().toString("ddMMyyyyhhmmsszzz"));
    QString first = "    John  ";
    QString last = "   Doe        ";
    name.setLastFirstMiddle(last, first);
    s.setName(name);
    QCOMPARE(s.key(), "Doe_John_" + 
            s.dateTime().toString("ddMMyyyyhhmmsszzz"));
    // make sure keys stay constant when name changes
    QString originalKey = s.key();
    last = "Nobody";
    name.setLastFirstMiddle(last, QString());
    s.setName(name);
    QCOMPARE(s.key(), originalKey);
    // make sure keys are copied
    Study s1(s);
    QCOMPARE(s1.key(), s.key());
    s1.setDateTime(QDateTime::currentDateTime());
    // keys should still be equal
    QCOMPARE(s1.key(), s.key());
    s1.resetKey();
    s1.setDateTime(QDateTime::currentDateTime());
    // now keys should be different
    QVERIFY(s1.key() != s.key());
    
}

void TestEpSimulator::testStudyFileName() {
    Study s;
    QVERIFY(s.fileName() == "study.dat");
    s.setPath("garbage");
    Name n("Smith", "John", "");
    s.setName(n);
    QVERIFY(s.filePath() == "garbage/study.dat");
}

void TestEpSimulator::testStudyLoadSave() {
    Study s;
    s.setPath("tmp");
    //qDebug() << "tmp path = " << s.filePath();
    Name name("Doe", "James", "");
    s.setName(name);
    s.save();
    Study s1;
    s1.setPath("tmp");
    s1.load();
    QCOMPARE(s.key(), s1.key());
    Study s2 = s1;
    QCOMPARE(s2.key(), s1.key());
    Study s3 = s2;
    name.setLastFirstMiddle("Roe", name.first(), name.middle());
    s3.setName(name);   // this should NOT change key, already = to s2 key()
    QCOMPARE(s3.key(), s2.key());
    s3.save();
    s3.load();
    name.setLastFirstMiddle("Coe", name.first(), name.middle());
    s3.setName(name);
    QCOMPARE(s3.key(), s2.key());
    // test with a pointer
    Study* sp = new Study;
    sp->setName(name);
    QString originalKey = sp->key();
    sp->setPath("tmp");
    sp->save();
    sp->load();
    QCOMPARE(sp->key(), originalKey);
    delete sp;
    Study s4;
    name.setLastFirstMiddle("NewName", name.first(), name.middle());
    s4.setName(name);
    QString key1 = s4.key();
    s4.setPath("tmp");
    s4.save();
    s4.load();
    name.setLastFirstMiddle("AnotherNewName", name.first(), name.middle());
    s4.setName(name);
    s4.save();
    s4.load();
    QCOMPARE(key1, s4.key());
    QVERIFY(s4.key() != s1.key());
    Study s5;
    s5.setPath("tmp");
    s5.load();
    // key should be same as the s4 key if loading is actually loading the key
    QCOMPARE(s4.key(), s5.key());    
}

void TestEpSimulator::testIsPreregisterStudy() {
    Study s;
    QVERIFY(s.isPreregisterStudy());
}

void TestEpSimulator::testEF() {
    Study s;
    QVERIFY(s.ef() > 0 && s.ef() < 100);
    s.setEf(110);
    QVERIFY(s.ef() > 0 && s.ef() < 100);
    s.setEf(-10);
    QVERIFY(s.ef() > 0 && s.ef() < 100);
    s.setEf(0);
    QVERIFY(s.ef() > 0 && s.ef() < 100);
    s.setEf(100);
    QVERIFY(s.ef() > 0 && s.ef() < 100);
    Study s1 = s;
    QCOMPARE(s.ef(), s1.ef());
};


void TestEpSimulator::testOptions() {
    Options* o = Options::instance();
    QString s = o->opticalStudyPath;
    QVERIFY(s == o->opticalStudyPath);
    o->opticalStudyPath = s;
    QVERIFY(s == o->opticalStudyPath);
    delete o;
}

void TestEpSimulator::testOptionsFlags() {
    Options* o = Options::instance();
    setFlag(o->filePathFlags, Options::EnableAcquisition);
    QVERIFY(o->filePathFlags.testFlag(Options::EnableAcquisition));
    clearFlag(o->filePathFlags, Options::EnableAcquisition);
    QVERIFY(!o->filePathFlags.testFlag(Options::EnableAcquisition));
    o->writeSettings();
    o->readSettings();  // enable acquisition should still be off
    QVERIFY(!o->filePathFlags.testFlag(Options::EnableAcquisition));
}

void TestEpSimulator::testOpticalDisk() {
    QCOMPARE(OpticalDisk::makeStudiesPath("test"), QString("test/studies"));
    QCOMPARE(OpticalDisk::studiesDirName(), QString("studies"));
    OpticalDisk* o = new OpticalDisk(".", ".");
    QCOMPARE(o->path(), o->labelPath());
    QCOMPARE(o->path(), QString("."));
    delete o;
    EmulatedOpticalDisk* e = new EmulatedOpticalDisk(".", ".", true);
    QVERIFY(e->path() != e->labelPath());
    delete e;
}

void TestEpSimulator::testPatientDialog() {
    Study* s1 = new Study;
    Study* s2 = new Study;
    QString last = "Test3";
    QString first = "John";
    Name n(last, first);
    s1->setName(n);
    PatientDialog* p = new PatientDialog;
    p->setFields(s1);
    p->getFields(s2);
    QVERIFY(s2->name().last() == "Test3");
    QVERIFY(s2->name().first() == "John");
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
    double calcHeight = heightIn * 2.54;
    QVERIFY(calcHeight - delta < s->height() 
	    && s->height() < calcHeight + delta);
    double calcWeight = weightLbs * 0.45359;
    QVERIFY(calcWeight - delta < s->weight() 
	    && s->weight() < calcWeight + delta);
    double calcBsa = std::sqrt((s->height() * s->weight()) / 3600);
    QVERIFY(calcBsa - delta < s->bsa() && s->bsa() < calcBsa + delta);
    // Now test manually setting bsa.
    double bsa = 12345.1;
    s->setBsa(bsa);
    s->setBsaManualEdit(true);
    p->setFields(s);
    completePatientDialog(p);
    p->getFields(s);
    QVERIFY(bsa - delta < s->bsa() && s->bsa() < bsa + delta);
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
    QVERIFY(filterCatalog->dateFilter() == FilterCatalogDialog::AnyDate);
    delete filterCatalog;
}


void TestEpSimulator::testGetSetPatientDialogDefaultStudies() {
    PatientDialog pd;
    Study *s1 = new Study;
    Study *s2 = new Study;
    Name n1("Doe", "John", "E");
    s1->setName(n1);
    Name n2("Dale", "Jane", "A");
    s2->setName(n2);
    QString k1 = s1->key();
    QString k2 = s2->key(); // generate both keys
    QVERIFY(k1 != k2);
    pd.setFields(s1);
    pd.getFields(s2);
    // making study date time the same won't affect keys now
    QVERIFY(s1->key() != s2->key());
    QVERIFY(s1->name().last() == s2->name().last());
    QVERIFY(s1->dateTime() == s2->dateTime());
    Study *s3 = new Study;
    Study *s4 = new Study;
    Name n3("Dunn", "James", "F");
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
    PasswordDialog* d = new PasswordDialog("0");
    // test consistency, will fail if line edit is giving gibberish
    d->setPassword("admin");
    QVERIFY(d->testPassword());
    delete d;
}

void TestEpSimulator::testPasswordHandler() {
    PasswordHandler* ph = new PasswordHandler();
    QVERIFY(ph->testPassword("admin"));
    QString s = "password1";
    ph->setPassword(s);
    QVERIFY(!ph->testPassword("admin"));
    QVERIFY(ph->testPassword("password1"));
    delete ph;
}

void TestEpSimulator::testUser() {
    User* u = User::instance();
    QVERIFY (!u->machineName().isEmpty());
    QVERIFY (u->role() == tr("EPSIMUSER"));
    u->setIsAdministrator(true);
    QVERIFY(u->role() == tr("ADMINISTRATOR"));
    QVERIFY(u->name() == tr("ADMINISTRATOR"));
    u->setIsAdministrator(false);
    QVERIFY(u->role() == tr("EPSIMUSER"));
    delete u;
}

void TestEpSimulator::testCatalog() {
    Options* o = Options::instance();
    Catalog c(o->systemCatalogPath, "catalog.dat");
    QVERIFY(c.filePath() == o->systemCatalogPath + "/catalog.dat");
    c.setPath("/testpath/");
    // make sure no duplicate backslashes
    QVERIFY(c.filePath() == "/testpath/catalog.dat");
    // test Catalog subclasses
//     Catalog* cp = new OpticalCatalog("../System", "catalog.dat");
//     cerr <<   "catalog filepath" << cp->filePath();
//    delete cp;
    delete o;
}

void TestEpSimulator::testCatalogs() {
    Options* o = Options::instance();
    Catalogs* c1 = new Catalogs(o, "tmp/");
    c1->setCatalogPath(Catalog::Other, "tmp/test2");
    c1->setCurrentCatalog(Catalog::Other);
    QVERIFY(c1->currentCatalog()->path() == "tmp/test2");
    delete c1;
}

void TestEpSimulator::testCatalogAddStudy() {
    Options* o = Options::instance();
    Catalogs* cats = new Catalogs(o, o->opticalStudyPath);
    Study* s = new Study;
    Name n("Doe", "John", "E");
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
    Options* o = Options::instance();
    CatalogComboBox* c = new CatalogComboBox(o->includeNetworkCatalog());
    clearFlag(o->filePathFlags, Options::EnableNetworkStorage);
    // must refresh catalogcombobox after system options changed!
    c->refresh(o->includeNetworkCatalog());
    QVERIFY(c->source() == Catalog::System);
    QVERIFY(c->currentIndex() == 0);   // there should be no Network
    c->setSource(Catalog::Network);  // should not work because of above
    QVERIFY(!o->filePathFlags.testFlag(Options::EnableNetworkStorage));
    QVERIFY(c->currentIndex() == 0);
    QVERIFY(c->source() != Catalog::Network);
    // should be System
    setFlag(o->filePathFlags, Options::EnableNetworkStorage, true);
    c->refresh(o->includeNetworkCatalog());
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
    delete c;
    delete o;
}
    
void TestEpSimulator::testDeleteDir() {
    QDir dir("tmp");
    if (!dir.exists("test"))
        QCOMPARE(dir.mkdir("test"), true);
    QCOMPARE(dir.exists("test"), true);
    // what happens if mkdir done twice? -- should be false
    QCOMPARE(dir.mkdir("test"), false);
    QCOMPARE(dir.exists("test"), true);
    dir.cd("test");
    dir.mkdir("test1");
    dir.cd("test1");
    Catalog c(dir.path());  // make a file here
    dir.cdUp(); // test
    // deleteDir() requires an absolute path
    QString dirAbsolutePath = dir.absolutePath();
    EpCore::deleteDir(dirAbsolutePath);
    QCOMPARE(dir.exists("test1"), false);
}

void TestEpSimulator::testCopyDir() {
    QDir dir("tmp");
    dir.mkdir("source");
    dir.mkdir("destination");
    dir.mkdir("source/study");
    Catalog c("tmp/source/study");
    EpCore::copyDir(dir.path() + "/source/study", dir.path() + "/destination");
    QCOMPARE(dir.exists("destination/study/catalog.dat"), true);
    EpCore::deleteDir(dir.absolutePath() + "/source");
    EpCore::deleteDir(dir.absolutePath() + "/destination");    
}

void TestEpSimulator::testSaturation() {
    Saturation sat;
    QCOMPARE(sat.number(), 0);
    sat = 99;
    QCOMPARE(sat.number(), 99);
    QVERIFY(sat == 99);
    QVERIFY(sat.percent() == "99%");
    sat.setNumber(110);
    QCOMPARE(sat.number(), 100);
    sat.setNumber(-90);
    QCOMPARE(sat.number(), 0);
    sat = 99;
    sat = sat + 2;
    QCOMPARE(sat.number(), 100);
    --sat;
    QCOMPARE(sat.number(), 99);
    sat -= 9;
    QCOMPARE(sat.number(), 90);
    sat = 1;
    sat--;
    QCOMPARE(sat.number(), 0);
    Saturation sat2 = sat--;
    QCOMPARE(sat2.number(), 0);
    QCOMPARE(sat.number(), 0);
    QVERIFY(sat2 == sat);
    ++sat2;
    QVERIFY(sat2 != sat);
}

void TestEpSimulator::testAmplifier() {
    Amplifier amp;  // default is 48 channels
    QCOMPARE(amp.numChannels(), 48);
    QCOMPARE(amp.numCIMConnections(), 2);
    Amplifier amp2(128);
    QCOMPARE(amp2.numCIMConnections(), 7);
    Amplifier amp3(64);
    QCOMPARE(amp3.numCIMConnections(), 3);
    EpAmplifier::Channel* c1 = amp3.channel(1);
    // 1st 12 channels are ECG channels always
    QCOMPARE(c1->channelType(), EpAmplifier::Channel::ECG);
    QVERIFY(c1->channelType() == EpAmplifier::Channel::ECG);
    QVERIFY(c1->label() == tr("I"));
    QVERIFY(c1->highPassFilter() == 0.05);
    QVERIFY(c1->lowPassFilter() == 150);
    QVERIFY(c1->notchFilter() == true);
    EpAmplifier::Channel* c12 = amp2.channel(12);
    QVERIFY(c12->label() == tr("V6"));
    EpAmplifier::Channel* c13 = amp.channel(13);
    QVERIFY(c13->channelType() == EpAmplifier::Channel::Pressure);
    QVERIFY(c13->label() == tr("P1"));
    EpAmplifier::Channel* c17 = amp.channel(17);
    QVERIFY(c17->channelType() == EpAmplifier::Channel::NotUsed);
    QVERIFY(c17->label().isEmpty());
    EpAmplifier::Channel* clast = amp3.channel(amp3.numChannels());
    QVERIFY(clast->channelType() == EpAmplifier::Channel::Stim2);
}

void TestEpSimulator::testAmplifierStream() {
    Amplifier a;
    QCOMPARE(a.numChannels(), 48);
    a.channel(1)->setLabel("XYZ");
    QVERIFY(a.channel(1)->label() == "XYZ");
    MockDataStream<Amplifier> dataStream;
    a.save(&dataStream);
    Amplifier b;
    b.load(&dataStream);
    QVERIFY(b.channel(1)->label() == "XYZ");
    Amplifier c;
    QVERIFY(c.channel(1)->label() == "I");
    MockSystemData<Amplifier> systemDataStream;
    systemDataStream.save(c);
    systemDataStream.load(c);
    QVERIFY(c.channel(1)->label() == "I");
    c.channel(1)->setLabel("ZZZ");
    systemDataStream << c;
    Amplifier d;
    systemDataStream >> d;
    QVERIFY(d.channel(1)->label() == "ZZZ");
    d.channel(2)->setLabel("YYY");
    MockSystemData<Amplifier> m;
    m << d;
    Amplifier f;
    m >> f;
    QVERIFY(f.channel(2)->label() == "YYY");
}

void TestEpSimulator::testStudyConfigurationStream() {
    StudyConfiguration sc;
    QVERIFY(sc.name() == tr("<default>"));
    sc.setName("TEST");
    QVERIFY(sc.name() == "TEST");
    MockSystemData<StudyConfiguration> m;
    m << sc;
    StudyConfiguration sc1;
    m >> sc1;
    QVERIFY(sc1.name() == "TEST");
}


void TestEpSimulator::testStudyConfigurations() {
    StudyConfigurations list;
    QVERIFY(list.size() == 1);
    StudyConfiguration config;
    config.setName("testing123");
    list.add(config);
    QVERIFY(list.size() == 2);
    for (int i = 0; i < list.size(); ++i)
        if (list[i].name() == tr("<default>")) {
            QVERIFY(list[i].amplifier()->channel(1)->label() == tr("I"));
            QVERIFY(list[i].amplifier()->numChannels()
                == Options::instance()->numChannels);
        }
    list.remove("testing123");
    QVERIFY(list.size() == 1);
    list.remove("No Such Config");
    QVERIFY(list.size() == 1);
    // ? can the default study config be removed?
    QVERIFY(list[0].name() == tr("<default>"));
    list.add(config);
    QVERIFY(list.size() == 2);
    MockSystemData<StudyConfigurations> m;
    m << list;
    StudyConfigurations list2;
    m >> list2;
    QVERIFY(list.size() == 2);
}

void TestEpSimulator::testBloodPressure() {
    BloodPressure bp(100, 50);
    QVERIFY(bp.bp() == "100/50");
    QVERIFY(bp.meanBp() == 66);
    QVERIFY(bp.meanBp(60) == 66);
    QVERIFY(bp.meanBp(BloodPressure::rateCutoff) == 75);

    // give bad blood pressure
    BloodPressure bp1(-100, -80);
    QVERIFY(bp1.bp() == "0/0");
    QVERIFY(bp1.meanBp() == 0);
    // another bad bp
    BloodPressure bp2(50, 100);
    QVERIFY(bp2.bp() == "100/100");
    bp2.setSystolic(150);
    bp2.setDiastolic(90);
    QVERIFY(bp2.bp() == "150/90");
    bp2.setBp(200, 100);
    QVERIFY(bp2.bp() == "200/100");

}

void TestEpSimulator::testConversions() {
    HeartRate rate = 100;
    QVERIFY(rateToInterval(rate) == 600);
    msec cycleLength = 600;
    QVERIFY(intervalToRate(cycleLength) == 100);
    rate = 60;
    QVERIFY(rateToInterval(rate) == 1000);
}

void TestEpSimulator::testJoinPaths() {
#ifdef Q_OS_WIN
    QString p1 = "\\hello\\.\\";
    QString p2 = "\\tmp";
#else
    QString p1 = "///hello/./";
    QString p2 = "/tmp";
#endif
    QVERIFY(joinPaths(p1, p2) == "/hello/tmp");
    // note that trailing slash is always removed
    p2 = "/tmp/";
    QVERIFY(QDir::cleanPath(p2) == "/tmp");
    QVERIFY(joinPaths(p1, p2) == "/hello/tmp");
    // backslashs are converted to forward slashes
    QString p3 = "\\hello";
    QVERIFY(joinPaths(p1, p2) == "/hello/tmp");
}

void TestEpSimulator::testIsRemovableMedia() {
#ifdef Q_OS_WIN
    QDir dir("C:\\Windows");
    QVERIFY(!EpCore::isRemovableMedia(dir));
    dir.setPath("D:\\");
    QVERIFY(EpCore::isRemovableMedia(dir));
#else
    QDir dir("/home/mannd/test");
    QVERIFY(!EpCore::isRemovableMedia(dir));
    dir.setPath("/media/cdrom");
    QVERIFY(EpCore::isRemovableMedia(dir));
#endif
}

void TestEpSimulator::testListSelector() {
    QStringList list1, list2;
    list1 << "test1" << "test2";
    list2 << "test3" << "test4";
    ListSelector ls(list1, list2, new QListView, new QListView);
    QVERIFY(ls.all().size() == 4);

}

void TestEpSimulator::testProtocol() {
    QStringList names = Protocol::protocolNames(Protocol::defaultItems());
    QVERIFY(names[0] == tr("<default>"));
    StudyConfiguration config;
    names = Protocol::protocolNames(config.protocolList());
    QVERIFY(names[0] == tr("<default>"));
}

void TestEpSimulator::testColumnFormat() {
    QList<Interval> intervals;
    intervals.append(Interval("Int1"));
    intervals.append(Interval("Int2"));
    ColumnFormat cf("Test", intervals);
    QVERIFY(cf.name() == "Test");
    QVERIFY(cf.intervals()[0].name() == "Int1");
    QVERIFY(cf.intervals()[1].name() == "Int2");
}

void TestEpSimulator::cleanupTestCase() {
    bool workingPathUnchanged = QDir::currentPath() == workingPath_;
    QVERIFY(workingPathUnchanged);
    if (workingPathUnchanged) {
	EpCore::deleteDir(workingPath_ + "/tmp");
	QCOMPARE(QDir::current().exists("tmp"), false);
    }
}

// private functions

void TestEpSimulator::testStudyDefaults(const Study& study) {
    QVERIFY(study.sex() == Male);
    //QVERIFY(study.vagalTone() == DEFAULT_VAGAL_TONE);
    //QVERIFY(study.sympatheticTone() == DEFAULT_SYMPATHETIC_TONE);
    //QVERIFY(study.ef() == DEFAULT_EF);
    QVERIFY(study.ischemia() == false);
    //QVERIFY(study.dateOfBirth() == DEFAULT_BIRTH_DATE);
    QVERIFY(study.height() == 0);
    QVERIFY(study.weight() == 0);
    QVERIFY(study.heightIn() == 0);
    QVERIFY(study.weightLbs() == 0);
    QVERIFY(study.bsa() == 0);
    QVERIFY(study.bsaManualEdit() == false);
    QVERIFY(study.dateTime().date() == QDateTime::currentDateTime().date());
}
    

void TestEpSimulator::testStudiesEqual(const Study& s1, const Study& s2) {
    testStudiesSimilar(s1, s2);
    QCOMPARE(s1.dateTime(), s2.dateTime());  
}

// won't test date or time
void TestEpSimulator::testStudiesSimilar(const Study& s1, const Study& s2) {
    QCOMPARE(s1.name().first(), s2.name().first());
    QCOMPARE(s1.name().middle(), s2.name().middle());
    QCOMPARE(s1.name().last(), s2.name().last());
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
    p->on_weightLbsLineEdit_textEdited();
    p->on_heightInLineEdit_textEdited();
}

QTEST_MAIN(TestEpSimulator)
//#include "testepsimulator.moc"


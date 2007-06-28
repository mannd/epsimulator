/***************************************************************************
 *   Copyright (C) 2006 by EP Studios, Inc.                                *
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

// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include "epsimdefs.h"
#include "catalog.h"
#include "error.h"
#include "filtercatalog.h"
#include "study.h"
#include "heart.h"
#include "options.h"
#include "opticaldisk.h"
#include "patientdialog.h"
#include "passworddialog.h"
#include "passwordhandler.h"
#include "catalogcombobox.h"
#include "user.h"

#include <qbuttongroup.h>
#include <qdatetimeedit.h>
#include <qdir.h>
#include <qfile.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qregexp.h>


#include <iostream>
#include <math.h>
        

using std::cerr;
using std::cout;
using std::endl;

class MyTestSuite : public CxxTest::TestSuite {
public:
    void testStudyConstructor(void) {
        Study study;
        testStudyDefaults(study);
    }
    void testStudyCopyConstructor(void) {
        Study study1;
        Study study2(study1);
        testStudyDefaults(study2);
    }
    void testStudyOperatorEqual() {
        Study study1;
        Study study2 = study1;
        testStudyDefaults(study2);
        testStudiesEqual(study1, study2);   
    }
    void testStudiesSimilar() {
        Study s1, s2;
        testStudiesSimilar(s1, s2);
    }
    void testFullName1() {
        Study s;
        Name name;
        name.first = "David";
        name.middle = "E.";
        name.last = "Mann";
        s.setName(name);
        TS_ASSERT(s.name().fullName() == "David Mann");
        TS_ASSERT(s.name().fullName(true) == "Mann, David");
        TS_ASSERT(s.name().fullName(false, true) == "David E. Mann");
        TS_ASSERT(s.name().fullName(true, true) == "Mann, David E.");
        showFullNameOutput(s);
    }
    // make sure extra spaces are trimmed off
    void testFullName2() {
        Study s;
        Name name;
        name.first = "         David          ";
        name.middle = "   E.     ";
        name.last = "     Mann          ";
        s.setName(name);
        TS_ASSERT(s.name().fullName() == "David Mann");
        TS_ASSERT(s.name().fullName(true) == "Mann, David");
        TS_ASSERT(s.name().fullName(false, true) == "David E. Mann");
        TS_ASSERT(s.name().fullName(true, true) == "Mann, David E.");
        showFullNameOutput(s);
    }
        
    void testFilePath() {
        Study s;
        s.setPath("/home/");
        TS_ASSERT(s.filePath() == "/home/study.dat");
        cerr <<  "filePath() = " << s.filePath();
        s.setPath("");
        TS_ASSERT(s.filePath() == "/study.dat");
        TS_ASSERT(s.filePath() == "/" + s.studyFileName());
    }

    void testKey() {
        Study s;
        // below won't work in debug mode as there is an assert in Study
        // that intercepts this
        // TS_ASSERT(s.key() == QString::null);
        Name name;
        name.first = "John";
        name.last = "Doe";
        s.setName(name);
        cerr <<  "testKey() name.first = " << name.first;
        TS_ASSERT(s.key() == "Doe_John_" + 
            s.dateTime().toString("ddMMyyyyhhmmsszzz"));
}

// make sure white space is removed
    void testKey2() {
        Study s;
        // see testKey() above.  This won't work anymore
        // TS_ASSERT(s.key() == QString::null);
        Name name;
        name.first = "    John  ";
        name.last = "   Doe        ";
        s.setName(name);
        cerr <<   "testKey2 result is " << s.key();
        TS_ASSERT(s.key() == "Doe_John_" + 
            s.dateTime().toString("ddMMyyyyhhmmsszzz"));
}
    
    void testOptions() {
        Options* options = Options::instance();
        cerr <<   options->opticalStudyPath() << endl
            << options->systemCatalogPath() << endl
            << options->networkStudyPath() << endl
            << options->tempStudyPath();
        QString s = options->opticalStudyPath();
        options->setOpticalStudyPath(s);
        TS_ASSERT(s == options->opticalStudyPath());
    }

    void testPatientDialog() {
        Study* s1 = new Study;
        Study* s2 = new Study;
        Name n;
        n.last = "Test3";
        n.first = "John";
        s1->setName(n);
        PatientDialog* p = new PatientDialog;
        p->setFields(s1);
        p->getFields(s2);
        cerr <<   "In testPatientDialog";
        TS_ASSERT(s2->name().last == "Test3");
        TS_ASSERT(s2->name().first == "John");
        delete p;
        delete s1;
        delete s2;
    }
        
    void testPatientDialogCalculations() {
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
        cerr <<   s->bsa();
        double delta = 0.001;
        TS_ASSERT_DELTA(s->height(), heightIn * 2.54, delta);
        TS_ASSERT_DELTA(s->weight(), weightLbs * 0.45, delta);
        TS_ASSERT_DELTA(s->bsa(), 
                        sqrt(s->height() * s->weight()) / 3600,
                        delta);
        // Now test manually setting bsa.
        double bsa = 12345.1;
        s->setBsa(bsa);
        s->setBsaManualEdit(true);
        p->setFields(s);
        completePatientDialog(p);
        p->getFields(s);
        TS_ASSERT_DELTA(s->bsa(), bsa, delta);
        delete p;
        delete s;
    }

    void testFilterCatalog() {
        FilterCatalog* filterCatalog = new FilterCatalog;
      	QRegExp lastNameRegExp(filterCatalog->lastNameLineEdit->text().isEmpty()
            ? "*" : filterCatalog->lastNameLineEdit->text(), false, true);
	QRegExp firstNameRegExp(filterCatalog->firstNameLineEdit->text().isEmpty()
            ? "*" : filterCatalog->firstNameLineEdit->text(), false, true);
	QRegExp mrnRegExp(filterCatalog->mrnLineEdit->text().isEmpty()
            ? "*" : filterCatalog->mrnLineEdit->text(), false, true);
	QRegExp studyConfigRegExp(
            filterCatalog->studyConfigLineEdit->text().isEmpty()
            ? "*" :filterCatalog->studyConfigLineEdit->text(), false, true);
	QRegExp studyNumberRegExp(
            filterCatalog->studyNumberLineEdit->text().isEmpty()
            ? "*" : filterCatalog->studyNumberLineEdit->text(), false, true);
	QRegExp studyFileRegExp(filterCatalog->studyLocationLineEdit->
            text().isEmpty() ? "*" : filterCatalog->
            studyLocationLineEdit->text(), false, true);
	// date stuff next
	QDate today = QDate::currentDate();
	QDate startDate = today, endDate = today;
	bool anyDate = false;
	switch (filterCatalog->studyDateButtonGroup->selectedId()) {
	    case 0 : 
		anyDate = true;
		break;

	    case 1 : // today, default settings are true
		break;

	    case 2 : 
		startDate = endDate.addDays(-7);
		break; // i.e. last week's studies

	    case 3 :   // specific dates selected
		startDate = filterCatalog->beginDateEdit->date();
		endDate = filterCatalog->endDateEdit->date();
		break;
	}
        TS_ASSERT(lastNameRegExp.pattern() == "*");
        TS_ASSERT(firstNameRegExp.pattern() == "*");
        TS_ASSERT(mrnRegExp.pattern() == "*");
        TS_ASSERT(studyConfigRegExp.pattern() == "*");
        TS_ASSERT(studyNumberRegExp.pattern() == "*");
        TS_ASSERT(studyFileRegExp.pattern() == "*");
        TS_ASSERT(anyDate);
        TS_ASSERT(startDate == endDate);
        TS_ASSERT(startDate == QDate::currentDate());
        cerr <<   "Selected ID = " << filterCatalog->studyDateButtonGroup->selectedId();
        delete filterCatalog;
}


   void testGetSetPatientDialogDefaultStudies() {
        PatientDialog pd;
        Study *s1 = new Study;
        Study *s2 = new Study;
        Name n = {"Doe", "John", "E"};
        s1->setName(n);
        pd.setFields(s1);
        pd.getFields(s2);
        TS_ASSERT(s1->key() != s2->key());
        cerr <<   "s1->key()=" << s1->key() << endl
            << "s2->key()=" << s2->key();
        delete s1;
        delete s2;
   }


    void testCatalogComboBox() {
        CatalogComboBox* c = new CatalogComboBox;
        Options* o = Options::instance();
        bool originalEnableNetwork = o->enableNetworkStorage();
        o->setEnableNetworkStorage(false);
        // must refresh catalogcombobox after system options changed!
        c->refresh();
        TS_ASSERT(c->source() == Catalog::System);
        TS_ASSERT(c->currentItem() == 0);   // there should be no Network
        c->setSource(Catalog::Network);  // should not work because of above
        TS_ASSERT(!o->enableNetworkStorage());
        TS_ASSERT(c->currentItem() == 0);
        TS_ASSERT(c->source() != Catalog::Network);
        // should be System
        o->setEnableNetworkStorage(true);
        c->refresh();
        TS_ASSERT(c->source() == Catalog::System);
        c->setSource(Catalog::Network);
        TS_ASSERT(c->source() == Catalog::Network);
        c->setSource(Catalog::Optical);
        TS_ASSERT(c->source() == Catalog::Optical);
        c->setSource(Catalog::Other);
        TS_ASSERT(c->source() == Catalog::Other);
        c->setSource(Catalog::System);
        TS_ASSERT(c->source() == Catalog::System);
        // reset options
        o->setEnableNetworkStorage(originalEnableNetwork);
        delete c;
    }
    
    void testCatalog() {
        Catalog c("../System", "catalog.dat");
        TS_ASSERT(c.filePath() == "../System/catalog.dat");
        c.setPath("/testpath/");
        // make sure no duplicate backslashes
        TS_ASSERT(c.filePath() == "/testpath/catalog.dat");
        // test Catalog subclasses
        Catalog* cp = new OpticalCatalog("../System", "catalog.dat");
        cerr <<   "catalog filepath" << cp->filePath();
        delete cp;
    }
    
    void testCatalogs() {
        Options* o = Options::instance();
        Catalogs* c1 = new Catalogs(o, "");
        c1->setCatalogPath(Catalog::Other, "/tmp/test");
        c1->setCurrentCatalog(Catalog::Other);
        TS_ASSERT(c1->currentCatalog()->path() == "/tmp/test");
        delete c1;
    }
    
    void testPasswordDialog() {
        Options* o = Options::instance();
        PasswordDialog* d = new PasswordDialog(o);
        // test consistency, will fail if line edit is giving gibberish
        bool result1 = d->testPassword();
        delete d;
        PasswordDialog* d1 = new PasswordDialog(o);
        bool result2 = d1->testPassword();
        TS_ASSERT(result1 == result2);
        // another consistency test
        QString pw = "abcdefghi";
        d1->setPassword(pw);
        TS_ASSERT(!d1->testPassword());
        // different password
        d1->setPassword("nonsense");
        TS_ASSERT(!d1->testPassword());
        // need way to set password to do more testing
        
        delete d1;
    }
    
    void testPasswordHandler() {
        Options* o = Options::instance();
        PasswordHandler* ph = new PasswordHandler(o);
        QString s = "password1";
        ph->setPassword(s);
        TS_ASSERT(ph->testPassword(s));
        // tests for non-zero password hash    
        unsigned int hash = o->passwordHash().toUInt(); 
        TS_ASSERT(hash != 0);
        // restore blank password
        ph->setPassword("");
        TS_ASSERT(ph->testPassword(""));
        // blank password results in hash of 0
        hash = o->passwordHash().toUInt();
        TS_ASSERT(hash == 0);
        delete ph;
    }
    
// Below doesn't throw any more, just leaves catalog empty
    void testIoError() {
        // garbage, nonexistent catalog path and file
        TS_ASSERT_THROWS_NOTHING(Catalog c("/djdkfdjdkfjdk/ekekeke/", "dkjkfjekjee"));
        Catalog c1("/djdkfdjdkfjdk/ekekeke/", "dkjkfjekjee");
        // garbage catalog path should give empty catalog
        TS_ASSERT(c1.isEmpty());
        cerr <<   "finished testIoError" << endl;
        // should throw exception
    }
//     
    void testCatalogAddStudy() {
        Options* o = Options::instance();
        cerr << "o->opticalStudyPath() = " << o->opticalStudyPath() << endl;
        Catalogs* cats = new Catalogs(o, o->opticalStudyPath());
        Study* s = new Study;
        Name n = {"Doe", "John", "E"};
        s->setName(n);
        // key will be null unless there is a last name
        QString key = s->key();
        cerr <<   "s->key()=" << s->key() << endl;
        cats->addStudy(s);
        Study s1 = (*cats->currentCatalog())[key];
        TS_ASSERT(s1.key() == key);
        cats->deleteStudy(s);
        delete s;
        delete cats;
    }
    
    void testStudyFileName() {
    	Study s;
    	TS_ASSERT(s.studyFileName() == "study.dat");
        s.setPath("garbage");
        Name n = {"Smith", "John", ""};
        s.setName(n);
        cerr <<   "s.filePath() = " << s.filePath();
        TS_ASSERT(s.filePath() == "garbage/study.dat");
        
    }
    
    void testEmulatedOpticalDisk() {
    	Options* o = Options::instance();
    	EmulatedOpticalDisk d(o->opticalStudyPath());
    	cerr <<   d.path();
    	TS_ASSERT(d.path() == QDir::cleanDirPath(o->opticalStudyPath() + "/disks/"
    		+ d.diskName() + "/" + d.side()));
        TS_ASSERT(d.filePath() == QDir::cleanDirPath( o->opticalStudyPath() + "//disks/"
    		+ d.diskName() + "/" + d.side() + "/label.dat"));
        cerr <<   "optical disk assertion " << o->opticalStudyPath() + "/disks/"
    		+ d.diskName() + "/" + d.side() + "/label.dat" << endl;
        cerr <<   "d.diskName() = " << d.diskName() << endl;
        cerr <<   "emulatedOpticalDisk filePath() = " << d.filePath() << endl;
    }
    
    void testOpticalDisk() {
    	Options* o = Options::instance();
    	OpticalDisk* d = new OpticalDisk(o->opticalStudyPath());
    	TS_ASSERT(d->path() == o->opticalStudyPath());
        TS_ASSERT(d->filePath() == d->path() + "/label.dat");
    	delete d;
    }
    
    void testTranslatedSided() {
        Options* o = Options::instance();
        OpticalDisk* d = new OpticalDisk(o->opticalStudyPath());
        d->setSide("A");
        TS_ASSERT(d->translatedSide() == QObject::tr("A"));
        d->setSide("B");
        TS_ASSERT(d->translatedSide() == QObject::tr("B"));
        delete d;
        d = new EmulatedOpticalDisk(o->opticalStudyPath());
        // make sure default side is A
        TS_ASSERT(d->translatedSide() == QObject::tr("A"));
        delete d;
    }	

    void testUser() {
        User* u = User::instance();
        cerr <<   "Machine name is " << u->machineName() << endl;
        TS_ASSERT (!u->machineName().isNull());
        TS_ASSERT (u->role() == QObject::tr("EPSIMUSER"));
        u->makeAdministrator(true);
        TS_ASSERT(u->role() == QObject::tr("ADMINISTRATOR"));
        TS_ASSERT(u->name() == QObject::tr("ADMINISTRATOR"));
        u->makeAdministrator(false);
        TS_ASSERT(u->role() == QObject::tr("EPSIMUSER"));
        cerr <<   "User name is " << u->name();
    }



private:
    void testStudyDefaults(Study& study) {
        TS_ASSERT(study.sex() == Male);
        TS_ASSERT(study.vagalTone() == DEFAULT_VAGAL_TONE);
        TS_ASSERT(study.sympatheticTone() == DEFAULT_SYMPATHETIC_TONE);
        TS_ASSERT(study.ef() == DEFAULT_EF);
        TS_ASSERT(study.ischemia() == false);
        TS_ASSERT(study.dateOfBirth() == DEFAULT_BIRTH_DATE);
        TS_ASSERT(study.height() == study.weight() 
            == study.heightIn() == study.weightLbs() 
            == study.bsa() == 0);
        TS_ASSERT(study.bsaManualEdit() == false);
        TS_ASSERT(study.dateTime().date() == QDateTime::currentDateTime().date());
        TS_ASSERT(study.file() == "");
//        TS_ASSERT(study.heart()->name() == DEFAULT_HEART);
    }
    // this won't test date or time
    void testStudiesSimilar(Study& s1, Study& s2) {
        TS_ASSERT_EQUALS(s1.name().first, s2.name().first);
        TS_ASSERT_EQUALS(s1.name().middle, s2.name().middle);
        TS_ASSERT_EQUALS(s1.name().last, s2.name().last);
        TS_ASSERT_EQUALS(s1.mrn(), s2.mrn());
        TS_ASSERT_EQUALS(s1.dateOfBirth(), s2.dateOfBirth());
        TS_ASSERT_EQUALS(s1.sex(), s2.sex());
        TS_ASSERT_EQUALS(s1.height(), s2.height());
        TS_ASSERT_EQUALS(s1.weight(), s2.weight());
        TS_ASSERT_EQUALS(s1.bsa(), s2.bsa());
        TS_ASSERT_EQUALS(s1.bsaManualEdit(), s2.bsaManualEdit());
        TS_ASSERT_EQUALS(s1.ef(), s2.ef());
        TS_ASSERT_EQUALS(s1.ischemia(), s2.ischemia());
        TS_ASSERT_EQUALS(s1.vagalTone(), s2.vagalTone());
        TS_ASSERT_EQUALS(s1.sympatheticTone(), s2.sympatheticTone());
        //TS_ASSERT_EQUALS(s1.date(), s2.date());  // there is a slight chance 
        // this will fail around midnight!
        //TS_ASSERT_EQUALS(s1.time(), s2.time());  // this will fail for 2 different
        // studies 
        TS_ASSERT_EQUALS(s1.number(), s2.number());
        TS_ASSERT_EQUALS(s1.file(), s2.file());
        TS_ASSERT_EQUALS(s1.location(), s2.location());
    }
    void testStudiesEqual(Study& s1, Study& s2) {
        TS_ASSERT_EQUALS(s1.name().first, s2.name().first);
        TS_ASSERT_EQUALS(s1.name().middle, s2.name().middle);
        TS_ASSERT_EQUALS(s1.name().last, s2.name().last);
        TS_ASSERT_EQUALS(s1.mrn(), s2.mrn());
        TS_ASSERT_EQUALS(s1.dateOfBirth(), s2.dateOfBirth());
        TS_ASSERT_EQUALS(s1.sex(), s2.sex());
        TS_ASSERT_EQUALS(s1.height(), s2.height());
        TS_ASSERT_EQUALS(s1.weight(), s2.weight());
        TS_ASSERT_EQUALS(s1.bsa(), s2.bsa());
        TS_ASSERT_EQUALS(s1.bsaManualEdit(), s2.bsaManualEdit());
        TS_ASSERT_EQUALS(s1.ef(), s2.ef());
        TS_ASSERT_EQUALS(s1.ischemia(), s2.ischemia());
        TS_ASSERT_EQUALS(s1.vagalTone(), s2.vagalTone());
        TS_ASSERT_EQUALS(s1.sympatheticTone(), s2.sympatheticTone());
        TS_ASSERT_EQUALS(s1.dateTime(), s2.dateTime());  
        TS_ASSERT_EQUALS(s1.number(), s2.number());
        TS_ASSERT_EQUALS(s1.file(), s2.file());
        TS_ASSERT_EQUALS(s1.path(), s2.path());
        TS_ASSERT_EQUALS(s1.fileName(), s2.fileName());
        TS_ASSERT_EQUALS(s1.location(), s2.location());
    }

    void showFullNameOutput(const Study& s) {
        cerr <<   "fullName() = " << s.name().fullName();
        cerr <<   "fullName(true) = " << s.name().fullName(true);
        cerr <<   "fullName(false, true) = " << s.name().fullName(false, true);
        cerr <<   "fullName(true, true) = " << s.name().fullName(true, true) ;
    }

    void completePatientDialog(PatientDialog* p) {
        p->weightKgLineEdit_lostFocus();
        p->weightLbsLineEdit_lostFocus();
        p->heightCmLineEdit_lostFocus();
        p->heightInLineEdit_lostFocus();
    }
};



/*date_(QDate::currentDate()),
    time_(QTime::currentTime()), dateOfBirth_(1950, 1, 1),
    sex_(FEMALE), height_(0), weight_(0), 
    heightIn_(0), weightLbs_(0), bsa_(0), 
    bsaManualEdit_(false), vagalTone_(DEFAULT_VAGAL_TONE),
    sympatheticTone_(DEFAULT_SYMPATHETIC_TONE), ef_(DEFAULT_EF), 
    ischemia_(false), heart_(0) {
    heart_ = new Heart;
    testInvariant();
}

*/

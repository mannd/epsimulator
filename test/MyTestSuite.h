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
#include "filtercatalog.h"
#include "study.h"
#include "heart.h"
#include "options.h"
#include "patientdialog.h"

#include <qbuttongroup.h>
#include <qdatetimeedit.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qregexp.h>


#include <iostream>
#include <math.h>

using std::cout;

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
        s.setFile("/epfiles");
        TS_ASSERT(s.filePath() == "/home/epfiles");
        cout << "filePath() = " << s.filePath() << '\n';
        s.setPath("/home");
        TS_ASSERT(s.filePath() == "/home/epfiles");
        s.setFile("epfiles");
        TS_ASSERT(s.filePath() == "/home/epfiles");
        s.setPath("home");
        TS_ASSERT(s.filePath() == "home/epfiles");
        s.setPath("");
        s.setFile("epfiles");
        TS_ASSERT(s.filePath() == "/epfiles");
    }

    void testKey() {
        Study s;
        TS_ASSERT(s.key() == QString::null);
        Name name;
        name.first = "John";
        name.last = "Doe";
        s.setName(name);
        TS_ASSERT(s.key() == "Doe_John." + s.dateTime().toString("dd.MM.yyyy.hh.mm.ss"));
}

// make sure white space is removed
    void testKey2() {
        Study s;
        TS_ASSERT(s.key() == QString::null);
        Name name;
        name.first = "    John  ";
        name.last = "   Doe        ";
        s.setName(name);
        cout <<  "testKey2 result is " << s.key() << "\n";
        TS_ASSERT(s.key() == "Doe_John." + s.dateTime().toString("dd.MM.yyyy.hh.mm.ss"));
}
    
    void testOptions() {
        Options* options = Options::instance();
        cout << options->studyPath() << '\n';
        QString s = options->studyPath();
        options->setStudyPath(s);
        TS_ASSERT(s == options->studyPath());
        delete options;
    }

    void testPatientDialog() {
        Study s1, s2;
        Name n;
        n.last = "Test3";
        n.first = "John";
        s1.setName(n);
        PatientDialog* p = new PatientDialog;
        p->setFields(s1);
        p->getFields(s2);
        TS_ASSERT(s2.name().last == "Test3");
        TS_ASSERT(s2.name().first == "John");
        delete p;
    }
        
    void testPatientDialogCalculations() {
        Study s;
        double heightIn = 72, weightLbs = 143;
        s.setHeightIn(heightIn);
        s.setWeightLbs(weightLbs);
        PatientDialog* p = new PatientDialog;
        p->setFields(s);
        // Below should do all the calculations by faking
        // tabbing through the fields.
        completePatientDialog(p);
        p->getFields(s);
        cout << s.bsa();
        double delta = 0.001;
        TS_ASSERT_DELTA(s.height(), heightIn * 2.54, delta);
        TS_ASSERT_DELTA(s.weight(), weightLbs * 0.45, delta);
        TS_ASSERT_DELTA(s.bsa(), 
                        sqrt(s.height() * s.weight()) / 3600,
                        delta);
        // Now test manually setting bsa.
        double bsa = 12345.1;
        s.setBsa(bsa);
        s.setBsaManualEdit(true);
        p->setFields(s);
        completePatientDialog(p);
        p->getFields(s);
        TS_ASSERT_DELTA(s.bsa(), bsa, delta);
        delete p;
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
	QRegExp studyFileRegExp(filterCatalog->studyFileLineEdit->text().isEmpty()
            ? "*" : filterCatalog->studyFileLineEdit->text(), false, true);
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
        cout << "Selected ID = " << filterCatalog->studyDateButtonGroup->selectedId() << std::endl;
        delete filterCatalog;
}

//    void testGetSetPatientDialogDefaultStudies() {
//        PatientDialog pd;
//        Study s1, s2;
//        pd.setFields(s1);
//        pd.getFields(s2);
//        testStudiesEqual(s1, s2);
//    }

private:
    void testStudyDefaults(Study& study) {
        TS_ASSERT(study.sex() == Male);
        cout << (int)study.sex() << '\n';;
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
    }

    void showFullNameOutput(const Study& s) {
        cout << "fullName() = " << s.name().fullName() << '\n';
        cout << "fullName(true) = " << s.name().fullName(true) << '\n';
        cout << "fullName(false, true) = " << s.name().fullName(false, true) << '\n';
        cout << "fullName(true, true) = " << s.name().fullName(true, true)  << '\n';
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

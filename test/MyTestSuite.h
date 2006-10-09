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
#include "../src/study.h"
#include "../src/heart.h"
#include "../src/patientdialog.h"

#include <qstring.h>
#include <qdatetime.h>

#include <iostream>
using namespace std;

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
        cout <<s.name().fullName() << '\n';
        cout <<s.name().fullName(true) << '\n';
        cout <<s.name().fullName(false, true) << '\n';
        cout <<s.name().fullName(true, true)  << '\n';
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
        cout <<s.name().fullName() << '\n';
        cout <<s.name().fullName(true) << '\n';
        cout <<s.name().fullName(false, true) << '\n';
        cout <<s.name().fullName(true, true)  << '\n';
    }
        
    void testFilePath() {
        Study s;
        s.setPath("/home/");
        s.setFile("/epfiles");
        TS_ASSERT(s.filePath() == "/home/epfiles");
        cout << s.filePath();
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

        

//    void testGetSetPatientDialogDefaultStudies() {
//        PatientDialog pd;
//        Study s1, s2;
//        pd.setFields(s1);
//        pd.getFields(s2);
//        testStudiesEqual(s1, s2);
//    }

private:
    void testStudyDefaults(Study& study) {
        TS_ASSERT(study.sex() == FEMALE);
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

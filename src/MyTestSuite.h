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
#include "study.h"

class MyTestSuite : public CxxTest::TestSuite {
public:
    void testAddition( void ) {

        TS_ASSERT( 1 + 1 > 1 );

        TS_ASSERT_EQUALS( 1 + 1, 2 );
    }
    void testStudyConstructor(void) {
        Study study;
        TS_ASSERT(study.sex() == FEMALE);
        TS_ASSERT(study.vagalTone() == DEFAULT_VAGAL_TONE);
        TS_ASSERT(study.sympatheticTone() == DEFAULT_SYMPATHETIC_TONE);
        TS_ASSERT(study.ef() == DEFAULT_EF);
        TS_ASSERT(study.ischemia() == false);
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
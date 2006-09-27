/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "MyTestSuite.h"

static MyTestSuite suite_MyTestSuite;

static CxxTest::List Tests_MyTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_MyTestSuite( "MyTestSuite.h", 23, "MyTestSuite", suite_MyTestSuite, Tests_MyTestSuite );

static class TestDescription_MyTestSuite_testAddition : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testAddition() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 25, "testAddition" ) {}
 void runTest() { suite_MyTestSuite.testAddition(); }
} testDescription_MyTestSuite_testAddition;

#include "navigatortest.h"

static NavigatorTest suite_NavigatorTest;

static CxxTest::List Tests_NavigatorTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_NavigatorTest( "navigatortest.h", 23, "NavigatorTest", suite_NavigatorTest, Tests_NavigatorTest );

static class TestDescription_NavigatorTest_testNavigator : public CxxTest::RealTestDescription {
public:
 TestDescription_NavigatorTest_testNavigator() : CxxTest::RealTestDescription( Tests_NavigatorTest, suiteDescription_NavigatorTest, 25, "testNavigator" ) {}
 void runTest() { suite_NavigatorTest.testNavigator(); }
} testDescription_NavigatorTest_testNavigator;

#include <cxxtest/Root.cpp>

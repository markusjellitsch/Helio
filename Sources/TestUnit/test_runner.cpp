//------------------------------------------------------------------------
// NAME         : test_runner.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : test runner class to execute test_unit
// -----------------------------------------------------------------------

#include "test_runner.h"
#include <Logger/logger.h>
#include "ctime"
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>


using namespace std;


// --------------------------------
// add a test to the test list
// --------------------------------
int TestRunner::addTest(TestUnit * test,string const & testName){
    if (test == nullptr){
        return TEST_ERROR_NULLPTR;
    }

    // set default logger
    test->setLogger(mLogger);

    // set test name
    test->setTestName(testName);

    // add test
    mTestList.push_back(test);

    return TEST_OK;
}

// --------------------------------
// run a specific test
// --------------------------------
int TestRunner::runTest(unsigned int const testNr){

    unsigned int len = mTestList.size();

    if (testNr >= len) return -1;

    int success = -1;

    if (mTestList[testNr] != nullptr) {

        // setup test
        success = mTestList[testNr]->setup();
        TESTRUNASSERT(success);

        // run test
        success = mTestList[testNr]->runTest();
        TESTRUNASSERT(success);

        // teard down test
        success = mTestList[testNr]->tearDown();
        TESTRUNASSERT(success);

        log(TEST_SEVERITY_NOTE,"Test completed and succesful!");
        return TEST_OK;
    }

    return -1;
}

// --------------------------------
// run all test in test list
// --------------------------------
int TestRunner::runAllTests(){

    int success = -1;

    for (unsigned int i = 0; i<mTestList.size();i++){
        success = runTest(i);
    }

    return success;
}



// -----------------------------------
// set the logger for logging info
// -----------------------------------
int TestRunner::setLogger(Logger * logger){

    mLogger = logger;

   return TEST_OK;
}

// -----------------------------------
// log error
// -----------------------------------
int TestRunner::log(uint8_t const logLevel, std::string const & text){

    if (mLogger == 0) return TEST_ERROR_NULLPTR;

    stringstream stream;
    stream << "Test Runner" << ": " << text;

    switch (logLevel){

    case TEST_SEVERITY_ERROR:
        mLogger->Error(stream.str());
        break;

    case TEST_SEVERITY_NOTE:
        mLogger->Note(stream.str());
        break;
    }

    return TEST_OK;
}


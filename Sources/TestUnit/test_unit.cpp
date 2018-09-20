//------------------------------------------------------------------------
// NAME         : test_unit.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : test unit base class
// -----------------------------------------------------------------------

#include "test_unit.h"
#include "ctime"
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>



using namespace std;

// -----------------------------------
// constructor
// ------------------------------------
 TestUnit::TestUnit(){
    mTestName = "Unknown Test Name";
    mLogger = nullptr;
 }

 // -----------------------------------
 // destructor
 // ------------------------------------
TestUnit::~TestUnit(){
     //todo: destroy singelton instance
 }


// -----------------------------------
// initializing the test environment
// ------------------------------------
int TestUnit::setup(){

    log(TEST_SEVERITY_NOTE,"Setup Test!");

    // call setup hook
    int success = doSetup();

    // log success
    if (success == TEST_OK){
        log(TEST_SEVERITY_NOTE, "Setup successful!");
    }
    else log(TEST_SEVERITY_ERROR, "Setup error occured!");

    return success;
}

// -----------------------------------
// clean up test environment
// -----------------------------------
int TestUnit::tearDown(){

    log(TEST_SEVERITY_NOTE,"Tear down Test!");

    // call tear down hook
    int success = doTearDown();

    // log success
    if (success == TEST_OK){
        log(TEST_SEVERITY_NOTE, "Tear down successful!");
    }
    else log(TEST_SEVERITY_ERROR, "Tear down error occured!");

    return success;
}

// -----------------------------------
// run test
// -----------------------------------
int TestUnit::runTest(){

    log(TEST_SEVERITY_NOTE,"Run Test!");

    // call run test hook
    int success = doRunTest();

    // log success
    if (success == TEST_OK){
        log(TEST_SEVERITY_NOTE, "Test run successful!");
    }
    else log(TEST_SEVERITY_ERROR, "Test run error occured!");

    return success;
}

// -----------------------------------
// set the logger for logging info
// -----------------------------------
int TestUnit::setLogger(Logger * logger){

    mLogger = logger;

   return TEST_OK;
}

// -----------------------------------
// log error
// -----------------------------------
int TestUnit::log(uint8_t const logLevel, std::string const & text){

    if (mLogger == 0) return TEST_ERROR_NULLPTR;

    stringstream stream;
    stream << mTestName << ": " << text;

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

// -----------------------------------
// set test name
// -----------------------------------
int TestUnit::setTestName(std::string const & name){
        mTestName = name;

        return TEST_OK;
}

// -----------------------------------
// assert handler
// -----------------------------------
int TestUnit::assertHandler(int errorCode,string const & file, int line){

    string text = "ASSERT FAILED IN";
    text.append(file);
    text.append(" IN LINE ");
    text.append(to_string(line));
    text.append(".ERROR CODE:"+ errorCode);
    log(TEST_SEVERITY_ERROR,text);

    return TEST_OK;
}

std::string TestUnit::readStdin(){

    string str= "";

    cin >>str;

    return str;
}

int TestUnit::writeStdOut(std::string const &str,bool newline){

    cout << mTestName << ": " << str;
    if (newline) cout << endl;

    return TEST_OK;
}

int TestUnit::checkYesNo(std::string const & text){

    writeStdOut(text,false);
    cout << " ['y'or 'n'] Your Input:";
    string result = readStdin();
    if (result != "y"){
        return -1;
    }

    return TEST_OK;
}


//------------------------------------------------------------------------
// NAME         : test_unit.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : test unit base class providing neceessary funciton
// -----------------------------------------------------------------------

#include "test_unit.h"
#include "ctime"
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <QSettings>

using namespace std;

// -----------------------------------
// constructor
// ------------------------------------
 TestUnit::TestUnit(){
    memset(&mTestParameters,sizeof(TestParameterList_t),0);
    mTestParameters.valid = 0;
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

    log(TEST_SEVERITY_VERBOSE,"Setup Test!");

    // call setup hook
    int success = doSetup();

    // log success
    if (success == TEST_OK){
        log(TEST_SEVERITY_VERBOSE, "Setup successful!");
    }
    else log(TEST_SEVERITY_ERROR, "Setup error occured!");

    return success;
}

// -----------------------------------
// clean up test environment
// -----------------------------------
int TestUnit::tearDown(){

    log(TEST_SEVERITY_VERBOSE,"Tear down Test!");

    // call tear down hook
    int success = doTearDown();

    // log success
    if (success == TEST_OK){
        log(TEST_SEVERITY_VERBOSE, "Tear down successful!");
    }
    else log(TEST_SEVERITY_ERROR, "Tear down error occured!");

    return success;
}

// -----------------------------------
// run test
// -----------------------------------
int TestUnit::runTest(){

    log(TEST_SEVERITY_VERBOSE,"Run Test!");

    // call run test hook
    int success = doRunTest();

    // log success
    if (success == TEST_OK){
        log(TEST_SEVERITY_VERBOSE, "Test run successful!");
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

    case TEST_SEVERITY_MESSAGE:
        mLogger->Message(stream.str());
        break;

    case TEST_SEVERITY_VERBOSE:
        mLogger->Verbose(stream.str());
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

std::string TestUnit::getTestName(){
    return mTestName;
}

// -----------------------------------
// assert handler
// -----------------------------------
int TestUnit::assertHandler(int errorCode,string const & file, int line){

    string text = "ASSERT FAILED IN";
    text.append(file);
    text.append(" IN LINE ");
    text.append(to_string(line));
    text.append(".ERROR CODE:");
    text.append(to_string(errorCode));
    log(TEST_SEVERITY_ERROR,text);

    return TEST_OK;
}

// -----------------------------------
// Parse the .ini file
// -----------------------------------
int TestUnit::useIniFile(std::string const & fileName){

    // open config file
    QSettings configuration(QString::fromStdString(fileName),QSettings::IniFormat);
    configuration.beginGroup(QString::fromStdString(mTestName));

    QString param = "";

    TestParameterList_t paramList;

    // reset test parameters
    mTestParameters.valid = 0;
    mTestParameters.param1 = 0;
    mTestParameters.param2 = 0;
    mTestParameters.param3 = 0;
    mTestParameters.param4 = 0;
    mTestParameters.param5 = 0;

    // read param 1
    param = configuration.value("param1","undef").toString();
    if (param == "undef"){
        configuration.endGroup();
        return TEST_ERROR_INVALID_CONFIG;
    }
    paramList.param1 = param.toInt();

    // read param 2
    param = configuration.value("param2","undef").toString();
    if (param == "undef"){
        configuration.endGroup();
        return TEST_ERROR_INVALID_CONFIG;
    }
    paramList.param2 = param.toInt();

    // read param 3
    param = configuration.value("param3","undef").toString();
    if (param == "undef"){
        configuration.endGroup();
        return TEST_ERROR_INVALID_CONFIG;
    }
    paramList.param3 = param.toInt();

    // read param 4
    param = configuration.value("param4","undef").toString();
    if (param == "undef"){
        configuration.endGroup();
        return TEST_ERROR_INVALID_CONFIG;
    }
    paramList.param4 = param.toInt();

    // read param 5
    param = configuration.value("param5","undef").toString();
    if (param == "undef"){
        configuration.endGroup();
        return TEST_ERROR_INVALID_CONFIG;
    }
    paramList.param5 = param.toInt();

    // accept parameters
    mTestParameters = paramList;

    // set valid
    mTestParameters.valid = 1;

    configuration.endGroup();

    return TEST_OK;
}

// -----------------------------------
// Read line from stdint
// -----------------------------------
std::string TestUnit::readStdin(){

    string str= "";

    cin >>str;

    return str;
}

// -----------------------------------
// write to stdou
// -----------------------------------
int TestUnit::writeStdOut(std::string const &str,bool newline){

    cout << mTestName << ": " << str;
    if (newline) cout << endl;

    return TEST_OK;
}

// -----------------------------------
// Ask user for input
// -----------------------------------
int TestUnit::checkYesNo(std::string const & text){

    writeStdOut(text,false);
    cout << " ['y'or 'n'] Your Input:";
    string result = readStdin();
    if (result != "y"){
        return -1;
    }

    return TEST_OK;
}


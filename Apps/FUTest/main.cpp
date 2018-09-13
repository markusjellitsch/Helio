// ********************************************************************
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 24.05.2018
// DESCRIPTION  : testing helio test interface (=Heti), a rs485 based interface
// **********************************************************************

#include <iostream>
#include <string>
#include <string.h>
#include <Heti/heti.h>
#include <TestUnit/test_runner.h>
#include <TestUnit/test_fu.h>
#include <Logger/stdlogger.h>

using namespace std;

#define TESTNAME "FU Test"

/*--------------------------------------------------
 * Modbus RTU (RS485) testing
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{

    TestFU fuUnit;
    TestRunner runner;
    StdLogger logger;

    bool logging = false;

    // enable logging level0
    if (argc ==2){
        if (strcmp(argv[1],"-v0")==0){
            logging = true;
        }
    // enable logging level1 (+more specific)
    else if (strcmp(argv[1],"-v1")==0){
            logging = true;
            HETI::GetInstance()->setLogger(&logger);
        }
    }

    logger.setLoggingOption(logging,logging,logging);
    logger.setLogName("Test Runner Logger");

    // set the logger for the runner
    runner.setLogger(&logger);

    // add the test
    runner.addTest(&fuUnit,TESTNAME);

    // run all testst
    int success = runner.runAllTests();

    if (success == TESTRUNNER_OK){
       cout <<  TESTNAME << ": OK!" << endl;
    }
    else cout << TESTNAME << ": NOK!" << endl;


    return 0;
}

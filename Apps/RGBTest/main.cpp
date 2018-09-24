// ********************************************************************
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 24.05.2018
// DESCRIPTION  : testing helio test interface (=Heti), a rs485 based interface
// **********************************************************************

#include <iostream>
#include <string>
#include <string.h>
#include <helio.h>

using namespace std;

#define TESTNAME "RGB Test"

/*--------------------------------------------------
 * Modbus RTU (RS485) testing
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{

    TestRGB rgbUnit;
    TestRunner runner;
    StdLogger logger;

    bool verbose = false;

    // enable logging level0
    if (argc ==2){
        if (strcmp(argv[1],"-v0")==0){
            verbose = true;
        }
    // enable logging level1 (+more specific)
    else if (strcmp(argv[1],"-v1")==0){
            verbose = true;
            HETI::getInstance()->setLogger(&logger);
        }
    }

    logger.setLoggingOption(true,verbose,true);
    logger.setLogName("Test Runner Logger");

    // set the logger for the runner
    runner.setLogger(&logger);

    // add the test
    runner.addTest(&rgbUnit,TESTNAME);

    int success = 0;

    success = runner.runAllTests();

    cout << "Finished!" << endl;
    return 0;
}

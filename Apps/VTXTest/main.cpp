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

#define TESTNAME "VTX Test"

/*--------------------------------------------------
 * Modbus RTU (RS485) testing
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{

    TestVTX vtxUnit;
    TestRunner runner;
    StdLogger logger;

    Console console(argc,argv);
    console.setAppDescription(TESTNAME);
    console.addOption("v0","Verbose Level 0");
    console.addOption("v1","Verbose Level 1");
    console.addOption("config","Set Configuration File");

    // parse argument list
    int success = console.parse();
    if (success < 0){
        console.showOptions();
        return 0;
    }
    bool verbose = false;

    // enable logging level0
    if (console.isOptionEntered("v0")){
        verbose = true;
    }
    // enable logging level1 (+more specific)
    else if (console.isOptionEntered("v1")){
        verbose = true;
        HETI::getInstance()->setLogger(&logger);
    }

    // check .ini file
    if (console.isOptionEntered("config")){
        runner.setConfigFile(console.getOptionValue("config"));
    }

    logger.setLoggingOption(true,verbose,true);
    logger.setLogName("Test Runner Logger");

    // set the logger for the runner
    runner.setLogger(&logger);

    // add the test
    runner.addTest(&vtxUnit,TESTNAME);

    success = 0;

    success = runner.runAllTests();

    cout << "Finished!" << endl;
    return 0;
}

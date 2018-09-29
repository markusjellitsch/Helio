// ********************************************************************
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 24.05.2018
// DESCRIPTION  : testing WebEx. Complete Test
// **********************************************************************

#include <iostream>
#include <string>
#include <string.h>
#include <helio.h>

using namespace std;

#define TESTNAME "WebEx Test"

/*--------------------------------------------------
 * Testing WebEx
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{

    // set application options
    Console console(argc,argv);
    console.setAppDescription(TESTNAME);
    console.addOption("v0","Verbose Level 0");
    console.addOption("v1","Verbose Level 1");
    console.addOption("config","Test configuration file (.ini)");
    console.addOption("test","Select a test (1,2,3 or 4");
    console.addOption("help","Show possible options");

    // test units
    TestPWM pwmUnit;
    TestRGB rgbUnit;
    TestVTX vtxUnit;
    TestFU fuUnit;

    // test runner & logger
    TestRunner runner;
    StdLogger logger;


    // parse argument list
    int success = console.parse();
    if (success < 0 || console.isOptionEntered("help")){
        console.showOptions();
        return 0;
    }

    // verbose mode
    bool verbose = false;

    // enable verbose level0
    if (console.isOptionEntered("v0")){
        verbose = true;
    }
    // enable verbose level1 (+more specific)
    else if (console.isOptionEntered("v1")){
        verbose = true;
        HETI::getInstance()->setLogger(&logger);
        RTU::getInstance()->setLogger(&logger);
    }

    if (console.isOptionEntered("config")){
        runner.setConfigFile(console.getOptionValue("config"));
    }

    // set the logging options
    logger.setLoggingOption(true,verbose,true);
    logger.setLogName("WebEx Test Info");

    // set the logger for the runner
    runner.setLogger(&logger);

    // add the test
    runner.addTest(&fuUnit,"FU Test");
    runner.addTest(&rgbUnit,"RGB Test");
    runner.addTest(&vtxUnit,"VTX Test");
    runner.addTest(&pwmUnit,"PWM Test");

    success = 0;

    // run the test
    if (console.isOptionEntered("test")){
        int testNr = atoi(console.getOptionValue("test").c_str());
        cout << "Test Nr:"<< dec << testNr << endl;
        success = runner.runTest((uint8_t)testNr-1);
    }
    else success = runner.runAllTests();

    cout << "Test Runner result:"<< success << endl;
    cout << "Finished!" << endl;

    return 0;
}

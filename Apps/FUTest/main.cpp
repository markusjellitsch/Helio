// ********************************************************************
// NAME         : main.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 24.05.2018
// DESCRIPTION  : testing helio test interface (=Heti), a rs485 based interface
// **********************************************************************

#include <iostream>
#include <string>
#include <Heti/heti.h>
#include <TestUnit/test_runner.h>
#include <TestUnit/test_fu.h>
#include <Logger/stdlogger.h>

using namespace std;

/*--------------------------------------------------
 * Modbus RTU (RS485) testing
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{

    //
    TestFU fuUnit;
    TestRunner runner;
    StdLogger logger;

    logger.setLogName("Test Runner Logger");

    // set the logger for the runner
    runner.setLogger(&logger);

    // add the test
    runner.addTest(&fuUnit,"FU Test");

    // run all testst
    runner.runAllTests();

    return 0;
}

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
#include <cxxopts.hpp>

using namespace std;
using namespace cxxopts;

#define TESTNAME "VTX Test"

/*--------------------------------------------------
 * Modbus RTU (RS485) testing
 * --------------------------------------------------*/
int main(int argc, char *argv[])
{

  Options options("VTX Test","Testing Vortex of WebEx")

  options.add_options()("v,verbose","Enable verbose mode");
  auto result = options.parse(argc,argv);
  if (result.count("verbose")>0){

      cout << result["verbose"].as<string>() << endl;
  }

    TestVTX vtxUnit;
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
    runner.addTest(&vtxUnit,TESTNAME);

    int success = 0;

    success = runner.runAllTests();

    cout << "Finished!" << endl;
    return 0;
}

//------------------------------------------------------------------------
// NAME         : test_runner.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : test runner class to execute test_units
// -----------------------------------------------------------------------

#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <string>
#include <stdint.h>
#include <TestUnit/test_unit.h>
#include <Logger/logger.h>
#include <vector>

#define TESTRUNNER_OK       TEST_OK

#define  TESTRUNASSERT(x)                                                       \
{                                                                               \
    if (x != TESTRUNNER_OK){                                                    \
        log(TEST_SEVERITY_MESSAGE,"Test FAIL");                                \
        return x;                                                               \
    }                                                                           \
}

class TestRunner
{
public:

    TestRunner()=default;
    ~TestRunner()=default;

    // add a test to the test list
    int addTest(TestUnit * test,std::string const & testName);

    // run a specific test
    int runTest(unsigned int const testNr);

    // run all tests
    int runAllTests();

    // set the logger for logging info
    int setLogger(Logger * logger);

    // set the .ini config file to be read by the Test Unit
    int setConfigFile(std::string fileName);

protected:

    // log errors
    int log(uint8_t const logLevel, std::string const & text);

    Logger * mLogger = nullptr;

    std::vector<TestUnit*> mTestList;

    std::string mConfigFileName="";
};

#endif

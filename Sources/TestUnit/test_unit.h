//------------------------------------------------------------------------
// NAME         : test_unit.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : test base class
// -----------------------------------------------------------------------

#ifndef TEST_UNIT_H
#define TEST_UNIT_H

#include <string>
#include <stdint.h>
#include <Logger/logger.h>


#define TEST_OK    0

#define TEST_ERROR_SETUP            -1
#define TEST_ERROR_TEARDOWN         -2
#define TEST_ERROR_RUNTEST          -3
#define TEST_ERROR_NULLPTR          -4
#define TEST_ERROR_INVALID_CONFIG   -5

#define TEST_SEVERITY_ERROR          0
#define TEST_SEVERITY_MESSAGE        1
#define TEST_SEVERITY_VERBOSE        2

#define TEST_DEFAULT_TESTNAME        ("Unknown Test")

// asserting test
#define  TESTASSERT(x,y)                                                         \
{                                                                               \
                                                                                 \
    log(TEST_SEVERITY_VERBOSE,y);                                                   \
    if (x != 0){                                                                \
        assertHandler(x,__FILE__,__LINE__);                                     \
        return x;                                                               \
    }                                                                           \
    log(TEST_SEVERITY_VERBOSE,"Check OK");                                         \
}

// compare test
#define  TESTCOMPARE(x,val)                                                     \
{                                                                               \
    if (x != val){                                                              \
        assertHandler(x,__FILE__,__LINE__);                                     \
        return x;                                                               \
    }                                                                           \
}

// test parameters read from .ini file
struct TestParameterList_t{
    int32_t param1;
    int32_t param2;
    int32_t param3;
    int32_t param4;
    int32_t param5;
    uint8_t valid;
};

class TestUnit
{
public:

    TestUnit();
    virtual ~TestUnit();

    // initializing the test environment
    int setup();

    // clean up test environment
    int tearDown();

    // run test
    int runTest();

    // set the logger for logging info
    int setLogger(Logger * logger);

    // set test names
    int setTestName(std::string const & name);
    std::string getTestName();

    // use .iniFile
    int useIniFile(std::string const & fileName);

protected:

    // log errors
    int log(uint8_t const logLevel, std::string const & text);

    // assert handler
    int assertHandler(int errorCode,std::string const & file, int line);

    std::string readStdin();
    int writeStdOut(std::string const & text,bool newline=true);
    int checkYesNo(std::string const & text);

    // return test parameter list
    TestParameterList_t getTestParameterList();

    // hooks to be implemented
    virtual int doSetup()=0;
    virtual int doTearDown()= 0;
    virtual int doRunTest() = 0;


    // variables
    std::string mTestName;
    Logger * mLogger = nullptr;

    TestParameterList_t mTestParameters;
};

#endif

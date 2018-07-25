#include <QCoreApplication>
#include <test_runner.h>
#include <test_fu.h>
#include <stdlogger.h>

int main(int argc, char *argv[])
{

    // logger
    StdLogger logger("Std Logger");
    logger.setLoggingOption(true,true,true);

    // test runner
    TestRunner testRunner;
    testRunner.setLogger(&logger);

    // add tests here
    TestFU fuTest;
    TestFU fuTest2;

    testRunner.addTest(&fuTest,"FU Test");
    testRunner.addTest(&fuTest2,"FU Test 2");


    testRunner.runAllTests();

    return 0;
}

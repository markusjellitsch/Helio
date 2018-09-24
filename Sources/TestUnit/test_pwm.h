//------------------------------------------------------------------------
// NAME         : test_pwm.h
// AUTHOR       : Markus Jellitsch
// DATE         : 22.09.2018
// DESCRIPTION  : test harness for FU (= Frequenzumrichter)
// -----------------------------------------------------------------------

#ifndef test_pwm_H
#define test_pwm_H

#include <TestUnit/test_unit.h>

class PWMTest : public TestUnit
{
public:

    PWMTest()=default;
    virtual ~PWMTest()=default;

protected:

    // setup hooks to be implemented
    virtual int doSetup();

    // tear down hook to be implemented
    virtual int doTearDown();

    // run test hook to be implemted
    virtual int doRunTest();

 protected:

    // additional variables
};

#endif

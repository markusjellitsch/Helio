//------------------------------------------------------------------------
// NAME         : test_pwm.h
// AUTHOR       : Markus Jellitsch
// DATE         : 22.09.2018
// DESCRIPTION  : test harness for FU (= Frequenzumrichter)
// -----------------------------------------------------------------------

#ifndef TEST_PWM_H
#define TEST_PWM_H

#include <TestUnit/test_unit.h>

class TestPWM : public TestUnit
{
public:

    TestPWM()=default;
    virtual ~TestPWM()=default;

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

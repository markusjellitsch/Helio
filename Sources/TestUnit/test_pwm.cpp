//------------------------------------------------------------------------
// NAME         : test_pwm.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 22.09.2018
// DESCRIPTION  : test harness for PWM of WebEX
// -----------------------------------------------------------------------

#include "test_pwm.h"
#include <helio.h>


using namespace std;

// setup hooks to be implemented
int PWMTest::doSetup(){

    TESTASSERT(RTU::getInstance()->connect(),"Open RTU Instance");
    TESTASSERT(WebEx::getInstance()->connect(),"Conenct to WebEx");

    return TEST_OK;
}

// tear down hook to be implemented
int PWMTest::doTearDown(){

    return TEST_OK;
}

// run test hook to be implemted
int PWMTest::doRunTest(){

    TESTASSERT(RTU::releaseInstance(),"Release RTU Instance");
    TESTASSERT(WebEx::releaseInstance(),"Release WebEx Instance");

    return TEST_OK;
}

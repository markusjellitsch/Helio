//------------------------------------------------------------------------
// NAME         : test_vtx.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 15.09.2018
// DESCRIPTION  : test harness for
// -----------------------------------------------------------------------

#include "test_vtx.h"
#include <helio.h>
#include "unistd.h"

using namespace std;

// setup hooks to be implemented
int TestVTX::doSetup(){

    TESTASSERT(RTU::getInstance()->connect(),"Open RTU Instance");
    TESTASSERT(WebEx::getInstance()->connect(),"Connect to WebEx");

    return TEST_OK;
}

// tear down hook to be implemented
int TestVTX::doTearDown(){

    TESTASSERT(RTU::releaseInstance(),"Release RTU Instance");
    TESTASSERT(WebEx::releaseInstance(),"Release WebEx Instance");
    return TEST_OK;
}

// run test hook to be implemted
int TestVTX::doRunTest(){

    // enable VTX1 and PWM (rtu)
    TESTASSERT(WebEx::getInstance()->startCounter(1,3000),"Start Counter 1");
    usleep(500);
    TESTASSERT(RTU::getInstance()->startPWM(RTU_PWM_CHANNEl1,1000,50),"Start PWM 1");
    usleep(3500*1000);
    TESTASSERT(RTU::getInstance()->stopCapture(RTU_PWM_CHANNEl1),"Stop PWM 1");
    TESTASSERT(WebEx::getInstance()->checkRegister(7,0),"Check Counter stopped");

    usleep(500);

    // enable VTX12 and PWM (rtu)
    TESTASSERT(WebEx::getInstance()->startCounter(2,3000),"Start Counter 2");
    usleep(500);
    TESTASSERT(RTU::getInstance()->startPWM(RTU_PWM_CHANNEl2,1000,50),"Start PWM 2");
    usleep(3500*1000);
    TESTASSERT(RTU::getInstance()->stopCapture(RTU_PWM_CHANNEl2),"Stop PWM 2");
    TESTASSERT(WebEx::getInstance()->checkRegister(7,0),"Check Counter stopped");

    return TEST_OK;
}

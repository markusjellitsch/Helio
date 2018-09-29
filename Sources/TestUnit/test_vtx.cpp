//------------------------------------------------------------------------
// NAME         : test_vtx.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 15.09.2018
// DESCRIPTION  : test harness for
// -----------------------------------------------------------------------

#include "test_vtx.h"
#include <helio.h>
#include "unistd.h"
#include <iostream>
#include <string>

using namespace std;

// setup hooks to be implemented
int TestVTX::doSetup(){

    TESTASSERT(RTU::getInstance()->connect(),"Open RTU Instance");
    TESTASSERT(WebEx::getInstance()->connect(),"Connect to WebEx");

    if (!mTestParameters.valid){
      mTestParameters.param1 = 3000; // cnt vtx1
      mTestParameters.param2 = 3000; // cnt vtx2
      mTestParameters.param3 = 0;    // cnt vt3
      mTestParameters.param4 = 0;
      mTestParameters.param5 = 0;
    }

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

    // -------
    // VTX1
    //---------
    if (mTestParameters.param1){
        TESTASSERT(WebEx::getInstance()->startCounter(1,(uint16_t)mTestParameters.param1),"Start Counter 1");
        usleep(500);
        TESTASSERT(RTU::getInstance()->startPWM(RTU_PWM_CHANNEl1,1000,50),"Start PWM 1");
        usleep(3500*1000);
        TESTASSERT(RTU::getInstance()->stopCapture(RTU_PWM_CHANNEl1),"Stop PWM 1");
        TESTASSERT(WebEx::getInstance()->checkRegister(7,0),"Check Counter stopped");
    }
    usleep(500);

    // -------
    // VTX2
    //---------
    if (mTestParameters.param2){
        TESTASSERT(WebEx::getInstance()->startCounter(2,mTestParameters.param2),"Start Counter 2");
        usleep(500);
        TESTASSERT(RTU::getInstance()->startPWM(RTU_PWM_CHANNEl2,1000,50),"Start PWM 2");
        usleep(3500*1000);
        TESTASSERT(RTU::getInstance()->stopCapture(RTU_PWM_CHANNEl2),"Stop PWM 2");
        TESTASSERT(WebEx::getInstance()->checkRegister(7,0),"Check Counter stopped");
    }

    return TEST_OK;
}

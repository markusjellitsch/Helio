//------------------------------------------------------------------------
// NAME         : test_pwm.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 22.09.2018
// DESCRIPTION  : test harness for PWM of WebEX
// -----------------------------------------------------------------------

#include "test_pwm.h"
#include <helio.h>
#include <unistd.h>


using namespace std;

// setup hooks to be implemented
int TestPWM::doSetup(){

    TESTASSERT(RTU::getInstance()->connect(),"Open RTU Instance");
    TESTASSERT(WebEx::getInstance()->connect(),"Conenct to WebEx");

    return TEST_OK;
}

// tear down hook to be implemented
int TestPWM::doTearDown(){

    TESTASSERT(RTU::releaseInstance(),"Release RTU Instance");
    TESTASSERT(WebEx::releaseInstance(),"Release WebEx Instance");

    return TEST_OK;
}

// run test hook to be implemted
int TestPWM::doRunTest(){

    uint16_t counter = 2000;
    uint8_t dutyCycle = 50;
    uint16_t duration = 0; // 0 => infinite

    // start pwm on channel 1
    TESTASSERT(WebEx::getInstance()->startPWM(dutyCycle,duration),"Start Pwm Channel 1");

    // start the rtu counter for counting pwm periods
    TESTASSERT(RTU::getInstance()->setCaptureCount(RTU_CNT_CH1,counter),"Start counter on Channel 1");

    // wait counter seconds (+ 5 ms bonus)
    usleep(counter * 1000 +5000);

    bool success = RTU::getInstance()->isCaptureFinished(RTU_CNT_CH1);
    TESTASSERT((success == 0),"Check Counter finished Channel 1");

    TESTASSERT(RTU::getInstance()->stopCapture(RTU_CNT_CH1),"Stop counter on Channel 1");

    // get frequency channel 1
    uint16_t frequency = 0;
    TESTASSERT(RTU::getInstance()->getFrequency(RTU_CNT_CH1,&frequency),"Get frequency value!");

    //TODO other channels

    // stop pwm
    TESTASSERT(WebEx::getInstance()->stopPWM(),"Stop Pwm Channel 1");

    return TEST_OK;
}

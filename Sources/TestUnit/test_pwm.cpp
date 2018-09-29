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

    if (!mTestParameters.valid){
      mTestParameters.param1 = 3000; // cnt pwm1
      mTestParameters.param2 = 3000; // cnt pwm2
      mTestParameters.param3 = 0;    // cnt pwm3
      mTestParameters.param4 = 0;
      mTestParameters.param5 = 0;
    }

    return TEST_OK;
}


// run test hook to be implemted
int TestPWM::doRunTest(){

    uint8_t dutyCycle = 50;
    uint16_t duration = 0; // 0 => infinite
    uint16_t frequency = 0;
    bool success = 0;

    // start pwm on channel 1
    TESTASSERT(WebEx::getInstance()->startPWM(dutyCycle,duration),"Start Pwm");

    // RTU Counters
    TESTASSERT(RTU::getInstance()->stopCapture(RTU_CNT_CHANNEL1),"Stop Counter 1");
    TESTASSERT(RTU::getInstance()->stopCapture(RTU_CNT_CHANNEL2),"Stop Counter 2");
    TESTASSERT(RTU::getInstance()->stopCapture(RTU_CNT_CHANNEL3),"Stop Counter 3");
    TESTASSERT(RTU::getInstance()->stopCapture(RTU_CNT_CHANNEL4),"Stop Counter 4");

    //--------------
    // PWM 1
    //--------------

    if (mTestParameters.param1){
        log(TEST_SEVERITY_MESSAGE,"Testing PWM 1");

        // start the rtu counter for counting pwm periods
        TESTASSERT(RTU::getInstance()->setCaptureCount(RTU_CNT_CHANNEL1,mTestParameters.param1),"Set capture counter on Channel 1");

        TESTASSERT(RTU::getInstance()->startCapture(RTU_CNT_CHANNEL1),"Start capture Channel 1");

        // wait counter seconds (+ 5 ms bonus)
        usleep(mTestParameters.param1 * 1000 +5000);

        success = RTU::getInstance()->isCaptureFinished(RTU_CNT_CHANNEL1);
        TESTASSERT((success == 0),"Check Counter finished Channel 1");

        TESTASSERT(RTU::getInstance()->stopCapture(RTU_CNT_CHANNEL1),"Stop counter on Channel 1");

        // get frequency channel 1
        TESTASSERT(RTU::getInstance()->getFrequency(RTU_CNT_CHANNEL1,&frequency),"Get frequency value!");

        log(TEST_SEVERITY_MESSAGE,"Capture successful.Freq (Hz)="+to_string(frequency));
    }

    //--------------
    // PWM 2
    //--------------

    if (mTestParameters.param2){
        log(TEST_SEVERITY_MESSAGE,"Testing PWM 2");

        // start the rtu counter for counting pwm periods
        TESTASSERT(RTU::getInstance()->setCaptureCount(RTU_CNT_CHANNEL2,mTestParameters.param2),"Set capture counter on Channel 2");

        TESTASSERT(RTU::getInstance()->startCapture(RTU_CNT_CHANNEL2),"Start capture Channel 2");

        // wait counter seconds (+ 5 ms bonus)
        usleep(mTestParameters.param2 * 1000 +5000);

        success = RTU::getInstance()->isCaptureFinished(RTU_CNT_CHANNEL2);
        TESTASSERT((success == 0),"Check Counter finished Channel 2");

        TESTASSERT(RTU::getInstance()->stopCapture(RTU_CNT_CHANNEL2),"Stop counter on Channel 2");

        // get frequency channel 2
        frequency = 0;
        TESTASSERT(RTU::getInstance()->getFrequency(RTU_CNT_CHANNEL2,&frequency),"Get frequency value!");

        log(TEST_SEVERITY_MESSAGE,"Capture successful.Freq="+to_string(frequency));
    }

    //--------------
    // PWM 3
    //--------------

    if (mTestParameters.param3){
        log(TEST_SEVERITY_MESSAGE,"Testing PWM 3");

        // start the rtu counter for counting pwm periods
        TESTASSERT(RTU::getInstance()->setCaptureCount(RTU_CNT_CHANNEL3,mTestParameters.param3),"Set capture counter on Channel 3");

        TESTASSERT(RTU::getInstance()->startCapture(RTU_CNT_CHANNEL3),"Start capture Channel 3");

        // wait counter seconds (+ 5 ms bonus)
        usleep(mTestParameters.param3 * 1000 +5000);

        success = RTU::getInstance()->isCaptureFinished(RTU_CNT_CHANNEL3);
        TESTASSERT((success == 0),"Check Counter finished Channel 3");

        TESTASSERT(RTU::getInstance()->stopCapture(RTU_CNT_CHANNEL3),"Stop counter on Channel 3");

        // get frequency channel 3
        frequency = 0;
        TESTASSERT(RTU::getInstance()->getFrequency(RTU_CNT_CHANNEL3,&frequency),"Get frequency value!");

        log(TEST_SEVERITY_MESSAGE,"Capture successful.Freq="+to_string(frequency));
    }


    // stop pwm
    TESTASSERT(WebEx::getInstance()->stopPWM(),"Stop Pwm");

    return TEST_OK;
}

// tear down hook to be implemented
int TestPWM::doTearDown(){

    // make sure pwm is stopped
    TESTASSERT(WebEx::getInstance()->stopPWM(),"Stop Pwm");

    TESTASSERT(RTU::releaseInstance(),"Release RTU Instance");
    TESTASSERT(WebEx::releaseInstance(),"Release WebEx Instance");

    return TEST_OK;
}

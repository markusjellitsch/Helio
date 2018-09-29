//------------------------------------------------------------------------
// NAME         : test_rgb.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 14.09.2018
// DESCRIPTION  : test harness for RGB
// -----------------------------------------------------------------------

#include "test_rgb.h"
#include <unistd.h>
#include <helio.h>

// use HETI
#include "Heti/heti.h"

// use RTU
//#include "RTU/rtu.h"

using namespace std;

// setup hooks to be implemented
int TestRGB::doSetup(){

    // open the interface
    TESTASSERT(HETI::getInstance()->open(),"Connect to Heti");

    if (!mTestParameters.valid){
      mTestParameters.param1 = 4000000; // cnt usleep
      mTestParameters.param2 = 0;      //
      mTestParameters.param3 = 0;      //
      mTestParameters.param4 = 0;
      mTestParameters.param5 = 0;
    }

    return TEST_OK;
}



// run test hook to be implemted
int TestRGB::doRunTest(){

    int result = 1;

    // Set color
    writeStdOut("Set RGB Color to Green!");
    TESTASSERT(HETI::getInstance()->writeSingleRegister(1,WEBEX_RGB_GREEN),"Set RGB Color=Green");
    TESTASSERT(HETI::getInstance()->compareRegisterValue(1,WEBEX_RGB_GREEN,true),"Check RGB Color=Green");
    usleep(mTestParameters.param1);

    // Set color
    writeStdOut("Set RGB Color to LighBlue!");
    TESTASSERT(HETI::getInstance()->writeSingleRegister(1,WEBEX_RGB_LBLUE),"Set RGB Color=LightBlue");
    TESTASSERT(HETI::getInstance()->compareRegisterValue(1,WEBEX_RGB_LBLUE,true),"Check RGB Color=LightBlue");
    usleep(mTestParameters.param1);

    // Set color
    writeStdOut("Set RGB Color to DarkBlue!");
    TESTASSERT(HETI::getInstance()->writeSingleRegister(1,WEBEX_RGB_DBLUE),"Set RGB Color=DarkBlue");
    TESTASSERT(HETI::getInstance()->compareRegisterValue(1,WEBEX_RGB_DBLUE,true),"Check RGB Color=DarkBlue");
    usleep(mTestParameters.param1);

    // Set color
    writeStdOut("Set RGB Color to Red!");
    TESTASSERT(HETI::getInstance()->writeSingleRegister(1,WEBEX_RGB_RED),"Set RGB Color=Red");
    TESTASSERT(HETI::getInstance()->compareRegisterValue(1,WEBEX_RGB_RED,true),"Check RGB Color=Red");
    usleep(mTestParameters.param1);

    // Set color
    writeStdOut("Set RGB Color to Violet!");
    TESTASSERT(HETI::getInstance()->writeSingleRegister(1,WEBEX_RGB_VIOLET),"Set RGB Color=Violet");
    TESTASSERT(HETI::getInstance()->compareRegisterValue(1,WEBEX_RGB_VIOLET,true),"Check RGB Color=Violet");
    usleep(mTestParameters.param1);

    // Set color
    writeStdOut("Set RGB Color to Yellow!");
    TESTASSERT(HETI::getInstance()->writeSingleRegister(1,WEBEX_RGB_YELLOW),"Set RGB Color=Yellow");
    TESTASSERT(HETI::getInstance()->compareRegisterValue(1,WEBEX_RGB_YELLOW,true),"Check RGB Color=Yellow");
    usleep(mTestParameters.param1);

    // Set color
    writeStdOut("Set RGB Color to White!");
    TESTASSERT(HETI::getInstance()->writeSingleRegister(1,WEBEX_RGB_WHITE),"Set RGB Color=White");
    TESTASSERT(HETI::getInstance()->compareRegisterValue(1,WEBEX_RGB_WHITE,true),"Check RGB Color=White");
    usleep(mTestParameters.param1);

    //AskUser
    result = checkYesNo("All Colors OK?");
    TESTASSERT(result,"Check User Input");


    return TEST_OK;

}

// tear down hook to be implemented
int TestRGB::doTearDown(){

    // close the interface
    TESTASSERT(HETI::getInstance()->releaseInstance(),"Close Heti");

    return TEST_OK;
}

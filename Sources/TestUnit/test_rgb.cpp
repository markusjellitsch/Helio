//------------------------------------------------------------------------
// NAME         : test_rgb.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 14.09.2018
// DESCRIPTION  : test harness for RGB
// -----------------------------------------------------------------------

#include "test_rgb.h"
#include <unistd.h>

// use HETI
#include "Heti/heti.h"

// use RTU
//#include "RTU/rtu.h"

using namespace std;

// setup hooks to be implemented
int TestRGB::doSetup(){

    // open the interface
    int success = HETI::GetInstance()->open();
    if (success == HETI_OK) return TEST_OK;
    return success;

    return TEST_OK;
}



// run test hook to be implemted
int TestRGB::doRunTest(){

    int result = 1;

    // Set color
    writeStdOut("Set RGB Color to Green!");
    TESTASSERT(HETI::GetInstance()->writeSingleRegister(1,0),"Set RGB Color=Green");
    TESTASSERT(HETI::GetInstance()->compareRegisterValue(1,0,true),"Check RGB Color=Green");
    usleep(4000000);

    // Set color
    writeStdOut("Set RGB Color to LighBlue!");
    TESTASSERT(HETI::GetInstance()->writeSingleRegister(1,1),"Set RGB Color=LightBlue");
    TESTASSERT(HETI::GetInstance()->compareRegisterValue(1,1,true),"Check RGB Color=LightBlue");
    usleep(4000000);

    // Set color
    writeStdOut("Set RGB Color to DarkBlue!");
    TESTASSERT(HETI::GetInstance()->writeSingleRegister(1,2),"Set RGB Color=DarkBlue");
    TESTASSERT(HETI::GetInstance()->compareRegisterValue(1,2,true),"Check RGB Color=DarkBlue");
    usleep(4000000);

    // Set color
    writeStdOut("Set RGB Color to Red!");
    TESTASSERT(HETI::GetInstance()->writeSingleRegister(1,3),"Set RGB Color=Red");
    TESTASSERT(HETI::GetInstance()->compareRegisterValue(1,3,true),"Check RGB Color=Red");
    usleep(4000000);

    // Set color
    writeStdOut("Set RGB Color to Violet!");
    TESTASSERT(HETI::GetInstance()->writeSingleRegister(1,4),"Set RGB Color=Violet");
    TESTASSERT(HETI::GetInstance()->compareRegisterValue(1,4,true),"Check RGB Color=Violet");
    usleep(4000000);

    // Set color
    writeStdOut("Set RGB Color to Yellow!");
    TESTASSERT(HETI::GetInstance()->writeSingleRegister(1,5),"Set RGB Color=Yellow");
    TESTASSERT(HETI::GetInstance()->compareRegisterValue(1,5,true),"Check RGB Color=Yellow");
    usleep(4000000);

    // Set color
    writeStdOut("Set RGB Color to White!");
    TESTASSERT(HETI::GetInstance()->writeSingleRegister(1,6),"Set RGB Color=White");
    TESTASSERT(HETI::GetInstance()->compareRegisterValue(1,6,true),"Check RGB Color=White");
    usleep(2000000);

    //AskUser
    result = checkYesNo("All Colors OK?");
    TESTASSERT(result,"Check User Input");


    return TEST_OK;

}

// tear down hook to be implemented
int TestRGB::doTearDown(){

    return TEST_OK;
}

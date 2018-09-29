//------------------------------------------------------------------------
// NAME         : test_fu.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : test harness for FU (=Frequenzumrichter)
// -----------------------------------------------------------------------

#include "test_fu.h"
#include <string>
#include <iostream>
#include <helio.h>

using namespace std;

// -------------------------------
// setup hooks to be implemented
// -------------------------------
int TestFU::doSetup(){

    // open the interface
    TESTASSERT(HETI::getInstance()->open(),"Connect to Heti");

    if (!mTestParameters.valid){
      mTestParameters.param1 = 10;  // cnt writes to reg1
      mTestParameters.param2 = 0;   //
      mTestParameters.param3 = 0;   //
      mTestParameters.param4 = 0;
      mTestParameters.param5 = 0;
    }

    return TEST_OK;
}

// -----------------------------------
// run test hook to be implemented
// -----------------------------------
int TestFU::doRunTest(){

    int16_t val = 0;

    // Read from register 0;
    TESTASSERT(HETI::getInstance()->readSingleRegister(0,&val),"Reading Holding Reg 0");

    // Compare register
    TESTASSERT(HETI::getInstance()->compareRegisterValue(21,1,true),"Holding Reg 21=1");

    // loop
    for (int i=0;i<mTestParameters.param1;i++){

        // Write holding register
        TESTASSERT(HETI::getInstance()->writeSingleRegister(1,2),"Write Holding 1 val=2");

        // read back holding reg 2
        TESTASSERT(HETI::getInstance()->compareRegisterValue(1,2,true),"Holding Reg 1=2");
    }

    return TEST_OK;
}

// -----------------------------------
// tear down hook to be implemented
// -----------------------------------
int TestFU::doTearDown(){

    TESTASSERT(HETI::getInstance()->releaseInstance(),"Close Heti");
    return TEST_OK;
}

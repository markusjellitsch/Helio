//------------------------------------------------------------------------
// NAME         : test_fu.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : test harness for FU (=Frequenzumrichter)
// -----------------------------------------------------------------------

#include "test_fu.h"

// use HETI
#include "Heti/heti.h"

// use RTU
//#include "RTU/rtu.h"


using namespace std;

// -------------------------------
// setup hooks to be implemented
// -------------------------------
int TestFU::doSetup(){

    // open the interface
    int success = HETI::GetInstance()->open();
    if (success == HETI_OK) return TEST_OK;

    return success;
}

// -----------------------------------
// tear down hook to be implemented
// -----------------------------------
int TestFU::doTearDown(){

    return -1;
}

// -----------------------------------
// run test hook to be implemented
// -----------------------------------
int TestFU::doRunTest(){


    int16_t val = 0;

    // Read from register 0;
    TESTASSERT(HETI::GetInstance()->readSingleRegister(0,&val));

    // Update register.
    TESTASSERT(HETI::GetInstance()->readSingleRegister(0,&val));

    return TEST_OK;
}

//------------------------------------------------------------------------
// NAME         : test_vtx1.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 27.04.2018
// DESCRIPTION  : test harness for
// -----------------------------------------------------------------------

#include "test_vtx1.h"

using namespace std;

// setup hooks to be implemented
int TestVTX1::doSetup(){

    TESTASSERT(mTestInterface->open());

    TESTASSERT(mTestInterface->writeSingleRegister(0,0));


    return TEST_OK;
}

// tear down hook to be implemented
int TestVTX1::doTearDown(){

    return TEST_OK;
}

// run test hook to be implemted
int TestVTX1::doRunTest(){

    return TEST_OK;
}

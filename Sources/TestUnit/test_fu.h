//------------------------------------------------------------------------
// NAME         : test_fu.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : test harness for FU (= Frequenzumrichter)
// -----------------------------------------------------------------------

#ifndef TEST_FU_H
#define TEST_FU_H

#include <TestUnit/test_unit.h>
#include <Heti/heti.h>

class TestFU : public TestUnit
{
public:

    virtual ~TestFU()=default;

protected:

    // setup hooks to be implemented
    virtual int doSetup();

    // tear down hook to be implemented
    virtual int doTearDown();

    // run test hook to be implemted
    virtual int doRunTest();

 protected:

    // additional variables
    HETI * mTestInterface;
};

#endif

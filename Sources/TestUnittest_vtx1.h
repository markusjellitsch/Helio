//------------------------------------------------------------------------
// NAME         : test_vtx1.h
// AUTHOR       : Markus Jellitsch
// DATE         : 27.04.2018
// DESCRIPTION  : test harness for FU (= Frequenzumrichter)
// -----------------------------------------------------------------------

#ifndef test_vtx1_H
#define test_vtx1_H

#include <TestUnit/test_unit.h>

class TestVTX1 : public TestUnit
{
public:

    TestVTX1()=default;
    virtual ~TestVTX1()=default;

protected:

    // setup hooks to be implemented
    virtual int doSetup();

    // tear down hook to be implemented
    virtual int doTearDown();

    // run test hook to be implemted
    virtual int doRunTest();

 protected:

    // additional variables
};

#endif

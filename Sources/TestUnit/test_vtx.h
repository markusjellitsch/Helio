//------------------------------------------------------------------------
// NAME         : test_vtx.h
// AUTHOR       : Markus Jellitsch
// DATE         : 15.09.2018
// DESCRIPTION  : test harness for FU (= Frequenzumrichter)
// -----------------------------------------------------------------------

#ifndef test_vtx_H
#define test_vtx_H

#include <TestUnit/test_unit.h>

class TestVTX : public TestUnit
{
public:

    TestVTX()=default;
    virtual ~TestVTX()=default;

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

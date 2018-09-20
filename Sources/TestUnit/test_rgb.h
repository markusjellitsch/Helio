//------------------------------------------------------------------------
// NAME         : test_rgb.h
// AUTHOR       : Markus Jellitsch
// DATE         : 14.09.2018
// DESCRIPTION  : test harness for FU (= Frequenzumrichter)
// -----------------------------------------------------------------------

#ifndef test_rgb_H
#define test_rgb_H

#include <TestUnit/test_unit.h>

class TestRGB : public TestUnit
{
public:

    TestRGB()=default;
    virtual ~TestRGB()=default;

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

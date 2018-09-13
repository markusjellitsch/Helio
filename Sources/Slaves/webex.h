//------------------------------------------------------------------------
// NAME         : webex.h
// AUTHOR       : Markus Jellitsch
// DATE         : 26.08.2018
// DESCRIPTION  : Implementation of a  WebEx based class
// -----------------------------------------------------------------------

#ifndef WEBEX_H
#define WEBEX_H

#include <stdint.h>
#include <Heti/heti.h>


#define WEBEX_OK                             0

#define WEBEX_ADDRESS                        0x11

#define WEBEX_NUM_HOLDING_REGISTER           25


class WebEx
{
public:

    WebEx();
    ~WebEx()=default;

    int connect();

    int setRGBColor(uint8_t const color);

    int startPWM(uint8_t dutyCycle,uint16_t duration);
    int stopPWM();

    int startCounter(uint8_t cntNr, uint16_t count);
    int stopCounter();

    int setStepper(uint8_t stepNr,uint8_t const on);

    int readADC(uint16_t * value);

    int readDip(uint16_t * value);

    int dumpRegisters();
    int readRegister(uint8_t reg, int16_t * value);

private:

    HETI * mHeti;
    };

#endif // Webex

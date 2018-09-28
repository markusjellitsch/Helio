//------------------------------------------------------------------------
// NAME         : webex.h
// AUTHOR       : Markus Jellitsch
// DATE         : 26.08.2018
// DESCRIPTION  : Implementation of a  WebEx based class. This class uses
//                RS485 based modbusRTU communication for controling WebEx
// -----------------------------------------------------------------------

#ifndef WEBEX_H
#define WEBEX_H

#include <stdint.h>
#include <Heti/heti.h>


#define WEBEX_OK                             0

#define WEBEX_ADDRESS                        0x11

#define WEBEX_NUM_HOLDING_REGISTER           25

#define WEBEX_RGB_NUM_COLORS                 7
#define WEBEX_RGB_GREEN                      0
#define WEBEX_RGB_LBLUE                      1
#define WEBEX_RGB_DBLUE                    2
#define WEBEX_RGB_RED                      3
#define WEBEX_RGB_VIOLET                      4
#define WEBEX_RGB_YELLOW                     5
#define WEBEX_RGB_WHITE                     6

class WebEx
{
public:

    // singelton create & release functions
    static WebEx * getInstance();
    static int releaseInstance();

    // establish connection to the RS485 (=UART COM port)
    int connect();

    // set the WebEx rgb color
    int setRGBColor(uint8_t const color);

    // start & stop PWM channel
    int startPWM(uint8_t dutyCycle,uint16_t duration);
    int stopPWM();

    // start & stop a VTX frequency Counter
    int startCounter(uint8_t cntNr, uint16_t count);
    int stopCounter();

    // set & reset a stepper pin
    int setStepper(uint8_t stepNr,uint8_t const on);

    // read the WebEx analog channel
    int readADC(uint16_t * value);

    // read the dip switch value
    int readDip(uint16_t * value);

    // read a single register
    int readRegister(uint8_t reg, int16_t * value);

    // check register against a value
    int checkRegister(uint8_t reg,int16_t value);

    // dump all registers read via ModbusRTU
    int dumpRegisters();

private:

    // singelton
    WebEx();
    ~WebEx();

    HETI * mHeti=nullptr;
    static WebEx * mInstance;
 };

#endif // Webex

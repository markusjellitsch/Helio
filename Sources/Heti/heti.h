//------------------------------------------------------------------------
// NAME         : heti.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.02.2018
// DESCRIPTION  : Implementation of a  Modbus/RS485 based class
// -----------------------------------------------------------------------

#ifndef HETI_H
#define HETI_H

#include "Modbus/modbusrtu.h"
#include "Modbus/holdingregister.h"
#include "Logger/logger.h"
#include <ctime>
#include "rs485_rpi.h"


#define HETI_OK                             0


#define HETI_ADDRESS                        0x11

#define HETI_NUM_HOLDING_REGISTER  15

#define HETI_ERROR_RS485_WRITE_FAILED       -2
#define HETI_ERROR_RS485_READ_FAILED        -3
#define HETI_ERROR_NULLPTR                  -5
#define HETI_ERROR_INVALID_PACKET           -6
#define HETI_ERROR_COMPARISON_FAILED        -7


#define HETI_ERROR                          1
#define HETI_NOTE                           2




// Assert
#define HETIASSERT(x)                                                        \
{                                                                           \
    if ((x) != HETI_OK){                                                     \
        assertHandler(x,__FILE__,__LINE__);                                 \
        return x;                                                           \
    }                                                                       \
}


typedef struct {
   uint32_t commandsSent;
   uint32_t responsesReceived;
   uint32_t timeoutsOccured;
   uint32_t errorsOccured;
}HETIStatistic;

static RS485Config_t const cHetiRS485Config = {RS485_RPI_DEV_FILENAME,RS485_RPI_BAUD19200,1,0};

class HETI
{
public:

    /* make unique */
    static HETI * GetInstance();
    ~HETI();

    /* connect to rs485 serial port */
    int open(RS485Config_t * rs485Config = (RS485Config_t *)&cHetiRS485Config);

    /* set logger */
    int setLogger(Logger * const logger);

    /* read rtu internal registers */
    int readSingleRegister(uint8_t const reg,int16_t * value);
    int readMultiRegister(uint8_t const reg,uint8_t const num, ModbusHoldingRegister * holdings);

    /* write rtu internal register */
    int writeSingleRegister(uint8_t const reg, int16_t const value);

    /* return member register */
    int16_t getRegister(uint8_t const reg) const;

    /* read all registers from rtu */
    int updateRegisters();

    /* reset all possible registes in the rtu */
    int resetRegisters();

    /* print all registers to cout*/
    int dumpRegisters(bool const updateBefore=false);

    /* set & clear a single bit in a register */
    int setBit(uint8_t const reg, uint16_t const bitMask);
    int clearBit(uint8_t const reg, uint16_t const bitMask);

    /* compare register with specified value. This function needs reg update */
    int compareRegisterValue(uint8_t const reg, int16_t const value,bool const updateBefore=false);
    int compareRegisterMask(uint8_t const reg, int16_t const value,bool const updateBefore=false);
    int compareRegisterBit(uint8_t const reg, uint16_t const bit,bool const value,bool const updateBefore=false);
    int compareSet(uint8_t const reg, uint16_t const bit);
    int compareZero(uint8_t const reg, uint16_t const bit);

    /* Communication statistics */
    HETIStatistic getStatistic() const;
    int printStatistic() const;
    int setTerminalCursorBack(uint8_t const num)const;

    /* Stopwatch to measure time */
    int startTimer();
    double stopTimer();
    double getElapsed();


    /* rs485 transaction to read data from slave */
    int transaction(uint8_t * txData, unsigned int txLen,uint8_t * rxData,unsigned int rxLen);
    int transaction(ModbusFrame_t & send, ModbusFrame_t * receive);



private:

    /* delete constructor, copy,assignment */
    HETI();
    HETI(HETI const &) = delete;
    void operator=(HETI const &) = delete;

    /* log events */
    int log(uint8_t const logLevel, std::string const & text);

    /* assert handler */
    int assertHandler(int errorCode,std::string const & file, int line);

    /* timer functions */
    double getTime();
    void delayMilliseconds(uint16_t const milliseconds);

    ModbusHoldingRegister * mHoldingRegisters=nullptr;
    HETIStatistic mStatistic;
    RS485Rpi * mRS485Interface=nullptr;
    double mStopwatchCounter=-1;
    Logger * mLogger=nullptr;

   static HETI * mInstance;

   // default config for rs485

};

#endif // STM32_VLDISCO_H

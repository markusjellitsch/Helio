//------------------------------------------------------------------------
// NAME         : heti.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.02.2018
// DESCRIPTION  : Implementation for a modbus/rs485 based communication interface
// -----------------------------------------------------------------------

#include "heti.h"
#include "wiringPi.h"
#include "Modbus/holdingregister.h"
#include "Logger/logger.h"
#include "Modbus/modbusrtu.h"
#include <string>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <ctime>

using namespace std;

HETI * HETI::mInstance = nullptr;

/*---------------------------------------------------------------------------
 *  Return the singelton
 *--------------------------------------------------------------------------*/
HETI * HETI::GetInstance(){

    // create if not existing
    if (mInstance == nullptr){
        mInstance = new HETI;
        assert(mInstance != nullptr);
    }

    return mInstance;
}


/*---------------------------------------------------------------------------
 *  Constructor
 *--------------------------------------------------------------------------*/
HETI::HETI(){
    mLogger = nullptr;
    mRS485Interface = nullptr;
    mHoldingRegisters = new ModbusHoldingRegister(HETI_NUM_HOLDING_REGISTER);
}

/*---------------------------------------------------------------------------
 *  Destructor
 *--------------------------------------------------------------------------*/
HETI::~HETI(){

    delete mHoldingRegisters;
    mLogger = nullptr;
    mStopwatchCounter = -1;
    mRS485Interface->closeInterface();
    delete mRS485Interface ;
}

/*---------------------------------------------------------------------------
 * Connect to RTU by reading a special register. This register must contain
 * a specified value. Otherwise connection goes wrong
 *--------------------------------------------------------------------------*/
int HETI::open(RS485Config_t * rs485Config){

    if (rs485Config == nullptr) return HETI_ERROR_NULLPTR;

    // create if not existing
    if (mRS485Interface == nullptr){
        mRS485Interface = new RS485Rpi;
    }

    // open rs485
    int success = mRS485Interface->openInterface((void *)rs485Config);
    if (success){
        log(HETI_NOTE,"HETI initialization successful!");
    }
    else{
        log(HETI_NOTE,"HETI initialization successful!");
    }

    mStatistic.commandsSent = 0;
    mStatistic.errorsOccured = 0;
    mStatistic.responsesReceived = 0;
    mStatistic.timeoutsOccured = 0;

    return success;
}

int HETI::close(){
 return mRS485Interface->closeInterface();
}


/*---------------------------------------------------------------------------
 * Set Logger for logging events
 *--------------------------------------------------------------------------*/
int HETI::setLogger(Logger * const logger){

    mLogger = logger;
    HETIASSERT(mLogger == nullptr);

    return HETI_OK;
}

/*---------------------------------------------------------------------------
 * Log event (e.g to std:out or a file)
 *--------------------------------------------------------------------------*/
int HETI::log(uint8_t const logLevel,std::string const & text){


    if (mLogger == nullptr) return HETI_ERROR_NULLPTR;

    switch (logLevel){

        case HETI_ERROR:
            mLogger->Error(text);
            break;

         case HETI_NOTE:
             mLogger->Note(text);
             break;

         default:
             break;

     }

    return HETI_OK;
}


/*---------------------------------------------------------------------------
 * RS485 transaction to connected slave. This function transmits specified data and
 * return slave response back. This is the base function for a communication
 * between Raspberry Pi and slave.
 *--------------------------------------------------------------------------*/
int HETI::transaction(uint8_t * txData, unsigned int txLen,uint8_t * rxData,unsigned int rxLen){

    int error = 0;

    // transmit data
    error = mRS485Interface->writeMulti(0,txData,txLen);
    if (error != I_OK){
        log(HETI_ERROR,"Writing to RS485 failed!");
        return HETI_ERROR_RS485_WRITE_FAILED;
    }

     mStatistic.commandsSent++;

    // blocked waiting when reading
    error =  mRS485Interface->readMulti(0,rxData,rxLen);
    if ((size_t)error != rxLen){
        mStatistic.errorsOccured++;
        log(HETI_ERROR,"Reading from RS485 (first byte) failed!");
        return HETI_ERROR_RS485_READ_FAILED;
    }

    log(HETI_NOTE,"RS485 transaction OK!");

    return HETI_OK;
}


/*---------------------------------------------------------------------------
 * RS485 transaction to connected slave. This function transmits specified data and
 * return slave response back. This is the base function for a communication
 * between Raspberry Pi and slave.
 *--------------------------------------------------------------------------*/
int HETI::transaction(ModbusFrame_t & send, ModbusFrame_t * receive){

      uint8_t const cNumBytes = 255;
      uint8_t dataSend[cNumBytes] = {0};
      uint8_t dataReceive[cNumBytes] = {0};


      // prepare data frame
      int index = 0;
      dataSend[index++] = send.slaveAddress;
      dataSend[index++] = send.functionCode;
      for (int i = 0; i< send.dataLen;i++){
           dataSend[index +i] = send.data[i];

       }
       index+=send.dataLen;
       dataSend[index++] = send.crcLow;
       dataSend[index++] = send.crcHigh;


       if (receive == nullptr) return HETI_ERROR_NULLPTR;

       unsigned int numBytes = receive->dataLen + MODBUS_FRAME_OVERHEAD;

       int error = transaction(dataSend,index,dataReceive,numBytes);
       HETIASSERT(error);


       ModbusRTU response;
       response.setPayloadSize(receive->dataLen);

       // parse response frame to be valid
       if(response.parseData(dataReceive,numBytes)){
           response.getFrame(receive);
           mStatistic.responsesReceived++;
           return HETI_OK;
       }

       mStatistic.errorsOccured++;

       log(HETI_ERROR,"Invalid Response Packet received!");

       return HETI_ERROR_INVALID_PACKET;
}


/*---------------------------------------------------------------------------
 * Write a single register of the slave's internal registers. Note that some
 * internal registers can be marked as read only. Therfore these register can not
 * be written and this function won't return HETI_OK
 *--------------------------------------------------------------------------*/
int HETI::writeSingleRegister(uint8_t const reg, int16_t const value){

     uint8_t const cNumBytes = 4;
     uint8_t data[cNumBytes] = {0x0};
     data[1] = reg;
     data[2] = (uint8_t)(value >> 8);
     data[3] = (uint8_t)(value & 0xFF);

     ModbusRTU modbus;
     ModbusFrame_t responseFrame;
     responseFrame.dataLen = cNumBytes;

     // create modbus frame
     ModbusFrame_t sendFrame = modbus.createFrame(HETI_ADDRESS,MODBUS_FUNC_WRITESINGLE,data,cNumBytes);

     int error = transaction(sendFrame,&responseFrame);

    string text = "Write Single Register ";
    text.append(to_string(reg));

    if (error == HETI_OK){
        log(HETI_NOTE,text + " OK!");
    }
    else {
        log(HETI_ERROR,text + " NOK!");
    }


     return error;
 }

/*---------------------------------------------------------------------------
 * Read a single register of the slave's internal registers.
 *--------------------------------------------------------------------------*/
int HETI::readSingleRegister(uint8_t const reg,int16_t * value){

     uint8_t const cNumBytes = 4;
     uint8_t data[cNumBytes] = {0x0};
     data[1] = reg;
     data[2] = 0x00;
     data[3] = 0x01;

     // response frame
     ModbusRTU modbus;
     ModbusFrame_t responseFrame;
     responseFrame.dataLen = 3;

     // create modbus frame
     ModbusFrame_t sendFrame = modbus.createFrame(HETI_ADDRESS,MODBUS_FUNC_READHOLDING,data,cNumBytes);

     string text = "Reading Single Register ";
     text.append(to_string(reg));

     int error = transaction(sendFrame,&responseFrame);
     if (error == HETI_OK){

         *value = (int16_t)(responseFrame.data[1]<< 8) | responseFrame.data[2];

         // update internal holding reg
         mHoldingRegisters->setValue(reg,*value);

         log(HETI_NOTE,text + " OK!");

     }
     else  log(HETI_ERROR,text + " NOK!");

     return error;
}


/*---------------------------------------------------------------------------
 * Read multiple registers of the slave's internal registers.
 *--------------------------------------------------------------------------*/
 int HETI::readMultiRegister(uint8_t const reg,uint8_t const num, ModbusHoldingRegister * holdings){

     uint8_t const cNumBytes = 4;
     uint8_t data[cNumBytes] = {0x0};
     data[1] = reg;
     data[2] = 0x00;
     data[3] = num;

     ModbusRTU modbus;
     ModbusFrame_t responseFrame;
     responseFrame.dataLen = 1+num*2;

     ModbusFrame_t sendFrame = modbus.createFrame(HETI_ADDRESS,MODBUS_FUNC_READHOLDING,data,cNumBytes);

     int error = transaction(sendFrame,&responseFrame);

     string text = "Reading Multi Register " + num;

     if (error == HETI_OK){

         for (int i = 0; i<num;i++){
             holdings->setValue(reg+i,(int16_t)(responseFrame.data[2*i+1]<< 8) | responseFrame.data[2*i+2]);
         }

         log(HETI_NOTE,text + " OK!");
     }
     else log(HETI_ERROR,text + " NOK!");

     return error;
 }

 /*---------------------------------------------------------------------------
  * Dump all registers, which have been read the last time. The registers can
  * be updated before they are printed.
  *--------------------------------------------------------------------------*/
 int HETI::dumpRegisters(bool const updateBefore){

     if (mHoldingRegisters == 0) return -1;

     // update if necessary
     int error = 0;
     if (updateBefore){
        error = updateRegisters();
     }

     mHoldingRegisters->dump();

     return error;
}


 /*---------------------------------------------------------------------------
  * Update all registers of the slave and save them to the internal registers
  * of this class.
  *--------------------------------------------------------------------------*/
int HETI::updateRegisters(){

    if (mHoldingRegisters == nullptr) return -1;

    return  readMultiRegister(0,HETI_NUM_HOLDING_REGISTER,mHoldingRegisters);
}


/*---------------------------------------------------------------------------
 * Reset all registers of the slave.
 *--------------------------------------------------------------------------*/
int HETI::resetRegisters(){

    int error = 0;
    for (int i = 0; i<HETI_NUM_HOLDING_REGISTER;i++){
        int error = writeSingleRegister(i,0);
        if (error != HETI_OK){
            // IGNORE ERRORS!
        }
        cout << i << endl;
    }


   // after reset update
   error = updateRegisters();

   return error;
}


/*---------------------------------------------------------------------------
 * Return a register, which was read the last time.
 *--------------------------------------------------------------------------*/
int16_t HETI::getRegister(uint8_t const reg) const{

    if (reg > HETI_NUM_HOLDING_REGISTER) return HETI_OK;

    int16_t ret = 0;

    if (mHoldingRegisters->getValue(reg,&ret) != MODBUS_OK){
        return HETI_OK;
    }

    return ret;
}


/*---------------------------------------------------------------------------
 * Set a specified bit mask in the slave's internal register. This function can
 * be used to set a single bit in the register and avoid overwritig existing
 * value.
 *--------------------------------------------------------------------------*/
int HETI::setBit(uint8_t const reg, uint16_t const bitMask){

    int16_t value = 0;

    // read register
    int error = readSingleRegister(reg,&value);
    HETIASSERT(error);

    // set bit
    value = value | bitMask;

    // write back register
    error = writeSingleRegister(reg,value);

    return error;
}

/*---------------------------------------------------------------------------
 * Clear a bit mask in the slave's internal register. This function can be used
 * to reset a single bit in the register and avoid overwriting an existing value.
 *--------------------------------------------------------------------------*/
int HETI::clearBit(uint8_t const reg, uint16_t const bitMask){
    int16_t value = 0;

    // read register
    int error = readSingleRegister(reg,&value);
    HETIASSERT(error);

    // set bit
    value = value & ~bitMask;

    // write back register
    error = writeSingleRegister(reg,value);

    return error;
}


/*---------------------------------------------------------------------------
 * Compare a register with a specified value. This function returns HETI_OK
 * when both values match.
 *--------------------------------------------------------------------------*/
int HETI::compareRegisterValue(uint8_t const reg, int16_t const value,bool const updateBefore){

    int16_t tmp = 0;

    if (updateBefore){
        int error = readSingleRegister(reg,&tmp);
        HETIASSERT(error);
    }

    // get register value
    tmp =  getRegister(reg);

    // compare values
    if (tmp == value) return HETI_OK;

    return HETI_ERROR_COMPARISON_FAILED;
}

/*---------------------------------------------------------------------------
 * Compare a register with a specified bit mask. This function returns HETI_OK
 * when both values match.
 *--------------------------------------------------------------------------*/
int HETI::compareRegisterMask(uint8_t const reg, int16_t const value,bool const updateBefore){

    int16_t tmp = 0;

    if (updateBefore){
        readSingleRegister(reg,&tmp);
    }

    // get register value
    tmp =  getRegister(reg);

    // compare values
    if ((tmp & value) == value) return HETI_OK;

    return HETI_ERROR_COMPARISON_FAILED;
}



/*---------------------------------------------------------------------------
 * Compare a register with a specified bit value. This function returns HETI_OK
 * a specific bit 1 or zero.
 *--------------------------------------------------------------------------*/
int HETI::compareRegisterBit(uint8_t const reg, uint16_t const bit,bool const value,bool const updateBefore){

    int16_t tmp = 0;

    if (updateBefore){
      int error =   readSingleRegister(reg,&tmp);
      HETIASSERT(error);
    }

    // get register value
    tmp =  getRegister(reg);

    // get bit status
    bool isSet = ((tmp & bit) == bit);

    if (isSet == value) {
        return HETI_OK;
     }

    return HETI_ERROR_COMPARISON_FAILED;
}

/*---------------------------------------------------------------------------
 * Check if a specified register bit is set. In case of a match HETI_OK
 * is returned.
 *--------------------------------------------------------------------------*/
int HETI::compareSet(uint8_t const reg, uint16_t const bit){
        return compareRegisterBit(reg,bit,1,true);
}


/*---------------------------------------------------------------------------
 * Check if a specified register bit is zero. In case of a match HETI_OK
 * is returned.
 *--------------------------------------------------------------------------*/
int HETI::compareZero(uint8_t const reg, uint16_t const bit){
    return compareRegisterBit(reg,bit,0,true);
}




 /*---------------------------------------------------------------------------
  * Return Statistic
  *--------------------------------------------------------------------------*/
 HETIStatistic HETI::getStatistic() const{
     return mStatistic;
 }

 /*---------------------------------------------------------------------------
  * Print Statistic
  *--------------------------------------------------------------------------*/
 int HETI::printStatistic() const{

     cout << setw(35) << setfill('-') << "-" << setfill(' ')<< endl;
     cout << setw(35) << "RTU Statistic" << endl;
     cout << setw(35) << setfill('-') << "-" << setfill(' ') << endl;

     cout << setw(30) << "Modbus packets sent" << ":" << mStatistic.commandsSent << endl;
     cout << setw(30) << "Timouts occured" << ":" << mStatistic.timeoutsOccured << endl;
     cout << setw(30) << "Modbus reponses received" << ":" << mStatistic.responsesReceived << endl;
     cout << setw(30) << "Modbus errors received" << ":" << mStatistic.errorsOccured << endl;
     cout << setw(30) << "Error rate (%):" << ((double)mStatistic.errorsOccured +(double)mStatistic.timeoutsOccured) / (double)mStatistic.commandsSent * 100 << endl;

     return HETI_OK;
 }

 /*---------------------------------------------------------------------------
  * Move Terminal Cursor Up
  *--------------------------------------------------------------------------*/
 int HETI::setTerminalCursorBack(uint8_t const num)const{

     // set cursor to the beginning
     for (size_t i = 0; i<num; i++){
          cout << "\x1b[A";
     }

     return HETI_OK;
 }

 /*---------------------------------------------------------------------------
  * Start stopwatch
  *--------------------------------------------------------------------------*/
 int HETI::startTimer(){

     mStopwatchCounter = getTime();

     return HETI_OK;
 }

 /*---------------------------------------------------------------------------
  * Start stopwatch
  *--------------------------------------------------------------------------*/
 double HETI::stopTimer(){

     if (mStopwatchCounter == 0){
            return 0;
     }

     double elapsed = getElapsed();

     mStopwatchCounter = 0;

     return (elapsed);
 }

 /*---------------------------------------------------------------------------
  * Get stopwatch time
  *--------------------------------------------------------------------------*/
 double HETI::getTime(){

     struct timespec ts;
     clock_gettime(CLOCK_MONOTONIC,&ts);


     // calculate elapsed time
     uint64_t now = (uint64_t)ts.tv_sec * 1000000000U +(uint64_t)ts.tv_nsec;

     return (double)now;
 }

 /*---------------------------------------------------------------------------
  * Get elapsed time
  *--------------------------------------------------------------------------*/
 double HETI::getElapsed(){

     // calculate elapsed time (milliseconds)
     uint64_t elapsed = (getTime() - mStopwatchCounter) / 1000000;
     return elapsed;
 }

 void HETI::delayMilliseconds(uint16_t const milliseconds){

     double start = getTime();
     double elapsed = 0;

     while (elapsed <= milliseconds){
         elapsed = (getTime() - start) / 1000000;
     }
 }

 /*---------------------------------------------------------------------------
  * Assertion
  *--------------------------------------------------------------------------*/
int HETI::assertHandler(int errorCode,string const & file, int line){

    string text = "ASSERT FAILED IN";
    text.append(file);
    text.append(" IN LINE ");
    text.append(to_string(line));
    text.append(".ERROR CODE:"+ errorCode);
    log(HETI_ERROR,text);

    return HETI_OK;
}

/*---------------------------------------------------------------------------
 * Return Holding Register Instance
 *--------------------------------------------------------------------------*/
ModbusHoldingRegister * HETI::getHoldingRegisterInstance(){
    return mHoldingRegisters;
}

//------------------------------------------------------------------------
// NAME         : stm32_vldisco.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.02.2018
// DESCRIPTION  : This is the implementation of the RTU which are connected via
//                SPI to the Raspberry Pi. The RTU is able to generate PWM signal,
//                count frequency and has other functionality.
// -----------------------------------------------------------------------

#include "stm32_vldisco.h"
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

/*---------------------------------------------------------------------------
 * Constructor. Initialize holding registers and set SPI interface
 *--------------------------------------------------------------------------*/
STM32_VLDISCO::STM32_VLDISCO(SPIRPi * interface){

    mSPIInterface = interface;
    // config raspi pins with wiringPi library
    wiringPiSetup();
    pinMode(SPI_SLAVE_DRY_PIN,INPUT);
    pinMode(SPI_SLAVE_CS_PIN,OUTPUT);

    mHoldingRegisters = new ModbusHoldingRegister(RTU_NUM_HOLDING_REGISTER);
    assert(mHoldingRegisters != nullptr);

    // set default description
    mHoldingRegisters->setDescription(RTU_SYS_CR1,RTU_REG_DESC_SYS_CR1);
    mHoldingRegisters->setDescription(RTU_SYS_EN,RTU_REG_DESC_SYS_EN);
    mHoldingRegisters->setDescription(RTU_SYS_SR,RTU_REG_DESC_SYS_SR);
    mHoldingRegisters->setDescription(RTU_SYS_BOOT,RTU_REG_DESC_SYS_BOOT);
    mHoldingRegisters->setDescription(RTU_PWM_CR,RTU_REG_DESC_PWM_CR);
    mHoldingRegisters->setDescription(RTU_PWM_SR,RTU_REG_DESC_PWM_SR);
    mHoldingRegisters->setDescription(RTU_PWM_CNT,RTU_REG_DESC_PWM_CNT);
    mHoldingRegisters->setDescription(RTU_PWM_PRD,RTU_REG_DESC_PWM_PRD);
    mHoldingRegisters->setDescription(RTU_PWM_PSC,RTU_REG_DESC_PWM_PSC);
    mHoldingRegisters->setDescription(RTU_PWM_DUT,RTU_REG_DESC_PWM_DUT);
    mHoldingRegisters->setDescription(RTU_CNT_PRD,RTU_REG_DESC_CNT_PRD);
    mHoldingRegisters->setDescription(RTU_CNT_SR,RTU_REG_DESC_CNT_SR);
    mHoldingRegisters->setDescription(RTU_CNT_CH1,RTU_REG_DESC_CNT_CH1);
    mHoldingRegisters->setDescription(RTU_CNT_CH2,RTU_REG_DESC_CNT_CH2);
    mHoldingRegisters->setDescription(RTU_CNT_CH3,RTU_REG_DESC_CNT_CH3);
    mHoldingRegisters->setDescription(RTU_CNT_CH4,RTU_REG_DESC_CNT_CH4);
    mHoldingRegisters->setDescription(RTU_CNT_RES,RTU_REG_DESC_CNT_RES);

   // set default name
    mHoldingRegisters->setName(RTU_SYS_CR1,RTU_REG_NAME_SYS_CR1);
    mHoldingRegisters->setName(RTU_SYS_EN,RTU_REG_NAME_SYS_EN);
    mHoldingRegisters->setName(RTU_SYS_SR,RTU_REG_NAME_SYS_SR);
    mHoldingRegisters->setName(RTU_SYS_BOOT,RTU_REG_NAME_SYS_BOOT);
    mHoldingRegisters->setName(RTU_PWM_CR,RTU_REG_NAME_PWM_CR);
    mHoldingRegisters->setName(RTU_PWM_SR,RTU_REG_NAME_PWM_SR);
    mHoldingRegisters->setName(RTU_PWM_CNT,RTU_REG_NAME_PWM_CNT);
    mHoldingRegisters->setName(RTU_PWM_PRD,RTU_REG_NAME_PWM_PRD);
    mHoldingRegisters->setName(RTU_PWM_PSC,RTU_REG_NAME_PWM_PSC);
    mHoldingRegisters->setName(RTU_PWM_DUT,RTU_REG_NAME_PWM_DUT);
    mHoldingRegisters->setName(RTU_CNT_PRD,RTU_REG_NAME_CNT_PRD);
    mHoldingRegisters->setName(RTU_CNT_SR,RTU_REG_NAME_CNT_SR);
    mHoldingRegisters->setName(RTU_CNT_CH1,RTU_REG_NAME_CNT_CH1);
    mHoldingRegisters->setName(RTU_CNT_CH2,RTU_REG_NAME_CNT_CH2);
    mHoldingRegisters->setName(RTU_CNT_CH3,RTU_REG_NAME_CNT_CH3);
    mHoldingRegisters->setName(RTU_CNT_CH4,RTU_REG_NAME_CNT_CH4);
    mHoldingRegisters->setName(RTU_CNT_RES,RTU_REG_NAME_CNT_RES);

    mStatistic.commandsSent = 0;
    mStatistic.errorsOccured = 0;
    mStatistic.timeoutsOccured = 0;
    mStatistic.responsesReceived = 0;

    mStopwatchCounter = -1;

    mLogger = nullptr;
}

STM32_VLDISCO::~STM32_VLDISCO(){

    delete mHoldingRegisters;
    mLogger = nullptr;
    mStopwatchCounter = -1;
}

/*---------------------------------------------------------------------------
 * Connect to RTU by reading a special register. This register must contain
 * a specified value. Otherwise connection goes wrong
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::connect(){

    int error = updateRegisters();
    RTUASSERT(error);

    error = compareRegisterValue(RTU_SYS_BOOT,RTU_SYS_BOOT_SEQUENCE);
    RTUASSERT(error);

    RTUASSERT(writeSingleRegister(RTU_SYS_EN,0));
    RTUASSERT(writeSingleRegister(RTU_PWM_SR,0));

    log(RTU_NOTE,"RTU initialization successful!");

    return RTU_OK;
}

/*---------------------------------------------------------------------------
 * Testing SPI performance between RTU and Raspberry Pi
 *--------------------------------------------------------------------------*/
 int STM32_VLDISCO::testPerformance(unsigned int const  readCount, unsigned int const writeCount){

     int error = 0;
     int16_t value = 0;

     // testing command read holding
     log(RTU_NOTE,"Reading Holding Register Test!");
     for (unsigned int i = 0; i < readCount;i++){
        error = readSingleRegister(RTU_SYS_BOOT,&value);
        RTUASSERT(error);
        compareRegisterValue(RTU_SYS_BOOT,RTU_SYS_BOOT_SEQUENCE,false);

     }

    // testing command write holding
    log(RTU_NOTE,"Write Holding Register Test!");
    for (unsigned int i = 0;i<writeCount;i++){
         error = writeSingleRegister(RTU_SYS_CR1,0);
         RTUASSERT(error);
     }

    // print statistic
    printStatistic();

    return RTU_OK;
 }

/*---------------------------------------------------------------------------
 * Set Logger for logging events
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::setLogger(Logger * const logger){

    mLogger = logger;
    RTUASSERT(mLogger == nullptr);

    return RTU_OK;
}

/*---------------------------------------------------------------------------
 * Log event (e.g to std:out or a file)
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::log(uint8_t const logLevel,std::string const & text){


    if (mLogger == nullptr) return RTU_ERROR_NULLPTR;

    switch (logLevel){

        case RTU_ERROR:
            mLogger->Error(text);
            break;

         case RTU_NOTE:
             mLogger->Note(text);
             break;

         default:
             break;

     }


    return RTU_OK;
}


/*---------------------------------------------------------------------------
 * SPI transaction to the RTU. This function transmits specified data and
 * return slave response back. This is the base function for a communication
 * between Raspberry Pi and RTU.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::transaction(uint8_t * txData, unsigned int txLen,uint8_t * rxData,unsigned int * rxLen){


    // set slave select low
    digitalWrite(SPI_SLAVE_CS_PIN,0);

    // write data len as very first byte
    uint8_t len_byte =  (uint8_t)(txLen);
    int error = mSPIInterface->writeMulti(0,&len_byte,1);
    if (error != I_OK){
        log(RTU_ERROR,"Writing to SPI failed!");
        return RTU_ERROR_SPI_WRITE_FAILED;
    }

    // transmit data
    error = mSPIInterface->writeMulti(0,txData,txLen);
    if (error != I_OK){
        log(RTU_ERROR,"Writing to SPI failed!");
        return RTU_ERROR_SPI_WRITE_FAILED;
    }

     mStatistic.commandsSent++;

    bool timeout = 0;

    // start timer
    uint64_t start = getTime();

    // wait for stm32 to raise DATA READY line
    while (digitalRead(SPI_SLAVE_DRY_PIN)!=1){

        double elapsed = (getTime()-start)/1000000;
        if (elapsed >= RTU_DRY_TIMEOUT_MS) {
           timeout = true;
           break;
        }
    }


    // on timeout reset CS line & return
    if (timeout){
        log(RTU_ERROR,"DRY timeout occured!");
        // reset slave select
        digitalWrite(SPI_SLAVE_CS_PIN,1);
        mStatistic.timeoutsOccured++;
        return RTU_ERROR_SPI_DRY_TIMEOUT;
   }

    // read first byte from stm32 to get data len
    error =  mSPIInterface->readMulti(0,rxData,1);
    if (error != I_OK){
        mStatistic.errorsOccured++;
        log(RTU_ERROR,"Reading from SPI (first byte) failed!");
        return RTU_ERROR_SPI_READ_FAILED;
    }

    // read rest bytes
    uint16_t len = rxData[0];
    error =  mSPIInterface->readMulti(0,rxData,len);
    if (error != I_OK){
        mStatistic.errorsOccured++;
        log(RTU_ERROR,"Reading from SPI (payload bytes) failed!");
        return RTU_ERROR_SPI_READ_FAILED;
    }


    // reset slave select
    digitalWrite(SPI_SLAVE_CS_PIN,1);

    log(RTU_NOTE,"SPI transaction OK!");

    *rxLen = len;

    // return total data read
    return RTU_OK;
}

/*---------------------------------------------------------------------------
 * SPI transaction to the RTU. This function transmits specified data and
 * return slave response back. This is the base function for a communication
 * between Raspberry Pi and RTU.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::transaction(ModbusFrame_t & send, ModbusFrame_t * receive){

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


       if (receive == nullptr) return RTU_ERROR_NULLPTR;

       unsigned int numBytes = 0;

       int error = transaction(dataSend,index,dataReceive,&numBytes);
       RTUASSERT(error);


       ModbusRTU response;
       response.setPayloadSize(receive->dataLen);

       // parse response frame to be valid
       if(response.parseData(dataReceive,numBytes)){
           response.getFrame(receive);
           mStatistic.responsesReceived++;
           return RTU_OK;
       }

       mStatistic.errorsOccured++;

       log(RTU_ERROR,"Invalid Response Packet received!");

       return RTU_ERROR_INVALID_PACKET;
}


/*---------------------------------------------------------------------------
 * Write a single register of the RTU's internal registers. Note that some
 * internal registers can be marked as read only. Therfore this register can not
 * be written and this function won't return RTU_OK
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::writeSingleRegister(uint8_t const reg, int16_t const value){

     uint8_t const cNumBytes = 4;
     uint8_t data[cNumBytes] = {0x0};
     data[1] = reg;
     data[2] = (uint8_t)(value >> 8);
     data[3] = (uint8_t)(value & 0xFF);

     ModbusRTU modbus;
     ModbusFrame_t responseFrame;
     responseFrame.dataLen = cNumBytes;

     // create spi frame
     ModbusFrame_t sendFrame = modbus.createFrame(RTU_ADDRESS,MODBUS_FUNC_WRITESINGLE,data,cNumBytes);

     int error = transaction(sendFrame,&responseFrame);

    string text = "Write Single Register " + to_string(reg);

    if (error == RTU_OK){
        log(RTU_NOTE,text + " OK!");
    }
    else {
        log(RTU_ERROR,text + + " NOK!");
    }


     return error;
 }

/*---------------------------------------------------------------------------
 * Read a single register of the RTU's internal registers.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::readSingleRegister(uint8_t const reg,int16_t * value){

     uint8_t const cNumBytes = 4;
     uint8_t data[cNumBytes] = {0x0};
     data[1] = reg;
     data[2] = 0x00;
     data[3] = 0x01;

     // response frame
     ModbusRTU modbus;
     ModbusFrame_t responseFrame;
     responseFrame.dataLen = 3;

     // create spi frame
     ModbusFrame_t sendFrame = modbus.createFrame(RTU_ADDRESS,MODBUS_FUNC_READHOLDING,data,cNumBytes);

     string text = "Reading Single Register " + reg;

     int error = transaction(sendFrame,&responseFrame);
     if (error == RTU_OK){

         *value = (int16_t)(responseFrame.data[1]<< 8) | responseFrame.data[2];

         // update internal holding reg
         mHoldingRegisters->setValue(reg,*value);

         log(RTU_NOTE,text + " OK!");

     }
     else  log(RTU_ERROR,text + " NOK!");

     return error;
}


/*---------------------------------------------------------------------------
 * Read multiple registers of the RTU' internal registers.
 *--------------------------------------------------------------------------*/
 int STM32_VLDISCO::readMultiRegister(uint8_t const reg,uint8_t const num, ModbusHoldingRegister * holdings){

     uint8_t const cNumBytes = 4;
     uint8_t data[cNumBytes] = {0x0};
     data[1] = reg;
     data[2] = 0x00;
     data[3] = num;

     ModbusRTU modbus;
     ModbusFrame_t responseFrame;
     responseFrame.dataLen = 1+num*2;

     ModbusFrame_t sendFrame = modbus.createFrame(RTU_ADDRESS,MODBUS_FUNC_READHOLDING,data,cNumBytes);

     int error = transaction(sendFrame,&responseFrame);

     string text = "Reading Multi Register " + num;

     if (error == RTU_OK){

         for (int i = 0; i<num;i++){
             holdings->setValue(reg+i,(int16_t)(responseFrame.data[2*i+1]<< 8) | responseFrame.data[2*i+2]);
         }

         log(RTU_NOTE,text + " OK!");
     }
     else log(RTU_ERROR,text + " NOK!");

     return error;
 }

 /*---------------------------------------------------------------------------
  * Dump all registers, which have been read the last time. The registers can
  * be updated before they are printed.
  *--------------------------------------------------------------------------*/
 int STM32_VLDISCO::dumpRegisters(bool const updateBefore){

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
  * Update all registers of the RTU and save them to the internal registers
  * of this class.
  *--------------------------------------------------------------------------*/
int STM32_VLDISCO::updateRegisters(){

    if (mHoldingRegisters == nullptr) return -1;

    return  readMultiRegister(0,RTU_NUM_HOLDING_REGISTER,mHoldingRegisters);
}


/*---------------------------------------------------------------------------
 * Reset all registers of the RTU.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::resetRegisters(){

    int error = 0;
    for (int i = 0; i<RTU_NUM_HOLDING_REGISTER;i++){
        int error = writeSingleRegister(i,0);
        if (error != RTU_OK){
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
int16_t STM32_VLDISCO::getRegister(uint8_t const reg) const{

    if (reg > RTU_NUM_HOLDING_REGISTER) return RTU_OK;

    int16_t ret = 0;

    if (mHoldingRegisters->getValue(reg,&ret) != MODBUS_OK){
        return RTU_OK;
    }

    return ret;
}


/*---------------------------------------------------------------------------
 * Set a specified bit mask in the RTU's internal register. This function can
 * be used to set a single bit in the register and avoid overwritig existing
 * value.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::setBit(uint8_t const reg, uint16_t const bitMask){

    int16_t value = 0;

    // read register
    int error = readSingleRegister(reg,&value);
    RTUASSERT(error);

    // set bit
    value = value | bitMask;

    // write back register
    error = writeSingleRegister(reg,value);

    return error;
}

/*---------------------------------------------------------------------------
 * Clear a bit mask in the RTU's internal register. This function can be used
 * to reset a single bit in the register and avoid overwriting an existing value.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::clearBit(uint8_t const reg, uint16_t const bitMask){
    int16_t value = 0;

    // read register
    int error = readSingleRegister(reg,&value);
    RTUASSERT(error);

    // set bit
    value = value & ~bitMask;

    // write back register
    error = writeSingleRegister(reg,value);

    return error;
}


/*---------------------------------------------------------------------------
 * Compare a register with a specified value. This function returns RTU_OK
 * when both values match.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::compareRegisterValue(uint8_t const reg, int16_t const value,bool const updateBefore){

    int16_t tmp = 0;

    if (updateBefore){
        int error = readSingleRegister(reg,&tmp);
        RTUASSERT(error);
    }

    // get register value
    tmp =  getRegister(reg);

    // compare values
    if (tmp == value) return RTU_OK;

    return RTU_ERROR_COMPARISON_FAILED;
}

/*---------------------------------------------------------------------------
 * Compare a register with a specified bit mask. This function returns RTU_OK
 * when both values match.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::compareRegisterMask(uint8_t const reg, int16_t const value,bool const updateBefore){

    int16_t tmp = 0;

    if (updateBefore){
        readSingleRegister(reg,&tmp);
    }

    // get register value
    tmp =  getRegister(reg);

    // compare values
    if ((tmp & value) == value) return RTU_OK;

    return RTU_ERROR_COMPARISON_FAILED;
}

/*---------------------------------------------------------------------------
 * Compare a register with a specified bit value. This function returns RTU_OK
 * a specific bit 1 or zero.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::compareRegisterBit(uint8_t const reg, uint16_t const bit,bool const value,bool const updateBefore){

    int16_t tmp = 0;

    if (updateBefore){
      int error =   readSingleRegister(reg,&tmp);
      RTUASSERT(error);
    }

    // get register value
    tmp =  getRegister(reg);

    // get bit status
    bool isSet = ((tmp & bit) == bit);

    if (isSet == value) {
        return RTU_OK;
     }

    return RTU_ERROR_COMPARISON_FAILED;
}

/*---------------------------------------------------------------------------
 * Check if a specified register bit is set. In case of a match RTU_OK
 * is returned.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::compareSet(uint8_t const reg, uint16_t const bit){
        return compareRegisterBit(reg,bit,1,true);
}


/*---------------------------------------------------------------------------
 * Check if a specified register bit is zero. In case of a match RTU_OK
 * is returned.
 *--------------------------------------------------------------------------*/
int STM32_VLDISCO::compareZero(uint8_t const reg, uint16_t const bit){
    return compareRegisterBit(reg,bit,0,true);
}



/*---------------------------------------------------------------------------
 * Start PWM signal on a specified channel with a specified period and a
 * specified duty cycle.
 *--------------------------------------------------------------------------*/
 int STM32_VLDISCO::startPWM(uint8_t const channel, uint16_t const period, uint8_t const duty){

    int error = writeSingleRegister(RTU_PWM_CR,0);
    error = clearBit(RTU_SYS_EN,channel);
    RTUASSERT(error);


    error = writeSingleRegister(RTU_PWM_PSC,24);
    RTUASSERT(error);

    error = writeSingleRegister(RTU_PWM_PRD,period);
    RTUASSERT(error);

    error = writeSingleRegister(RTU_PWM_DUT,(uint16_t)duty);
    RTUASSERT(error);

    error = setBit(RTU_SYS_EN,channel);
    RTUASSERT(error);

    error = setBit(RTU_PWM_CR,0);
    RTUASSERT(error);

    return RTU_OK;
 }

 /*---------------------------------------------------------------------------
  * Set PWM Count. This is the total number of PWM pulsed, which are generated
  * by the RTU
  *--------------------------------------------------------------------------*/
 int STM32_VLDISCO::setPWMCount(uint16_t const count){

    int error = writeSingleRegister(RTU_PWM_CNT,count);
    RTUASSERT(error);

    cout << "Writing to " << RTU_REG_NAME_PWM_CNT << " OK" << endl;
    return RTU_OK;
 }

 /*---------------------------------------------------------------------------
  * Stop PWM singnal of a specified channel
  *--------------------------------------------------------------------------*/
 int STM32_VLDISCO::stopPWM(uint8_t const channel){
    return clearBit(RTU_SYS_EN,channel);
 }


 /*---------------------------------------------------------------------------
  * Return if a PWM channel is off
  *--------------------------------------------------------------------------*/
 bool STM32_VLDISCO::isPWMOff(uint8_t const channel){

     int enabled = compareSet(RTU_SYS_EN,channel);
     int status = compareSet(RTU_PWM_SR,channel);

     if (enabled == RTU_OK && status == RTU_OK) return true;

     return false;
 }

 int STM32_VLDISCO::setCaptureCount(uint8_t const channel, uint16_t const value){

    int error = -1;

    switch (channel){

        case RTU_CNT_CHANNEL1:
            error = writeSingleRegister(RTU_CNT_CH1,value);
            break;

        case RTU_CNT_CHANNEL2:
            error = writeSingleRegister(RTU_CNT_CH2,value);
            break;

        case RTU_CNT_CHANNEL3:
            error = writeSingleRegister(RTU_CNT_CH3,value);
            break;

        case RTU_CNT_CHANNEL4:
            error = writeSingleRegister(RTU_CNT_CH4,value);
            break;

        default:
            break;
    }

    RTUASSERT(error);

    return RTU_OK;
 }

 int STM32_VLDISCO::setCaptureResolution(uint16_t const resolution){

     int error = writeSingleRegister(RTU_CNT_RES,resolution);
     RTUASSERT(error);

     return RTU_OK;
 }

int STM32_VLDISCO::startCapture(uint8_t const channel){

    int error = setBit(RTU_SYS_EN,channel);
    RTUASSERT(error);

    error = compareSet(RTU_SYS_EN,channel);
    RTUASSERT(error);

    return RTU_OK;
}

int STM32_VLDISCO::stopCapture(uint8_t const channel){

    int error = clearBit(RTU_SYS_EN,channel);
    RTUASSERT(error);

    return RTU_OK;
}

bool STM32_VLDISCO::isCaptureFinished(uint8_t const channel){

    int error = writeSingleRegister(RTU_CNT_SR,channel>>4);
    if (error != RTU_OK) return false;

    error = compareZero(RTU_CNT_SR,channel>>4);
    if (error != RTU_OK) return false;

    return true;
}

int STM32_VLDISCO::getFrequency(uint8_t const channel,uint16_t * value){

    if (value == nullptr) return RTU_ERROR_NULLPTR;

    // writing to status register will wr
    int error = writeSingleRegister(RTU_CNT_PRD,channel>>4);
    RTUASSERT(error);

    int16_t period = 0;
    error = readSingleRegister(RTU_CNT_PRD,&period);
    RTUASSERT(error);

    // calculate frequency
    *value = period;
    return RTU_OK;
}

 /*---------------------------------------------------------------------------
  * Return Statistic
  *--------------------------------------------------------------------------*/
 RTUStatistic STM32_VLDISCO::getStatistic() const{
     return mStatistic;
 }

 /*---------------------------------------------------------------------------
  * Print Statistic
  *--------------------------------------------------------------------------*/
 int STM32_VLDISCO::printStatistic() const{

     cout << setw(35) << setfill('-') << "-" << setfill(' ')<< endl;
     cout << setw(35) << "RTU Statistic" << endl;
     cout << setw(35) << setfill('-') << "-" << setfill(' ') << endl;

     cout << setw(30) << "Modbus packets sent" << ":" << mStatistic.commandsSent << endl;
     cout << setw(30) << "Timouts occured" << ":" << mStatistic.timeoutsOccured << endl;
     cout << setw(30) << "Modbus reponses received" << ":" << mStatistic.responsesReceived << endl;
     cout << setw(30) << "Modbus errors received" << ":" << mStatistic.errorsOccured << endl;
     cout << setw(30) << "Error rate (%):" << ((double)mStatistic.errorsOccured +(double)mStatistic.timeoutsOccured) / (double)mStatistic.commandsSent * 100 << endl;
     return RTU_OK;
 }


 /*---------------------------------------------------------------------------
  * Start stopwatch
  *--------------------------------------------------------------------------*/
 int STM32_VLDISCO::startTimer(){

     mStopwatchCounter = getTime();

     return RTU_OK;
 }

 /*---------------------------------------------------------------------------
  * Start stopwatch
  *--------------------------------------------------------------------------*/
 double STM32_VLDISCO::stopTimer(){


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
 double STM32_VLDISCO::getTime(){

     struct timespec ts;
     clock_gettime(CLOCK_MONOTONIC,&ts);


     // calculate elapsed time
     uint64_t now = (uint64_t)ts.tv_sec * 1000000000U +(uint64_t)ts.tv_nsec;

     return (double)now;
 }

 /*---------------------------------------------------------------------------
  * Get elapsed time
  *--------------------------------------------------------------------------*/
 double STM32_VLDISCO::getElapsed(){

     // calculate elapsed time
     uint64_t elapsed = (getTime() - mStopwatchCounter) / 1000000;
     return elapsed;
 }

 void STM32_VLDISCO::delayMilliseconds(uint16_t const milliseconds){

     double start = getTime();
     double elapsed = 0;

     while (elapsed <= milliseconds){
         elapsed = (getTime() - start) / 1000000;
     }
 }


int STM32_VLDISCO::assertHandler(int errorCode,string const & file, int line){

    string text = "ASSERT FAILED IN";
    text.append(file);
    text.append(" IN LINE ");
    text.append(to_string(line));
    text.append(".ERROR CODE:"+ errorCode);
    log(RTU_ERROR,text);

    return RTU_OK;
}
















//------------------------------------------------------------------------
// NAME         : stm32_vldisco.h
// AUTHOR       : Markus Jellitsch
// DATE         : 6.02.2018
// DESCRIPTION  : This is the implementation of the RTU which are connected via
//                SPI to the Raspberry Pi. The RTU is able to generate PWM signal,
//                count frequency and has other functionality.
// -----------------------------------------------------------------------

#ifndef STM32_VLDISCO_H
#define STM32_VLDISCO_H

#include "spirpi.h"
#include "Modbus/modbusrtu.h"
#include "Modbus/holdingregister.h"
#include "Logger/logger.h"
#include <ctime>


#define RTU_OK                      0

#define RTU_NUM_HOLDING_REGISTER    20

#define RTU_SYS_BOOT_SEQUENCE       0x4711

#define RTU_DRY_TIMEOUT_MS          10

#define RTU_SYS_BASE_REG            (MODBUS_HOLDING_REG_4001)
#define RTU_SYS_CR1					(RTU_SYS_BASE_REG)
#define RTU_SYS_EN					(RTU_SYS_BASE_REG + 1)
#define RTU_SYS_SR					(RTU_SYS_BASE_REG + 2)
#define RTU_SYS_BOOT				(RTU_SYS_BASE_REG + 3)

#define RTU_PWM_BASE 				(MODBUS_HOLDING_REG_4006)
#define RTU_PWM_CR 					(RTU_PWM_BASE) 					     // control register
#define RTU_PWM_SR                  (RTU_PWM_BASE + 1)					// status register
#define RTU_PWM_PRD                 (RTU_PWM_BASE + 2)					// period register
#define RTU_PWM_CNT                 (RTU_PWM_BASE + 3)					// period register
#define RTU_PWM_PSC                 (RTU_PWM_BASE + 4)					// period register
#define RTU_PWM_DUT                 (RTU_PWM_BASE + 5)					// period register

#define RTU_CNT_BASE 				(MODBUS_HOLDING_REG_4012)
#define RTU_CNT_PRD 					(RTU_CNT_BASE) 					     // control register
#define RTU_CNT_SR                  (RTU_CNT_BASE + 1)					// status register
#define RTU_CNT_CH1                 (RTU_CNT_BASE + 2)					// period register
#define RTU_CNT_CH2                 (RTU_CNT_BASE + 3)					// period register
#define RTU_CNT_CH3                 (RTU_CNT_BASE + 4)					// period register
#define RTU_CNT_CH4                 (RTU_CNT_BASE + 5)					// period register
#define RTU_CNT_RES                 (RTU_CNT_BASE + 6)

#define RTU_SYS_EN_PWM1_Msk         (0x01 << 0)
#define RTU_SYS_EN_PWM2_Msk         (0x01 << 1)
#define RTU_SYS_EN_PWM3_Msk         (0x01 << 2)
#define RTU_SYS_EN_PWM4_Msk         (0x01 << 3)
#define RTU_SYS_EN_CNT1_Msk         (0x01 << 4)
#define RTU_SYS_EN_CNT2_Msk         (0x01 << 5)
#define RTU_SYS_EN_CNT3_Msk         (0x01 << 6)
#define RTU_SYS_EN_CNT4_Msk         (0x01 << 7)

#define RTU_PWM_SR_CH1_Msk          (0x01 << 0)
#define RTU_PWM_SR_CH2_Msk          (0x01 << 1)
#define RTU_PWM_SR_CH3_Msk          (0x01 << 2)
#define RTU_PWM_SR_CH4_Msk          (0x01 << 3)

#define RTU_CNT_SR_CH1_Msk          (0x01 << 0)
#define RTU_CNT_SR_CH2_Msk          (0x01 << 1)
#define RTU_CNT_SR_CH3_Msk          (0x01 << 2)
#define RTU_CNT_SR_CH4_Msk          (0x01 << 3)


#define RTU_REG_NAME_SYS_CR1        "SYS_CR1"
#define RTU_REG_NAME_SYS_EN         "SYS_EN"
#define RTU_REG_NAME_SYS_SR         "SYS_SR"
#define RTU_REG_NAME_SYS_BOOT       "SYS_BOOT"
#define RTU_REG_NAME_PWM_CR         "PWM_CR"
#define RTU_REG_NAME_PWM_SR         "PWM_SR"
#define RTU_REG_NAME_PWM_PRD        "PWM_PRD"
#define RTU_REG_NAME_PWM_CNT        "PWM_CNT"
#define RTU_REG_NAME_PWM_PSC        "PWM_PSC"
#define RTU_REG_NAME_PWM_DUT        "PWM_DUT"
#define RTU_REG_NAME_CNT_PRD         "CNT_PRD"
#define RTU_REG_NAME_CNT_SR         "CNT_SR"
#define RTU_REG_NAME_CNT_CH1        "CNT_CH1"
#define RTU_REG_NAME_CNT_CH2        "CNT_CH2"
#define RTU_REG_NAME_CNT_CH3        "CNT_CH3"
#define RTU_REG_NAME_CNT_CH4        "CNT_CH4"
#define RTU_REG_NAME_CNT_RES        "CNT_RES"

#define RTU_REG_DESC_SYS_CR1        "System Control Register Bank 1"
#define RTU_REG_DESC_SYS_EN         "System Enable Register"
#define RTU_REG_DESC_SYS_SR         "System Status Register"
#define RTU_REG_DESC_SYS_BOOT       "System BOOT Register"
#define RTU_REG_DESC_PWM_CR         "Enable PWM channels"
#define RTU_REG_DESC_PWM_SR         "Information about channels finished"
#define RTU_REG_DESC_PWM_PRD        "Set period of PWM"
#define RTU_REG_DESC_PWM_CNT        "Set count of PWM pulses"
#define RTU_REG_DESC_PWM_PSC        "Set resolution of PWM"
#define RTU_REG_DESC_PWM_DUT        "Set duty cycle"
#define RTU_REG_DESC_CNT_PRD        "Measured period of a specified channel"
#define RTU_REG_DESC_CNT_SR         "Information about frequency channels finished"
#define RTU_REG_DESC_CNT_CH1        "Set counter value for channel 1"
#define RTU_REG_DESC_CNT_CH2        "Set counter value for channel 2"
#define RTU_REG_DESC_CNT_CH3        "Set counter value for channel 3 "
#define RTU_REG_DESC_CNT_CH4        "Set counter value for channel 4"
#define RTU_REG_DESC_CNT_RES        "Set counting resolution to get exact result"

#define RTU_ADDRESS                 0x11

#define RTU_PWM_CHANNEl1            RTU_SYS_EN_PWM1_Msk
#define RTU_PWM_CHANNEl2            RTU_SYS_EN_PWM2_Msk
#define RTU_PWM_CHANNEl3            RTU_SYS_EN_PWM3_Msk
#define RTU_PWM_CHANNEl4            RTU_SYS_EN_PWM4_Msk

#define RTU_CNT_CHANNEL1            (RTU_SYS_EN_CNT1_Msk)
#define RTU_CNT_CHANNEL2            (RTU_SYS_EN_CNT2_Msk)
#define RTU_CNT_CHANNEL3            (RTU_SYS_EN_CNT3_Msk)
#define RTU_CNT_CHANNEL4            (RTU_SYS_EN_CNT4_Msk)

#define RTU_ERROR_SPI_WRITE_FAILED      -2
#define RTU_ERROR_SPI_READ_FAILED       -3
#define RTU_ERROR_SPI_DRY_TIMEOUT       -4
#define RTU_ERROR_NULLPTR               -5
#define RTU_ERROR_INVALID_PACKET        -6
#define RTU_ERROR_COMPARISON_FAILED     -7


#define RTU_ERROR                         1
#define RTU_NOTE                          2




#define RTUASSERT(x)                                                        \
{                                                                           \
    if ((x) != RTU_OK){                                                     \
        assertHandler(x,__FILE__,__LINE__);                                 \
        return x;                                                           \
    }                                                                       \
}


typedef struct {
   uint32_t commandsSent;
   uint32_t responsesReceived;
   uint32_t timeoutsOccured;
   uint32_t errorsOccured;
}RTUStatistic;


class STM32_VLDISCO
{
public:

    /* constructor */
    STM32_VLDISCO(SPIRPi * interface);
    ~STM32_VLDISCO();

    /* connect to rtu */
    int connect();

    /* run a test */
    int testPerformance(unsigned int const  readCount, unsigned int const writeCount);

    /* set logger */
    int setLogger(Logger * const logger);

    /* read rtu internal registers */
    int readSingleRegister(uint8_t const reg,int16_t * value);
    int readMultiRegister(uint8_t const reg,uint8_t const num, ModbusHoldingRegister * holdings);

    /* write rtu internal register */
    int writeSingleRegister(uint8_t const reg, int16_t const value);

    /* start & stop pwm on RTU */
    int setPWMCount(uint16_t const count);
    int startPWM(uint8_t const channel, uint16_t const period, uint8_t const duty);
    int stopPWM(uint8_t const channel);
    bool isPWMOff(uint8_t const channel);

    /* frequency counter functions */
    int setCaptureCount(uint8_t const channel, uint16_t const value);
    int setCaptureResolution(uint16_t const resolution);
    int startCapture(uint8_t const channel);
    int stopCapture(uint8_t const channel);
    bool isCaptureFinished(uint8_t const channel);
    int getFrequency(uint8_t const channel,uint16_t * value);

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

    /* RTU statistics */
    RTUStatistic getStatistic() const;
    int printStatistic() const;

    /* Stopwatch to measure time */
    int startTimer();
    double stopTimer();
    double getTime();
    double getElapsed();
    void delayMilliseconds(uint16_t const milliseconds);

private:

    /* spi transaction to read data from rtu */
    int transaction(uint8_t * txData, unsigned int txLen,uint8_t * rxData,unsigned int * rxLen);
    int transaction(ModbusFrame_t & send, ModbusFrame_t * receive);

    /* log events */
    int log(uint8_t const logLevel, std::string const & text);

    /* assert handler */
    int assertHandler(int errorCode,std::string const & file, int line);

    ModbusHoldingRegister * mHoldingRegisters=nullptr;
    RTUStatistic mStatistic;
    SPIRPi * mSPIInterface=nullptr;
    double mStopwatchCounter=-1;
    Logger * mLogger=nullptr;
};

#endif // STM32_VLDISCO_H

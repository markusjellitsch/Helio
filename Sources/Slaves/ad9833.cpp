//------------------------------------------------------------------------
// NAME         : ad9833.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : Driver class for the AD9833 waveform generator
// -----------------------------------------------------------------------

#include "ad9833.h"
#include <interface.h>
#include <slave.h>
#include <vector>
#include <unistd.h>

using namespace  std;


/* ------------------------------------
 * Write to the internal registers of the
 * waveform generator.
 * --------------------------------------*/
int AD9833::writeRegister(uint16_t const reg, uint16_t const value){
     uint16_t tmp = value | reg;

     unsigned char buf[2];
     buf[0] = (uint8_t)(tmp >> 8);
     buf[1] = (uint8_t)tmp&0xFF;
     int success = mInterface->writeMulti(0,buf,2);
     usleep(100);
     return success;
}


/* ------------------------------------
 * Set output frequency
 * --------------------------------------*/
int AD9833::setFrequency(const int freq){

    uint32_t value = (double)freq / AD9833_FREQUENCY * AD9833_FREQUENCY_DIV;
    value &= 0x0FFFFFFF;


    int success = writeRegister(AD9833_CONTROL_REG, AD9833_RESET_BIT);

    uint16_t low =value &0x3FFFF;

    success = writeRegister(AD9833_FREQ0_REG,low);

    success = writeRegister(AD9833_CONTROL_REG, AD9833_HLB_BIT |AD9833_RESET_BIT);

    uint16_t high = value>> 14;
    success = writeRegister(AD9833_FREQ0_REG,high);

    success = writeRegister(AD9833_CONTROL_REG, 0);

    return success;

}

/* ------------------------------------
 * Set a value to the control register
 * --------------------------------------*/
int AD9833::setControl(uint16_t mask,bool reset){
    uint16_t tmp = mask | reset;
    return writeRegister(AD9833_CONTROL_REG,tmp);
}


/* ------------------------------------
 * Specify output mode (Sinus, Triangle,
 * Rectangle)
 * --------------------------------------*/
int AD9833::setOutputMode(Output_Mode_t mode){
    if (mode == Triangle)setControl(AD9833_MODE_BIT);
    else if (mode == Sinus) setControl(0);

    return 0;
}





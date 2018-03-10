//------------------------------------------------------------------------
// NAME         : pca9685.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 6.10.2017
// DESCRIPTION  : PWM controller class with 16 channels
// -----------------------------------------------------------------------

#include "pca9685.h"
#include <interface.h>
#include <slave.h>
#include <string>

using namespace std;


/* --------------------------------------
 * Reset the pwm controller
 * --------------------------------------*/
void PCA9685::reset() {

    writeRegister(MODE1,0x01);
    writeRegister(MODE2,0x04);
}


/* --------------------------------------
 * Write to a specific register
 * --------------------------------------*/
int PCA9685::writeRegister(uint8_t const reg, uint8_t const value){

    unsigned const cDataSize=2;
    uint8_t data[cDataSize];
    SlaveData_t send;

    data[0] =reg;
    data[1] =value;
    send.push_back(data[0]);
    send.push_back(data[1]);
    return writeToSlave(send,cDataSize);
}

/* --------------------------------------
 * Set PWM Frequency for all channels
 * --------------------------------------*/
void PCA9685::setPWMFreq(int freq) {

        uint8_t prescale_val = (CLOCK_FREQ / 4096 / freq)-1;

        writeRegister(MODE1,0x10);
        writeRegister(PRE_SCALE,prescale_val);
        writeRegister(MODE1,0x80);
        writeRegister(MODE2,0x04);
}

/* --------------------------------------
 * Set PWM duty cycle of a channel
 * --------------------------------------*/
void PCA9685::setPWM(uint8_t led, int value) {
    setPWM(led, 0, value);
}

/* --------------------------------------
 * Set PWM duty cycle of a channel
 * --------------------------------------*/
void PCA9685::setPWM(uint8_t led, int on_value, int off_value) {


        uint8_t ledOnLow = LED0_ON_L + LED_MULTIPLYER * (led);
        uint8_t ledOnHigh = LED0_ON_H + LED_MULTIPLYER * (led);
        uint8_t ledOffLow = LED0_OFF_L + LED_MULTIPLYER * (led);
        uint8_t ledOffHigh = LED0_OFF_H + LED_MULTIPLYER * (led);

        writeRegister(ledOnLow,on_value & 0xFF);
        writeRegister(ledOnHigh,on_value >> 8);
        writeRegister(ledOffLow,off_value & 0xFF);
        writeRegister(ledOffHigh,off_value >> 8);
}

/* --------------------------------------
 * Read and update register bank of PWM Controller
 * --------------------------------------*/
int PCA9685::readRegisters(){
    for (unsigned int i = 0; i<PWM_NUM_REGISTERS;i++) mRegisters[i] = 0;

    SlaveData_t response;
    writeRegister(MODE1,0x20); // autoincremnt
    readFromSlave(&response,PWM_NUM_REGISTERS);

    if (response.size()!=PWM_NUM_REGISTERS){
        return -1;
    }

    for (unsigned int i = 0;i< response.size();i++){
        mRegisters[i] = response[i];
    }

    return 0;
}

/* --------------------------------------
 * Return Register. But first read all.
 * --------------------------------------*/
int PCA9685::getRegister(uint8_t reg){
    int ledval = 0;



   int success = readRegisters();
   if (!success){
        return -1;
   }
   //todo

    return 0;
}


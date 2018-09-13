//------------------------------------------------------------------------
// NAME         : webex.cpp
// AUTHOR       : Markus Jellitsch
// DATE         : 26.08.2018
// DESCRIPTION  : Implementation for webex control
// -----------------------------------------------------------------------

#include <Heti/heti.h>
#include <Slaves/webex.h>
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

WebEx::WebEx(){
     mHeti = HETI::GetInstance();
     mHeti->getHoldingRegisterInstance()->setName(0,"RGB MODE");
     mHeti->getHoldingRegisterInstance()->setName(1,"RGB COLOR");
     mHeti->getHoldingRegisterInstance()->setName(2,"RGB LOCK");
     mHeti->getHoldingRegisterInstance()->setName(3,"PWM DUTY");
     mHeti->getHoldingRegisterInstance()->setName(4,"PWM DUR");
     mHeti->getHoldingRegisterInstance()->setName(5,"PWM ON");
     mHeti->getHoldingRegisterInstance()->setName(6,"VTX CNT");
     mHeti->getHoldingRegisterInstance()->setName(7,"VTX ON");
     mHeti->getHoldingRegisterInstance()->setName(8,"STEP ALL");
     mHeti->getHoldingRegisterInstance()->setName(9,"STEP 1");
     mHeti->getHoldingRegisterInstance()->setName(10,"STEO 2");
     mHeti->getHoldingRegisterInstance()->setName(11,"STEP 3");
     mHeti->getHoldingRegisterInstance()->setName(12,"STEP 4");
     mHeti->getHoldingRegisterInstance()->setName(13,"CAN DIR");
     mHeti->getHoldingRegisterInstance()->setName(13,"CAN ON");
     mHeti->getHoldingRegisterInstance()->setName(15,"DIP SWITCH");
     mHeti->getHoldingRegisterInstance()->setName(16,"ANALOG R");
     mHeti->getHoldingRegisterInstance()->setName(17,"ANALOG C");
     mHeti->getHoldingRegisterInstance()->setName(22,"HW TYPE");
 }

int WebEx::connect(){

    if (mHeti == nullptr) return -2;

    int success = 0;

    success = mHeti->open();
    if (success != HETI_OK){
        return -1;
    }

    int16_t value = 0;
    success = mHeti->readSingleRegister(0,&value);
    if (success != HETI_OK){
        return -2;
    }

    return WEBEX_OK;
}

int WebEx::setRGBColor(const uint8_t color){

    if (mHeti == nullptr){
        return -1;
    }

    uint8_t tmp = color;

    if (tmp > 6) tmp = 6;

    int success = mHeti->writeSingleRegister(1,tmp);
    if (success != HETI_OK){
        return -2;
    }

    return WEBEX_OK;
}

int WebEx::startPWM(const uint8_t duty,uint16_t duration){

    if (mHeti == nullptr){
        return -1;
    }

    int success = 0;

    success = mHeti->writeSingleRegister(3,duty);
    if (success != HETI_OK){
        return -2;
    }

    success = mHeti->writeSingleRegister(4,duration);
    if (success != HETI_OK){
        return -3;
    }

    success = mHeti->writeSingleRegister(5,1);
    if (success != HETI_OK){
        return -3;
    }

    return WEBEX_OK;
}

int WebEx::startCounter(uint8_t cntNr, uint16_t count){

    if (mHeti == nullptr){
        return -1;
    }

    int success = 0;

    success = mHeti->writeSingleRegister(6,count);
    if (success != HETI_OK){
        return -2;
    }

    success = mHeti->writeSingleRegister(7,cntNr);
    if (success != HETI_OK){
        return -3;
    }

    return WEBEX_OK;
}
int WebEx::stopCounter(){

    if (mHeti == nullptr){
        return -1;
    }

    int success = 0;

    success = mHeti->writeSingleRegister(6,0);
    if (success != HETI_OK){
        return -2;
    }

    success = mHeti->writeSingleRegister(7,0);
    if (success != HETI_OK){
        return -3;
    }

    return WEBEX_OK;
}

int WebEx::stopPWM(){

    if (mHeti == nullptr){
        return -1;
    }

    int success = 0;

    success = mHeti->writeSingleRegister(5,0);
    if (success != HETI_OK){
        return -3;
    }

    return WEBEX_OK;
}

int WebEx::dumpRegisters(){

    if (mHeti == nullptr){
        return -1;
    }

    return mHeti->dumpRegisters(1);
}

int WebEx::readRegister(uint8_t reg, int16_t * value){

    if (mHeti == nullptr){
        return -1;
    }

    int success = 0;

    success = mHeti->readSingleRegister(reg,value);
    if (success != HETI_OK){
        return -3;
    }

    return WEBEX_OK;
}

int WebEx::setStepper(uint8_t stepNr,uint8_t const on){

    if (mHeti == nullptr){
        return -1;
    }

    int success = 0;

    uint8_t tmp = stepNr;
    if (tmp >4) tmp = 4;
    if (tmp ==0) tmp = 1;


    success = mHeti->writeSingleRegister(8+tmp,on);
    if (success != HETI_OK){
        return -3;
    }

    return WEBEX_OK;
}


int WebEx::readADC(uint16_t * value){

    if (mHeti == nullptr){
        return -1;
    }

    int success = 0;

    success = mHeti->readSingleRegister(17,(int16_t *)value);
    if (success != HETI_OK){
        return -3;
    }

    return WEBEX_OK;
}

int WebEx::readDip(uint16_t * value){

    if (mHeti == nullptr){
        return -1;
    }

    int success = 0;

    success = mHeti->readSingleRegister(15,(int16_t*)value);
    if (success != HETI_OK){
        return -3;
    }

    return WEBEX_OK;
}

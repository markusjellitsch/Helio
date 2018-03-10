#include "ssd1306.h"
#include "Drivers/SSD1306/ArduiPi_SSD1306.h"
#include "Drivers/SSD1306/Adafruit_GFX.h"
#include "Drivers/SSD1306/Adafruit_SSD1306.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <cassert>
#include <stdarg.h>

using namespace std;




/* -----------------------------------------
 * Constructor
 *------------------------------------------*/
SSD1306::SSD1306()
{
    mDisplay = new Adafruit_SSD1306;
    assert(mDisplay != nullptr);
    setSlaveAddress(ADAFRUIT_I2C_ADDRESS);
}


/* -----------------------------------------
 * Desctructor
 *------------------------------------------*/
SSD1306::~SSD1306(){
    delete  mDisplay;
}


/* -----------------------------------------
 * Initialize Display
 *------------------------------------------*/
bool SSD1306::initDisplay(){

   ASSERT_ERROR((mDisplay != nullptr),"Display nullptr!");

   size_t const cOLEDType = 2;      // this can be found in Adafruit driver
   if (mDisplay->init(OLED_I2C_RESET,cOLEDType))
   {

        mLogger->Note("SSD1306 initialized!");
   }
   else
   {
        mLogger->Note("SSD1306 not initialzed!");
        return false;
   }

   // open display
    mDisplay->begin();

    // default settings
    mDisplay->setTextSize(1);
    mDisplay->setTextColor(WHITE);
    mDisplay->setCursor(0,0);

    return true;

}

/* -----------------------------------------
 * Simple printf function
 *------------------------------------------*/
void SSD1306::printf( const char * format, ...){

    if (mDisplay == nullptr) return;

    size_t const cBufferSize = 150;
    char buf[cBufferSize];

    // get argumentes and convert to snptrintf
    va_list args;
    va_start (args, format);
    vsnprintf(buf,100,format,args);
    mDisplay->print(buf);
    va_end (args);
}

/* -----------------------------------------
 * Close display
 *------------------------------------------*/
void SSD1306::closeDisplay(){
    mDisplay->close();
    mLogger->Note("SSD1306 closed!");
}


/* -----------------------------------------
 * Set Focus to a specific line (0-Lastline)
 *------------------------------------------*/
void SSD1306::setFocus(const uint8_t line){
    uint8_t tmp = line;
    if (line > SSD1306_LASTLINE) tmp = SSD1306_LASTLINE;
    mDisplay->setCursor(0,tmp*(SSD1306_CHAR_HEIGHT+1));
}

/* -----------------------------------------
 * Clear a specific line
 *------------------------------------------*/
void SSD1306::clearLine(uint8_t const line){

    uint8_t tmp = line;
    if (line > SSD1306_LASTLINE) tmp = SSD1306_LASTLINE;
    for (uint8_t i = 0; i< (SSD1306_CHAR_HEIGHT+1);i++){
        mDisplay->drawLine(0,tmp*(SSD1306_CHAR_HEIGHT+1)+i,128,tmp*(SSD1306_CHAR_HEIGHT+1)+i,BLACK);
    }
    //mDisplay->display();
}

/* -----------------------------------------
 * Clear whole display
 *------------------------------------------*/
void SSD1306::clearDisplay(){
    mDisplay->clearDisplay();
    mDisplay->display();
}


/* -----------------------------------------
 * Show what has to be  displayed
 *------------------------------------------*/
void SSD1306::display(){

    mDisplay->display();
}


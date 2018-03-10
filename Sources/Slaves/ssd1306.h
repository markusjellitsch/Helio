#ifndef SSD1306_H
#define SSD1306_H
#include <iostream>
#include <string>
#include <slave.h>
#include "Drivers/SSD1306/ArduiPi_SSD1306.h"
#include "Drivers/SSD1306/Adafruit_GFX.h"
#include "Drivers/SSD1306/Adafruit_SSD1306.h"

#define SSD1306_LINE0       0
#define SSD1306_LINE1       1
#define SSD1306_LINE2       2
#define SSD1306_LINE3       3
#define SSD1306_LINE4       4
#define SSD1306_LINE5       5
#define SSD1306_LINE6       6
#define SSD1306_LINE7       7

#define ASSERT_ERROR(con,text)                      \
do {                                                \
    if (!con){                                      \
        if(mLogger!=nullptr)mLogger->Error(text);   \
        return false;                               \
    }                                               \
}while(0)                                           \


#define SSD1306_LASTLINE    SSD1306_LINE3 // change if necessary

#define SSD1306_BLACK       BLACK
#define SSD1306_WHITE       WHITE

#define SSD1306_CHAR_HEIGHT  7
#define SSD1306_CHAR_WIDTH   5

class SSD1306 : public Slave
{
public:

    SSD1306();
    ~SSD1306();


    bool initDisplay();
    void closeDisplay();
    void display();

    void printf(const char * format, ...);

    void setFocus(const uint8_t line);

    void clearLine(uint8_t const line);
    void clearDisplay();


 private:
    Adafruit_SSD1306 * mDisplay=nullptr;
};

#endif // SSD1306_H

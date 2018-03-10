QT += core
QT -= gui

CONFIG += c++11

TARGET = dac_mcp4725

target.path = /home/pi/Application
INSTALLS += target
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Interface/i2crpi.cpp \
    ../../Sources/Slaves/slave.cpp \
    ../../Sources/Slaves/mcp4725.cpp \
    ../../Sources/Logger/stdlogger.cpp \
    ../../Sources/Logger/logger.cpp \
    ../../Sources/Drivers/SSD1306/glcdfont.c \
    ../../Sources/Slaves/ssd1306.cpp


HEADERS += \
    ../../Sources/Interface/interface.h \
    ../../Sources/Slaves/slave.h \
    ../../Sources/Slaves/mcp4725.h \
    ../../Sources \
    ../../Sources/Logger/stdlogger.h \
    ../../Sources/Logger/logger.h \
    ../../Sources/Drivers/SSD1306/Adafruit_GFX.h \
    ../../Sources/Drivers/SSD1306/Adafruit_SSD1306.h \
    ../../Sources/Drivers/SSD1306/ArduiPi_SSD1306.h \
    ../../Sources/Drivers/SSD1306/bcm2835.h \
    ../../Sources/Slaves/ssd1306.h

    ../../Sources/Interface/i2crpi.h

INCLUDEPATH += \
    /home/user/raspi/sysroot/usr/include \
   /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
    ../../Sources \
    ../../Sources/Interface \
    ../../Sources/Slaves\
    ../../Sources/Logger/

LIBS += -lssd13061



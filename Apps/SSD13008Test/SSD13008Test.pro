QT += core
QT -= gui

CONFIG += c++11
INSTALLS += target
target.path = /home/pi/Application

TARGET = display_ssd13008
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Slaves/ssd1306.cpp \
    ../../Sources/Interface/i2crpi.cpp \
    ../../Sources/Slaves/slave.cpp \
    ../../Sources/Logger/stdlogger.cpp \
    ../../Sources/Logger/logger.cpp

HEADERS += \
    ../../Sources/Slaves/ssd1306.h \
    ../../Sources/Interface/i2crpi.h \
    ../../Sources/Slaves/slave.h \
    ../../Sources/Logger/stdlogger.h \
    ../../Sources/Logger/logger.h \
    ../../Sources/Interface/interface.h
    Adafruit_GFX.h


INCLUDEPATH += /home/user/raspi/sysroot/usr/include \
            /home/user/RPI_COM/Apps/SSD13008Test \
            /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
            ../../Sources \
            ../../Sources/Interface \
            ../../Sources/Slaves\
            ../../Sources/Logger/


LIBS+=  -lssd1306

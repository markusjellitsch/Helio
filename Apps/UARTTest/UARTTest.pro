QT += core
QT -= gui

CONFIG += c++11
INSTALLS += target
target.path = /home/pi/Application

TARGET = uterm
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Logger/stdlogger.cpp \
    ../../Sources/Logger/logger.cpp \
    ../../Sources/Interface/uart_rpi.cpp

HEADERS += \
    ../../Sources/Logger/stdlogger.h \
    ../../Sources/Logger/logger.h \
    ../../Sources/Interface/interface.h \
    ../../Sources/Interface/uart_rpi.h
    Adafruit_GFX.h


INCLUDEPATH += /home/user/raspi/sysroot/usr/include \
            /home/user/RPI_COM/Apps/SSD13008Test \
            /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
            ../../Sources \
            ../../Sources/Interface \
            ../../Sources/Slaves\
            ../../Sources/Logger/


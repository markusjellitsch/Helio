QT += core
QT -= gui

CONFIG += c++11
INSTALLS += target
target.path = /home/pi/Application
TARGET = spi_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Interface/spirpi.cpp \
    ../../Sources/Interface/uart_rpi.cpp

INCLUDEPATH += /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
            ../../Sources \
            ../../Sources/Interface \
            ../../Sources/Slaves\
            ../../Sources/Logger/


LIBS+=  -lwiringPi

HEADERS += \
    ../../Sources/Interface/interface.h \
    ../../Sources/Interface/spirpi.h \
    ../../Sources/Interface/uart_rpi.h

QT += core
QT -= gui

CONFIG += c++11
INSTALLS += target
target.path = /home/pi/HelioTests

TARGET = fu_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \


HEADERS += \
     ../../Sources/helio.h

LIBS+=  -lwiringPi -L../heliolib -lhelio

INCLUDEPATH += /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
            ../../Sources \



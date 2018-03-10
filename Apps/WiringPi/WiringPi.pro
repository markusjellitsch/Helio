QT += core
QT -= gui


CONFIG += c++11

TARGET = WiringPi
INSTALLS += target
target.path = /home/pi/Application

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
    ../../Sources/ArduiPi_SSD1306/bcm2835.c


INCLUDEPATH += /home/user/raspi/sysroot/usr/include/\
                ../../Sources/ArduiPi_SSD1306/ \

HEADERS +=

DISTFILES +=





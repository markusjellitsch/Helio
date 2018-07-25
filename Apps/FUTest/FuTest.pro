QT += core
QT -= gui

CONFIG += c++11
INSTALLS += target
target.path = /home/pi/Application

TARGET = heti_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Logger/stdlogger.cpp \
    ../../Sources/Logger/logger.cpp \
    ../../Sources/Interface/rs485_rpi.cpp \
    ../../Sources/Modbus/modbusrtu.cpp \
    ../../Sources/Modbus/holdingregister.cpp \
    ../../Sources/Heti/heti.cpp \
    ../../Sources/TestUnit/test_unit.cpp \
    ../../Sources/TestUnit/test_fu.cpp \
    ../../Sources/TestUnit/test_runner.cpp

HEADERS += \
    ../../Sources/Logger/stdlogger.h \
    ../../Sources/Logger/logger.h \
    ../../Sources/Interface/interface.h \
    ../../Sources/Interface/rs485_rpi.h \
    ../../Sources/Modbus/modbusrtu.h \
    ../../Sources/Modbus/holdingregister.h \
    ../../Sources/Heti/heti.h \
    ../../Sources/TestUnit/test_fu.h \
    ../../Sources/TestUnit/test_runner.h
    Adafruit_GFX.h

LIBS+=  -lssd1306 -lwiringPi

INCLUDEPATH += /home/user/raspi/sysroot/usr/include \
            /home/user/RPI_COM/Apps/SSD13008Test \
            /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
            ../../Sources \
            ../../Sources/Interface \
            ../../Sources/Slaves\
            ../../Sources/Logger/


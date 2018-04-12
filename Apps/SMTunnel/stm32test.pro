QT += core
QT -= gui

CONFIG += c++11
INSTALLS += target
target.path = /home/pi/Application
TARGET = smtunnel
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Interface/spirpi.cpp \
    ../../Sources/Interface/uart_rpi.cpp \
    ../../Sources/Slaves/stm32_vldisco.cpp \
    ../../Sources/Modbus/modbusrtu.cpp \
    ../../Sources/Modbus/modbusholdingregister.cpp

INCLUDEPATH += /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
            ../../Sources \
            ../../Sources/Interface \
            ../../Sources/Slaves\
            ../../Sources/Logger/


LIBS+=  -lssd1306 -lwiringPi

HEADERS += \
    ../../Sources/Interface/interface.h \
    ../../Sources/Interface/spirpi.h \
    ../../Sources/Interface/uart_rpi.h \
    ../../Sources/Slaves/stm32_vldisco.h \
    ../../Sources/Modbus/modbusrtu.h \
    ../../Sources/Modbus/modbusholdingregister.h

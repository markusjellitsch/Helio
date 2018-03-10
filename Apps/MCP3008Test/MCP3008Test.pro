QT += core
QT -= gui

CONFIG += c++11
INSTALLS += target
target.path = /home/pi/Application
TARGET = adc_mcp3008
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Interface/spirpi.cpp \
    ../../Sources/Interface/i2crpi.cpp \
    ../../Sources/Slaves/mcp4725.cpp \
    ../../Sources/Slaves/mcp3008.cpp \
    ../../Sources/Slaves/slave.cpp \
    ../../Sources/Logger/stdlogger.cpp \
    ../../Sources/Logger/logger.cpp

INCLUDEPATH += /home/user/raspi/sysroot/usr/include \
            /home/user/RPI_COM/Apps/SSD13008Test \
            /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
            ../../Sources \
            ../../Sources/Interface \
            ../../Sources/Slaves\
            ../../Sources/Logger/


LIBS+=  -lssd1306

HEADERS += \
    ../../Sources/Interface/interface.h \
    ../../Sources/Interface/spirpi.h \
    ../../Sources/Interface/i2crpi.h \
    ../../Sources/Slaves/mcp4725.h \
    ../../Sources/Slaves/mcp3008.h \
    ../../Sources/Slaves/slave.h \
    ../../Sources/Logger/stdlogger.h \
    ../../Sources/Logger/logger.h

QT += core
QT -= gui

CONFIG += c++11
INSTALLS += target
target.path = /home/pi/Application
TARGET = wave_ad9833
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Interface/spirpi.cpp \
    ../../Sources/Slaves/slave.cpp \
    ../../Sources/Logger/stdlogger.cpp \
    ../../Sources/Logger/logger.cpp \
    ../../Sources/Slaves/ad9833.cpp

INCLUDEPATH += /home/user/raspi/sysroot/usr/include \
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
    ../../Sources/Slaves/slave.h \
    ../../Sources/Logger/stdlogger.h \
    ../../Sources/Logger/logger.h \
    ../../Sources/Slaves/ad9833.h

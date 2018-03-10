QT += core
QT -= gui

CONFIG += c++11

TARGET = pwm_pca9685

target.path = /home/pi/Application
INSTALLS += target
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Interface/i2crpi.cpp \
    ../../Sources/Slaves/slave.cpp \
    ../../Sources/Logger/stdlogger.cpp \
    ../../Sources/Logger/logger.cpp \
    ../../Sources/Slaves/pca9685.cpp


HEADERS += \
    ../../Sources/Interface/interface.h \
    ../../Sources/Slaves/slave.h \
    ../../Sources \
    ../../Sources/Logger/stdlogger.h \
    ../../Sources/Logger/logger.h \
    ../../Sources/Slaves/pca9685.h

    ../../Sources/Interface/i2crpi.h

INCLUDEPATH += \
    /home/user/raspi/sysroot/usr/include \
   /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
    ../../Sources \
    ../../Sources/Interface \
    ../../Sources/Slaves\
    ../../Sources/Logger/

LIBS += -lssd13061



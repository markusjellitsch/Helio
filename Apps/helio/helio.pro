#-------------------------------------------------
#
# Project created by QtCreator 2018-09-15T10:01:29
#
#-------------------------------------------------

QT       -= gui

TARGET = helio
TEMPLATE = lib
CONFIG += staticlib
LIBS+=  -lwiringPi

SOURCES += \
    ../../Sources/Interface/uart_rpi.cpp \
    ../../Sources/Interface/spirpi.cpp \
    ../../Sources/Interface/rs485_rpi.cpp \
    ../../Sources/Interface/i2crpi.cpp \
    ../../Sources/Logger/logger.cpp \
    ../../Sources/Logger/stdlogger.cpp \
    ../../Sources/Heti/heti.cpp \
    ../../Sources/Modbus/modbusrtu.cpp \
    ../../Sources/Modbus/holdingregister.cpp \
    ../../Sources/TestUnit/test_vtx.cpp \
    ../../Sources/TestUnit/test_unit.cpp \
    ../../Sources/TestUnit/test_runner.cpp \
    ../../Sources/TestUnit/test_rgb.cpp \
    ../../Sources/TestUnit/test_fu.cpp \
    ../../Sources/Slaves/webex.cpp \
    ../../Sources/Slaves/rtu.cpp \
    ../../Sources/Console/console.cpp \
    ../../Sources/TestUnit/test_pwm.cpp

HEADERS += \
    ../../Sources/Interface/uart_rpi.h \
    ../../Sources/Interface/spirpi.h \
    ../../Sources/Interface/rs485_rpi.h \
    ../../Sources/Interface/interface.h \
    ../../Sources/Interface/i2crpi.h \
    ../../Sources/Logger/stdlogger.h \
    ../../Sources/Logger/logger.h \
    ../../Sources/Heti/heti.h \
    ../../Sources/Modbus/modbusrtu.h \
    ../../Sources/Modbus/holdingregister.h \
    ../../Sources/helio.h \
    ../../Sources/TestUnit/test_vtx.h \
    ../../Sources/TestUnit/test_unit.h \
    ../../Sources/TestUnit/test_runner.h \
    ../../Sources/TestUnit/test_rgb.h \
    ../../Sources/TestUnit/test_fu.h \
    ../../Sources/Slaves/webex.h \
    ../../Sources/Slaves/rtu.h \
    ../../Sources/Console/console.h \
    ../../Sources/TestUnit/test_pwm.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += /home/user/raspi/sysroot/usr/include \
            /home/user/RPI_COM/Apps/SSD13008Test \
            /home/user/raspi/sysroot/usr/include \
            /home/user/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/include/c++/4.8.3 \
            ../../Sources \
            ../../Sources/Interface \
            ../../Sources/Slaves\
            ../../Sources/Logger/




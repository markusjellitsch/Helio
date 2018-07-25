QT += core
QT -= gui

CONFIG += c++11

TARGET = FirstTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/TestUnit/test_fu.cpp \
    ../../Sources/TestUnit/test_runner.cpp \
    ../../Sources/TestUnit/test_unit.cpp \
    ../../Sources/Logger/logger.cpp \
    ../../Sources/Logger/stdlogger.cpp

HEADERS += \
    ../../Sources/TestUnit/test_fu.h \
    ../../Sources/TestUnit/test_runner.h \
    ../../Sources/TestUnit/test_unit.h \
    ../../Sources/Logger/logger.h \
    ../../Sources/Logger/stdlogger.h

INCLUDEPATH += ../../Sources\
            ../../Sources/Interface \
            ../../Sources/Slaves\
            ../../Sources/Logger\
            ../../Sources/TestUnit

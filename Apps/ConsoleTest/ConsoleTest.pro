QT += core
QT -= gui

CONFIG += c++11

TARGET = ConsoleTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Sources/Console/console.cpp

HEADERS += \
    ../../Sources/Console/console.h


INCLUDEPATH +=../../Sources/Console

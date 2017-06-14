QT += core sql serialbus serialport widgets
QT -= gui

CONFIG += c++11

TARGET = Monita_Modbus_Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    controller/worker.cpp \
    model/init_mysql.cpp \
    util/config.cpp

HEADERS += \
    controller/worker.h \
    util/utama.h \
    model/init_mysql.h \
    util/config.h

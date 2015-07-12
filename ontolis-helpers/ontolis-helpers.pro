#-------------------------------------------------
#
# Project created by QtCreator 2014-07-19T20:11:38
#
#-------------------------------------------------

QT       += core network widgets

TARGET = ontolis-helpers
TEMPLATE = lib
CONFIG += staticlib c++11
CONFIG -= debug_and_release_target
INCLUDEPATH  += ../

INCLUDEPATH += \
    ../../boost_1_56_0 \
    ../ontolis-lib \
    ../ontolis-repo

SOURCES += \
    DataControllerHelpers.cpp \
    GraphHelpers.cpp \
    IOHelpers.cpp

HEADERS += \
    DataControllerHelpers.hpp \
    GraphHelpers.hpp \
    IOHelpers.hpp

QMAKE_CXXFLAGS += -isystem ../../boost_1_56_0

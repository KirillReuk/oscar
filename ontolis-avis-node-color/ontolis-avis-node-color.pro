#-------------------------------------------------
#
# Project created by QtCreator 2013-12-26T21:11:32
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ontolis-avis-node-color
TEMPLATE = lib
CONFIG += c++11
CONFIG -= debug_and_release_target

DEFINES += ONTOLISAVISNODECOLOR_LIBRARY

SOURCES += extensions/OLSAdaptiveNodeColorVisualizer.cpp

INCLUDEPATH += ../ontolis-lib

HEADERS += extensions/OLSAdaptiveNodeColorVisualizer.h\
    extensions/ontolis-avis-node-color_global.h \
    extensions/OLSAdaptiveNodeColorVisualizerConstants.h

LIBS += -L$$OUT_PWD/../ontolis-lib -lontolis-lib

PRE_TARGETDEPS += FORCE \
    $$OUT_PWD/../ontolis-lib/libontolis-lib.a

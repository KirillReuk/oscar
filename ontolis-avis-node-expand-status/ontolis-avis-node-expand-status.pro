#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T15:28:14
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ontolis-avis-node-expand-status
TEMPLATE = lib
CONFIG += c++11
CONFIG -= debug_and_release_target

DEFINES += ONTOLISAVISNODEEXPANDSTATUS_LIBRARY

SOURCES += extensions/OLSAdaptiveNodeExpandStatusVisualizer.cpp

INCLUDEPATH += ../ontolis-lib \
    ../ontolis-avis-node-color \
    ../ontolis-avis-node-textarea

HEADERS += extensions/OLSAdaptiveNodeExpandStatusVisualizer.h \
    extensions/OLSAdaptiveNodeExpandStatusVisualizerConstants.h \
    extensions/ontolis-avis-node-expand-status_global.h

LIBS += -L$$OUT_PWD/../ontolis-lib -lontolis-lib

PRE_TARGETDEPS += FORCE \
    $$OUT_PWD/../ontolis-lib/libontolis-lib.a

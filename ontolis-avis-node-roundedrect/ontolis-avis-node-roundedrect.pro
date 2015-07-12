#-------------------------------------------------
#
# Project created by QtCreator 2013-12-04T20:52:24
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ontolis-avis-node-roundedrect
TEMPLATE = lib
CONFIG += c++11
CONFIG -= debug_and_release_target

DEFINES += ONTOLISAVISNODEROUNDEDRECT_LIBRARY

SOURCES += extensions/OLSAdaptiveNodeRoundedRectVisualizer.cpp

INCLUDEPATH += ../ontolis-lib \
    ../ontolis-avis-node-color \
    ../ontolis-avis-node-textarea \
    ../ontolis-avis-node-expand-status

HEADERS += \
    extensions/ontolis-avis-node-roundedrect_global.h \
    extensions/OLSAdaptiveNodeRoundedRectVisualizer.h

LIBS += -L$$OUT_PWD/../ontolis-lib -lontolis-lib

PRE_TARGETDEPS += FORCE \
    $$OUT_PWD/../ontolis-lib/libontolis-lib.a

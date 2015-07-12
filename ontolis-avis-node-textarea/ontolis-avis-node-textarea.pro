#-------------------------------------------------
#
# Project created by QtCreator 2013-12-04T14:12:15
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ontolis-avis-node-textarea
TEMPLATE = lib
CONFIG += c++11
CONFIG -= debug_and_release_target

DEFINES += OLSADAPTIVENODETEXTAREAVISUALIZER_LIBRARY

SOURCES += extensions/OLSAdaptiveNodeTextAreaVisualizer.cpp

INCLUDEPATH += ../ontolis-lib \
    ../ontolis-avis-node-color

HEADERS += \
    extensions/OLSAdaptiveNodeTextAreaVisualizer.h \
    extensions/OLSAdaptiveNodeTextAreaVisualizerConstants.h \
    extensions/ontolis-avis-node-textarea_global.h

LIBS += -L$$OUT_PWD/../ontolis-lib -lontolis-lib

PRE_TARGETDEPS += FORCE \
    $$OUT_PWD/../ontolis-lib/libontolis-lib.a

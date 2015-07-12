#-------------------------------------------------
#
# Project created by QtCreator 2013-12-04T14:12:15
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ontolis-avis-node-ellipse
TEMPLATE = lib
CONFIG += c++11
CONFIG -= debug_and_release_target

DEFINES += ONTOLISAVISNODEELLIPSE_LIBRARY

SOURCES += extensions/OLSAdaptiveNodeEllipseVisualizer.cpp

INCLUDEPATH += ../ontolis-lib \
    ../ontolis-avis-node-color \
    ../ontolis-avis-node-textarea \
    ../ontolis-avis-node-expand-status

HEADERS += \
    extensions/OLSAdaptiveNodeEllipseVisualizer.h \
    extensions/ontolis-avis-node-ellipse_global.h

LIBS += -L$$OUT_PWD/../ontolis-lib -lontolis-lib

PRE_TARGETDEPS += FORCE \
    $$OUT_PWD/../ontolis-lib/libontolis-lib.a

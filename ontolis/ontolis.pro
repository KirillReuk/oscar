#-------------------------------------------------
#
# Project created by QtCreator 2013-10-13T21:36:39
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = ontolis
TEMPLATE = app
RC_FILE = resources/resources.rc
CONFIG += c++11
CONFIG -= debug_and_release_target
INCLUDEPATH  += ../

SOURCES += \
    main.cpp \
    OntolisWindow.cpp \
    widgets/ProjectTree/OLSProject.cpp \
    widgets/ProjectTree/OLSProjectFile.cpp \
    widgets/ProjectTree/OLSProjectFileCategory.cpp \
    widgets/OntologyTree/OLSOntologyTreeViewController.cpp \
    widgets/OntologyPallete/OLSOntologyPalleteWidget.cpp \
    widgets/OntologyGraph/OLSOntologyGraphWidget.cpp \
    widgets/OntologyGraph/OLSOntologyGraphRelationVisualizedLine.cpp \
    widgets/OntologyGraph/OLSOntologyGraphView.cpp

INCLUDEPATH += \
    ../../boost_1_56_0 \
    ../ontolis-lib \
    ../ontolis-helpers

HEADERS  += \
    OntolisWindow.h \
    widgets/ProjectTree/OLSProject.h \
    widgets/ProjectTree/OLSProjectFile.h \
    widgets/ProjectTree/OLSProjectFileCategory.h \
    widgets/OntologyTree/OLSOntologyTreeViewController.h \
    widgets/OntologyPallete/OLSOntologyPalleteWidget.h \
    widgets/OntologyGraph/OLSOntologyGraphWidget.h \
    widgets/OntologyGraph/OLSOntologyGraphView.h \
    widgets/OntologyGraph/OLSOntologyGraphRelationVisualizedLine.h

FORMS    += \
    OntolisWindow.ui \
    widgets/OntologyGraph/OLSOntologyGraphAttributeEditor.ui \
    widgets/OntologyGraph/OLSOntologyGraphWidget.ui \

LIBS += \
    -L$$OUT_PWD/../ontolis-lib -lontolis-lib \
    -L$$OUT_PWD/../ontolis-helpers -lontolis-helpers

PRE_TARGETDEPS += FORCE \
    $$OUT_PWD/../ontolis-lib/libontolis-lib.a \
    $$OUT_PWD/../ontolis-helpers/libontolis-helpers.a

TRANSLATIONS += ontolis_ru.ts

QMAKE_CXXFLAGS += -isystem ../../boost_1_56_0

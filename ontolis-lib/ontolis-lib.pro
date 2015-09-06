#-------------------------------------------------
#
# Project created by QtCreator 2013-12-04T14:45:58
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ontolis-lib
TEMPLATE = lib
CONFIG += staticlib c++11
CONFIG -= debug_and_release_target

SOURCES += \
    extensions/OLSAdaptiveNodeVisualizerMaster.cpp \
    extensions/OLSAdaptiveNodeVisualizer.cpp \
    widgets/Shared/OLSOntologyDataController.cpp \
    widgets/OntologyGraph/OLSOntologyGraphRelationItem.cpp \
    widgets/OntologyGraph/OLSOntologyGraphNodeItem.cpp \
    widgets/OntologyGraph/OLSOntologyGraphElement.cpp \
    OLSAdaptiveNodeVisualizerForm.cpp \
    extensions/OLSAdaptiveNodeVisualizerSignalForwarder.cpp \
    extensions/OLSAdaptiveRelationVisualizerMaster.cpp \
    extensions/OLSAdaptiveRelationVisualizerParameters.cpp \
    widgets/Shared/OLSHistoryRecord.cpp \
    widgets/Shared/OLSSettings.cpp

HEADERS += \
    helpers/Singleton.h \
    extensions/OLSAdaptiveVisualizeParameter.h \
    extensions/OLSAdaptiveNodeVisualizerMaster.h \
    extensions/OLSAdaptiveNodeVisualizer.h \
    extensions/OLSAdaptiveVisualizerData.h \
    widgets/Shared/OLSOntologyRelationData.h \
    widgets/Shared/OLSOntologyNodeData.h \
    widgets/Shared/OLSOntologyDataController.h \
    widgets/OntologyGraph/OLSOntologyGraphRelationItem.h \
    widgets/OntologyGraph/OLSOntologyGraphNodeItem.h \
    widgets/OntologyGraph/OLSOntologyGraphItemType.h \
    widgets/OntologyGraph/OLSOntologyGraphItemDataKey.h \
    widgets/OntologyGraph/OLSOntologyGraphElement.h \
    OLSAdaptiveNodeVisualizerForm.h \
    extensions/OLSAdaptiveNodeVisualizerSignalForwarder.h \
    extensions/OLSAdaptiveRelationVisualizerMaster.h \
    extensions/OLSAdaptiveRelationVisualizerParameters.h \
    widgets/Shared/OLSHistoryRecord.h \
    widgets/Shared/OLSOntologyData.h \
    widgets/Shared/OLSSettings.h

FORMS += \
    OLSAdaptiveNodeVisualizerForm.ui

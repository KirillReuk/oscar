#ifndef OLSADAPTIVEVISUALIZERDATA_H
#define OLSADAPTIVEVISUALIZERDATA_H

#include <QtCore>
#include "OLSAdaptiveVisualizeParameter.h"
#include "widgets/Shared/OLSOntologyNodeData.h"
#include "widgets/Shared/OLSOntologyDataController.h"


struct OLSAdaptiveVisualizerData {
    QString displayName;
    QString libraryPath;
    QString className;
    bool isDefault;
    QList<OLSAdaptiveVisualizeParameter> parameters;
};

struct OLSAdaptiveVisualizerDataAsNodes {
    OLSOntologyNodeData *displayNameNode;
    OLSOntologyNodeData *libraryPathNode;
    OLSOntologyNodeData *classNameNode;
    OLSOntologyDataController *dataController;
    bool isDefault;
    QList<OLSAdaptiveVisualizeParameter> parameters;

public:
    OLSAdaptiveVisualizerData* asPlain() {
        OLSAdaptiveVisualizerData *data = new OLSAdaptiveVisualizerData();
        data->displayName = displayNameNode->name;
        data->libraryPath = libraryPathNode->name;
        data->className = classNameNode->name;
        data->parameters = parameters;
        data->isDefault = isDefault;
        return data;
    }
};

#endif // OLSADAPTIVEVISUALIZERDATA_H

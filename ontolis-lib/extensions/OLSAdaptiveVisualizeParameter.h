#ifndef OLSADAPTIVEVISUALIZEPARAMETER_H
#define OLSADAPTIVEVISUALIZEPARAMETER_H

#include <QtCore>

enum OLSAdaptiveVisualizeParameterType
    {
        Double,
        Integer,
        String
    };

struct OLSAdaptiveVisualizeParameter
{    
    QString name;
    QString displayName;
    QString defaultValue;
};

#endif // OLSADAPTIVEVISUALIZEPARAMETER_H

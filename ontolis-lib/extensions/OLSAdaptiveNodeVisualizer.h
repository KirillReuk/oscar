#ifndef OLSADAPTIVENODEVISUALIZER_H
#define OLSADAPTIVENODEVISUALIZER_H

#include <QObject>
#include "widgets/OntologyGraph/OLSOntologyGraphNodeItem.h"
#include "widgets/OntologyGraph/OLSOntologyGraphRelationItem.h"
#include "OLSAdaptiveVisualizeParameter.h"
#include "OLSAdaptiveVisualizerData.h"
#include "OLSAdaptiveNodeVisualizerForm.h"
#include "OLSAdaptiveNodeVisualizerSignalForwarder.h"

class OLSAdaptiveNodeVisualizer
{

private:
    static const QString m_isDefaultStr;
    OLSAdaptiveVisualizerDataAsNodes m_data;
    OLSAdaptiveNodeVisualizerSignalForwarder *m_signalForwarder;

protected:
    static QString extractParameter(QString name, OLSOntologyGraphNodeItem *node);
    static double getParameterValueOrDefaultDouble(QString name, OLSOntologyGraphNodeItem *node, double defaultValue);
    static double getParameterValueOrDefaultInt(QString name, OLSOntologyGraphNodeItem *node, int defaultValue);
    static QRectF enlargeRect(QRectF rect, double num);

public:
    explicit OLSAdaptiveNodeVisualizer();
    virtual ~OLSAdaptiveNodeVisualizer();
    virtual bool tryPerformAdaptableVisualization(
            OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo,
            QPainter *painter) = 0;
    virtual QStringList getDependencies() = 0;
    virtual void setDependency(QString name, OLSAdaptiveNodeVisualizer *visualizer) = 0;
    void setData(OLSAdaptiveVisualizerDataAsNodes data);
    OLSAdaptiveVisualizerData *data();
    QString displayName();
    QString libraryPath();
    QString className();
    bool isDefault();
    void setDefault();
    void setDisplayName(QString displayName);
    void setLibraryPath(QString libraryPath);
    void setClassName(QString className);
    void showForm();
    OLSAdaptiveNodeVisualizerSignalForwarder* signalForwarder();
    virtual void loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params) = 0;

};

#endif // OLSADAPTIVENODEVISUALIZER_H

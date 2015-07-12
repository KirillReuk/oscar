#ifndef OLSADAPTIVENODEEXPANDSTATUSVISUALIZER_H
#define OLSADAPTIVENODEEXPANDSTATUSVISUALIZER_H

#include "ontolis-avis-node-expand-status_global.h"
#include "extensions/OLSAdaptiveNodeExpandStatusVisualizerConstants.h"
#include "extensions/OLSAdaptiveNodeVisualizer.h"

class ONTOLISAVISNODEEXPANDSTATUSSHARED_EXPORT OLSAdaptiveNodeExpandStatusVisualizer : public OLSAdaptiveNodeVisualizer
{
private:    
    static const QString m_plusSizeStr;
    int m_plusSize = 4;
    QList<OLSAdaptiveNodeVisualizer *> m_textAreaVisualizers;

public:
    OLSAdaptiveNodeExpandStatusVisualizer();
    virtual bool tryPerformAdaptableVisualization(
            OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo,
            QPainter *painter) override;
    virtual void loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params) override;
    virtual QStringList getDependencies() override;
    virtual void setDependency(QString name, OLSAdaptiveNodeVisualizer *visualizer) override;
};

Q_DECLARE_METATYPE(OLSAdaptiveNodeExpandStatusVisualizer)

extern "C" {
ONTOLISAVISNODEEXPANDSTATUSSHARED_EXPORT void initVisualizer();
}

#endif // OLSADAPTIVENODEEXPANDSTATUSVISUALIZER_H

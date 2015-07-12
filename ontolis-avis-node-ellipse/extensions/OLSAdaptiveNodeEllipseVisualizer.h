#ifndef ONTOLISAVISNODEELLIPSE_H
#define ONTOLISAVISNODEELLIPSE_H

#include "ontolis-avis-node-ellipse_global.h"
#include "extensions/OLSAdaptiveNodeVisualizer.h"

class ONTOLISAVISNODEELLIPSESHARED_EXPORT OLSAdaptiveNodeEllipseVisualizer : public OLSAdaptiveNodeVisualizer
{
private:
    QList<OLSAdaptiveNodeVisualizer *> m_colorVisualizers, m_textAreaVisualizers, m_expandStateVisualizers;

public:
    OLSAdaptiveNodeEllipseVisualizer();
    virtual bool tryPerformAdaptableVisualization(
            OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo,
            QPainter *painter) override;
    virtual void loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params) override;
    virtual QStringList getDependencies() override;
    virtual void setDependency(QString name, OLSAdaptiveNodeVisualizer *visualizer) override;
};

Q_DECLARE_METATYPE(OLSAdaptiveNodeEllipseVisualizer)

extern "C" {
ONTOLISAVISNODEELLIPSESHARED_EXPORT void initVisualizer();
}

#endif // ONTOLISAVISNODEELLIPSE_H

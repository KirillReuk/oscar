#ifndef OLSADAPTIVENODEROUNDEDRECTVISUALIZER_H
#define OLSADAPTIVENODEROUNDEDRECTVISUALIZER_H

#include "ontolis-avis-node-roundedrect_global.h"
#include "extensions/OLSAdaptiveNodeVisualizer.h"

class ONTOLISAVISNODEROUNDEDRECTSHARED_EXPORT OLSAdaptiveNodeRoundedRectVisualizer : public OLSAdaptiveNodeVisualizer
{
private:
    static const int m_init;
    static const QString m_xRadiusStr, m_yRadiusStr;
    double m_xRadius, m_yRadius;
    QList<OLSAdaptiveNodeVisualizer *> m_colorVisualizers, m_textAreaVisualizers, m_expandStateVisualizers;

public:
    OLSAdaptiveNodeRoundedRectVisualizer();
    virtual bool tryPerformAdaptableVisualization(
            OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo,
            QPainter *painter) override;
    virtual void loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params) override;
    virtual QStringList getDependencies() override;
    virtual void setDependency(QString name, OLSAdaptiveNodeVisualizer *visualizer) override;
};

Q_DECLARE_METATYPE(OLSAdaptiveNodeRoundedRectVisualizer)

extern "C" {
ONTOLISAVISNODEROUNDEDRECTSHARED_EXPORT void initVisualizer();
}

#endif // OLSADAPTIVENODEROUNDEDRECTVISUALIZER_H

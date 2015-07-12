#ifndef OLSADAPTIVENODECOLORVISUALIZER_H
#define OLSADAPTIVENODECOLORVISUALIZER_H

#include "ontolis-avis-node-color_global.h"
#include "OLSAdaptiveNodeColorVisualizerConstants.h"
#include "extensions/OLSAdaptiveNodeVisualizer.h"

class ONTOLISAVISNODECOLORSHARED_EXPORT OLSAdaptiveNodeColorVisualizer: public OLSAdaptiveNodeVisualizer
{
private:
    static const int m_init;
    static const QString m_redStr, m_greenStr, m_blueStr, m_alphaStr;
    double m_red, m_green, m_blue, m_alpha;
public:
    OLSAdaptiveNodeColorVisualizer();
    virtual bool tryPerformAdaptableVisualization(
            OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo,
            QPainter *painter) override;
    virtual void loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params) override;
    virtual QStringList getDependencies() override;
    virtual void setDependency(QString name, OLSAdaptiveNodeVisualizer *visualizer) override;
};

Q_DECLARE_METATYPE(OLSAdaptiveNodeColorVisualizer)

extern "C" {
ONTOLISAVISNODECOLORSHARED_EXPORT void initVisualizer();
}

#endif // OLSADAPTIVENODECOLORVISUALIZER_H

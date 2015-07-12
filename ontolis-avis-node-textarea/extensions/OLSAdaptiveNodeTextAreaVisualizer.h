#ifndef OLSADAPTIVENODETEXTAREAVISUALIZER_H
#define OLSADAPTIVENODETEXTAREAVISUALIZER_H

#include "ontolis-avis-node-textarea_global.h"
#include "extensions/OLSAdaptiveNodeVisualizer.h"
#include "extensions/OLSAdaptiveNodeVisualizerMaster.h"
#include "extensions/OLSAdaptiveNodeTextAreaVisualizerConstants.h"

class OLSADAPTIVENODETEXTAREAVISUALIZERSHARED_EXPORT OLSAdaptiveNodeTextAreaVisualizer : public OLSAdaptiveNodeVisualizer
{
private:    
    static const QString m_offsetStr;
    double m_offset;
    QList<OLSAdaptiveNodeVisualizer *> m_colorVisualizers;

public:
    OLSAdaptiveNodeTextAreaVisualizer();
    virtual bool tryPerformAdaptableVisualization(
            OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo,
            QPainter *painter) override;
    virtual void loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params) override;
    virtual QStringList getDependencies() override;
    virtual void setDependency(QString name, OLSAdaptiveNodeVisualizer *visualizer) override;
};

Q_DECLARE_METATYPE(OLSAdaptiveNodeTextAreaVisualizer)

extern "C" {
OLSADAPTIVENODETEXTAREAVISUALIZERSHARED_EXPORT void initVisualizer();
}

#endif // OLSADAPTIVENODETEXTAREAVISUALIZER_H

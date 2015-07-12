#include "OLSAdaptiveNodeEllipseVisualizer.h"
#include "extensions/OLSAdaptiveNodeVisualizerMaster.h"
#include "extensions/OLSAdaptiveNodeColorVisualizerConstants.h"
#include "extensions/OLSAdaptiveNodeTextAreaVisualizerConstants.h"
#include "extensions/OLSAdaptiveNodeExpandStatusVisualizerConstants.h"
#include <QDebug>

void initVisualizer()
{
    qRegisterMetaType<OLSAdaptiveNodeEllipseVisualizer>();
}

OLSAdaptiveNodeEllipseVisualizer::OLSAdaptiveNodeEllipseVisualizer() { }

bool OLSAdaptiveNodeEllipseVisualizer::tryPerformAdaptableVisualization(OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *, QPainter *painter)
{
    if (nodeToVisualize == nullptr || painter == nullptr)
        return false;
    if (!OLSAdaptiveNodeVisualizerMaster::stepOfAdaptableVisualization(m_textAreaVisualizers, nodeToVisualize, painter))
        nodeToVisualize->setRect(nodeToVisualize->textRect());
    OLSAdaptiveNodeVisualizerMaster::stepOfAdaptableVisualization(m_expandStateVisualizers, nodeToVisualize, painter);
    OLSAdaptiveNodeVisualizerMaster::stepOfAdaptableVisualization(m_colorVisualizers, nodeToVisualize, painter);
    QColor backgroundColor = nodeToVisualize->getAndRemoveVisualizeParameter(OLS_AVIS_NODE_COLOR__COLOR, QColor(100, 100, 100)).value<QColor>();
    painter->setBrush(QBrush(backgroundColor));
    painter->drawEllipse(nodeToVisualize->rect());
    qDebug() << "OLSAdaptiveNodeEllipseVisualizer";
    return true;
}

void OLSAdaptiveNodeEllipseVisualizer::loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter>) { }

QStringList OLSAdaptiveNodeEllipseVisualizer::getDependencies()
{
    return QStringList()
            << OLS_AVIS_NODE_COLOR__VISUALIZER_CLASS
            << OLS_AVIS_NODE_TEXTAREA__VISUALIZER_CLASS
            << OLS_AVIS_NODE_EXPAND_STATUS__VISUALIZER_CLASS;
}

void OLSAdaptiveNodeEllipseVisualizer::setDependency(QString name, OLSAdaptiveNodeVisualizer *visualizer)
{
    if (name == OLS_AVIS_NODE_COLOR__VISUALIZER_CLASS)
        m_colorVisualizers << visualizer;
    else if (name == OLS_AVIS_NODE_TEXTAREA__VISUALIZER_CLASS)
        m_textAreaVisualizers << visualizer;
    else if (name == OLS_AVIS_NODE_EXPAND_STATUS__VISUALIZER_CLASS)
        m_expandStateVisualizers << visualizer;
    else
        throw;
}

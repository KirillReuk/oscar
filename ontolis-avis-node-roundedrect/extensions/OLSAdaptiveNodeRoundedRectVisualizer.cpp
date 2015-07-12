#include "OLSAdaptiveNodeRoundedRectVisualizer.h"
#include "extensions/OLSAdaptiveNodeVisualizerMaster.h"
#include "extensions/OLSAdaptiveNodeColorVisualizerConstants.h"
#include "extensions/OLSAdaptiveNodeTextAreaVisualizerConstants.h"
#include "extensions/OLSAdaptiveNodeExpandStatusVisualizerConstants.h"
#include <QDebug>

void initVisualizer()
{
    qRegisterMetaType<OLSAdaptiveNodeRoundedRectVisualizer>();
}

OLSAdaptiveNodeRoundedRectVisualizer::OLSAdaptiveNodeRoundedRectVisualizer() { }

const QString OLSAdaptiveNodeRoundedRectVisualizer::m_xRadiusStr = "xRadius";
const QString OLSAdaptiveNodeRoundedRectVisualizer::m_yRadiusStr = "yRadius";

bool OLSAdaptiveNodeRoundedRectVisualizer::tryPerformAdaptableVisualization(OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo, QPainter *painter)
{
    if (nodeToVisualize == nullptr || painter == nullptr)
        return false;
    qreal xRadius = getParameterValueOrDefaultDouble(m_xRadiusStr, nodeWithVisualizeInfo, m_xRadius);
    qreal yRadius = getParameterValueOrDefaultDouble(m_yRadiusStr, nodeWithVisualizeInfo, m_yRadius);
    if (!OLSAdaptiveNodeVisualizerMaster::stepOfAdaptableVisualization(m_textAreaVisualizers, nodeToVisualize, painter))
        nodeToVisualize->setRect(nodeToVisualize->textRect());
    OLSAdaptiveNodeVisualizerMaster::stepOfAdaptableVisualization(m_expandStateVisualizers, nodeToVisualize, painter);
    OLSAdaptiveNodeVisualizerMaster::stepOfAdaptableVisualization(m_colorVisualizers, nodeToVisualize, painter);
    QColor backgroundColor = nodeToVisualize->getAndRemoveVisualizeParameter(OLS_AVIS_NODE_COLOR__COLOR, QColor(100, 100, 100)).value<QColor>();
    painter->setBrush(QBrush(backgroundColor));
    qDebug() << nodeToVisualize->rect();
    painter->drawRoundedRect(nodeToVisualize->rect(), xRadius, yRadius);
    qDebug() << "OLSAdaptiveNodeRoundedRectVisualizer";
    return true;
}

void OLSAdaptiveNodeRoundedRectVisualizer::loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params)
{
    foreach (OLSAdaptiveVisualizeParameter param, params) {
        bool ok = false;
        if (param.name == m_xRadiusStr) {
            double tmp = param.defaultValue.toDouble(&ok);
            m_xRadius = ok ? tmp : 0;
        }
        else if (param.name == m_yRadiusStr) {
            double tmp = param.defaultValue.toDouble(&ok);
            m_yRadius = ok ? tmp : 0;
        }
    }
}

QStringList OLSAdaptiveNodeRoundedRectVisualizer::getDependencies()
{
    return QStringList()
            << OLS_AVIS_NODE_COLOR__VISUALIZER_CLASS
            << OLS_AVIS_NODE_TEXTAREA__VISUALIZER_CLASS
            << OLS_AVIS_NODE_EXPAND_STATUS__VISUALIZER_CLASS;
}

void OLSAdaptiveNodeRoundedRectVisualizer::setDependency(QString name, OLSAdaptiveNodeVisualizer *visualizer)
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

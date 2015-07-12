#include "OLSAdaptiveNodeTextAreaVisualizer.h"
#include <QDebug>

void initVisualizer()
{
    qRegisterMetaType<OLSAdaptiveNodeTextAreaVisualizer>(OLS_AVIS_NODE_TEXTAREA__VISUALIZER_CLASS);
}

OLSAdaptiveNodeTextAreaVisualizer::OLSAdaptiveNodeTextAreaVisualizer() { }

const QString OLSAdaptiveNodeTextAreaVisualizer::m_offsetStr = "offset";

bool OLSAdaptiveNodeTextAreaVisualizer::tryPerformAdaptableVisualization(OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo, QPainter *)
{
    if (nodeToVisualize == nullptr)
        return false;
    QRectF rect = enlargeRect(nodeToVisualize->textRect(), getParameterValueOrDefaultDouble(m_offsetStr, nodeWithVisualizeInfo, m_offset));
    if (!rect.isNull())
        nodeToVisualize->setRect(rect);
    else
        nodeToVisualize->setRect(nodeToVisualize->textRect());
    qDebug() << "OLSAdaptiveNodeTextAreaVisualizer";
    return true;
}

void OLSAdaptiveNodeTextAreaVisualizer::loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params)
{
    foreach (OLSAdaptiveVisualizeParameter param, params) {
        bool ok = false;
        if (param.name == m_offsetStr) {
            double tmp = param.defaultValue.toDouble(&ok);
            m_offset = ok ? tmp : 0;
        }
    }
}

QStringList OLSAdaptiveNodeTextAreaVisualizer::getDependencies()
{
    return QStringList();
}

void OLSAdaptiveNodeTextAreaVisualizer::setDependency(QString, OLSAdaptiveNodeVisualizer *)
{
    throw;
}

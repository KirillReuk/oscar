#include "OLSAdaptiveNodeExpandStatusVisualizer.h"
#include "extensions/OLSAdaptiveNodeVisualizerMaster.h"
#include "extensions/OLSAdaptiveNodeColorVisualizerConstants.h"
#include "extensions/OLSAdaptiveNodeTextAreaVisualizerConstants.h"
#include <QDebug>

void initVisualizer()
{
    qRegisterMetaType<OLSAdaptiveNodeExpandStatusVisualizer>(OLS_AVIS_NODE_EXPAND_STATUS__VISUALIZER_CLASS);
}

OLSAdaptiveNodeExpandStatusVisualizer::OLSAdaptiveNodeExpandStatusVisualizer() { }

const QString OLSAdaptiveNodeExpandStatusVisualizer::m_plusSizeStr = "plusSize";

bool OLSAdaptiveNodeExpandStatusVisualizer::tryPerformAdaptableVisualization(OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo, QPainter *painter)
{
    if (nodeToVisualize == nullptr || painter == nullptr)
        return false;
    int plusSize = getParameterValueOrDefaultInt(m_plusSizeStr, nodeWithVisualizeInfo, m_plusSize);
    OLSAdaptiveNodeVisualizerMaster::stepOfAdaptableVisualization(m_textAreaVisualizers, nodeToVisualize, painter);
    if (nodeToVisualize->expandState() != ExpandState::NotExpandable) {
        QPainterPath path;
        QRectF rect = nodeToVisualize->rect();
        QPointF mid(rect.x() - rect.height() / 2, rect.y() + rect.height() / 2);
        if (nodeToVisualize->expandState() == ExpandState::NotExpanded) {
            path.moveTo(mid - QPointF(0, -plusSize));
            path.lineTo(mid - QPointF(0, plusSize));
        }
        path.moveTo(mid - QPointF(-plusSize, 0));
        path.lineTo(mid - QPointF(plusSize, 0));

        QPen pen;
        pen.setWidthF(2.0);
        QPen oldPen = painter->pen();
        painter->setPen(pen);
        painter->drawPath(path);
        painter->setPen(oldPen);
    }
    qDebug() << OLS_AVIS_NODE_EXPAND_STATUS__VISUALIZER_CLASS;
    return true;
}

void OLSAdaptiveNodeExpandStatusVisualizer::loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params)
{
    foreach (OLSAdaptiveVisualizeParameter param, params) {
        bool ok = false;
        if (param.name == m_plusSizeStr) {
            int tmp = param.defaultValue.toInt(&ok);
            m_plusSize = ok ? tmp : 0;
        }
    }
}

QStringList OLSAdaptiveNodeExpandStatusVisualizer::getDependencies()
{
    return QStringList() << OLS_AVIS_NODE_TEXTAREA__VISUALIZER_CLASS;
}

void OLSAdaptiveNodeExpandStatusVisualizer::setDependency(QString name, OLSAdaptiveNodeVisualizer *visualizer)
{
    if (name == OLS_AVIS_NODE_TEXTAREA__VISUALIZER_CLASS)
        m_textAreaVisualizers << visualizer;
    else
        throw;
}

#include "OLSAdaptiveNodeColorVisualizer.h"
#include <QtGlobal>
#include <QDebug>

void initVisualizer()
{
    qRegisterMetaType<OLSAdaptiveNodeColorVisualizer>(OLS_AVIS_NODE_COLOR__VISUALIZER_CLASS);
}

OLSAdaptiveNodeColorVisualizer::OLSAdaptiveNodeColorVisualizer() { }

const QString OLSAdaptiveNodeColorVisualizer::m_redStr = "R";
const QString OLSAdaptiveNodeColorVisualizer::m_greenStr = "G";
const QString OLSAdaptiveNodeColorVisualizer::m_blueStr = "B";
const QString OLSAdaptiveNodeColorVisualizer::m_alphaStr = "A";

bool OLSAdaptiveNodeColorVisualizer::tryPerformAdaptableVisualization( OLSOntologyGraphNodeItem *nodeToVisualize,
            OLSOntologyGraphNodeItem *nodeWithVisualizeInfo, QPainter *)
{
    if (nodeToVisualize == nullptr)
        return false;
    int red = qRound(getParameterValueOrDefaultDouble(m_redStr, nodeWithVisualizeInfo, m_red));
    int green = qRound(getParameterValueOrDefaultDouble(m_greenStr, nodeWithVisualizeInfo, m_green));
    int blue = qRound(getParameterValueOrDefaultDouble(m_blueStr, nodeWithVisualizeInfo, m_blue));
    int alpha = qRound(getParameterValueOrDefaultDouble(m_alphaStr, nodeWithVisualizeInfo, m_alpha));
    nodeToVisualize->setVisualizeParameter(OLS_AVIS_NODE_COLOR__COLOR, QColor(red, green, blue, alpha));
    qDebug() << "OLSAdaptiveNodeColorVisualizer";
    return true;
}

void OLSAdaptiveNodeColorVisualizer::loadDefaultParameters(QList<OLSAdaptiveVisualizeParameter> params)
{
    foreach (OLSAdaptiveVisualizeParameter param, params) {
        bool ok = false;
        if (param.name == m_redStr) {
            double tmp = param.defaultValue.toDouble(&ok);
            m_red = ok ? tmp : 0;
        }
        else if (param.name == m_greenStr) {
            double tmp = param.defaultValue.toDouble(&ok);
            m_green = ok ? tmp : 0;
        }
        else if (param.name == m_blueStr) {
            double tmp = param.defaultValue.toDouble(&ok);
            m_blue = ok ? tmp : 0;
        }
        else if (param.name == m_alphaStr) {
            double tmp = param.defaultValue.toDouble(&ok);
            m_alpha = ok ? tmp : 255;
        }
    }
}

QStringList OLSAdaptiveNodeColorVisualizer::getDependencies()
{
    return QStringList();
}

void OLSAdaptiveNodeColorVisualizer::setDependency(QString, OLSAdaptiveNodeVisualizer *)
{
    throw;
}

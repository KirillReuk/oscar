#include "OLSAdaptiveNodeVisualizer.h"
#include "OLSAdaptiveNodeVisualizerMaster.h"

const QString OLSAdaptiveNodeVisualizer::m_isDefaultStr = "#isDefault";

OLSAdaptiveNodeVisualizer::OLSAdaptiveNodeVisualizer() {
    m_signalForwarder = new OLSAdaptiveNodeVisualizerSignalForwarder(this);
}

OLSAdaptiveNodeVisualizer::~OLSAdaptiveNodeVisualizer() {}

QString OLSAdaptiveNodeVisualizer::extractParameter(QString name, OLSOntologyGraphNodeItem *node) {
    if (node == nullptr)
        return "";
    QList<OLSOntologyGraphRelationItem *> relations = node->getOutgoingRelationsByName(name);
    return relations.size() != 1 ? "" : relations.first()->destinationNode()->name();
}

double OLSAdaptiveNodeVisualizer::getParameterValueOrDefaultDouble(QString name, OLSOntologyGraphNodeItem *node, double defaultValue) {
    QString paramValue;
    bool ok = false;
    double tmp = defaultValue;
    if ((paramValue = extractParameter(name, node)) != "") {
        tmp = paramValue.toDouble(&ok);
    }
    return ok ? tmp : defaultValue;
}

double OLSAdaptiveNodeVisualizer::getParameterValueOrDefaultInt(QString name, OLSOntologyGraphNodeItem *node, int defaultValue) {
    QString paramValue;
    bool ok = false;
    int tmp = defaultValue;
    if ((paramValue = extractParameter(name, node)) != "") {
        tmp = paramValue.toInt(&ok);
    }
    return ok ? tmp : defaultValue;
}

QRectF OLSAdaptiveNodeVisualizer::enlargeRect(QRectF rect, double num) {
    rect.adjust(-num, -num, num, num);
    return rect;
}

void OLSAdaptiveNodeVisualizer::setData(OLSAdaptiveVisualizerDataAsNodes data) {
    m_data = data;
    loadDefaultParameters(data.parameters);
}

OLSAdaptiveVisualizerData *OLSAdaptiveNodeVisualizer::data() {
    return m_data.asPlain();
}

void OLSAdaptiveNodeVisualizer::showForm() {
    OLSAdaptiveVisualizerData visualizerData = OLSAdaptiveNodeVisualizerForm::getVisualizerData(*data());
    setDisplayName(visualizerData.displayName);
    setClassName(visualizerData.className);
    setLibraryPath(visualizerData.libraryPath);
}

OLSAdaptiveNodeVisualizerSignalForwarder* OLSAdaptiveNodeVisualizer::signalForwarder() {
    return m_signalForwarder;
}

QString OLSAdaptiveNodeVisualizer::displayName() {
    return m_data.displayNameNode->name;
}

QString OLSAdaptiveNodeVisualizer::libraryPath() {
    return m_data.libraryPathNode->name;
}

QString OLSAdaptiveNodeVisualizer::className() {
    return m_data.classNameNode->name;
}

void OLSAdaptiveNodeVisualizer::setDisplayName(QString displayName) {
    m_data.dataController->nodeNameChanged(m_data.displayNameNode->id, "", displayName);
}

void OLSAdaptiveNodeVisualizer::setLibraryPath(QString libraryPath) {
    m_data.dataController->nodeNameChanged(m_data.libraryPathNode->id, "", libraryPath);
}

void OLSAdaptiveNodeVisualizer::setClassName(QString className) {
    m_data.dataController->nodeNameChanged(m_data.classNameNode->id, "", className);
}

bool OLSAdaptiveNodeVisualizer::isDefault() {
    return m_data.isDefault;
}

void OLSAdaptiveNodeVisualizer::setDefault() {
    foreach (OLSOntologyNodeData *mainNode, m_data.dataController->getRelatedOutgoingNodes("", "nodeAvailableVisualisers").values()) {
        QMap<OLSOntologyRelationData *, OLSOntologyNodeData *> kvps = m_data.dataController->getRelatedOutgoingNodes(mainNode->id);
        foreach (OLSOntologyRelationData *relation, kvps.keys()) {
            if (relation->name == m_isDefaultStr) {
                    m_data.dataController->nodeRemoved(relation->destinationNodeId);
            }
        }
    }
    long nodeId = m_data.dataController->nodeCreated();
    m_data.dataController->nodeNameChanged(nodeId, "", "true");
    long relationId = m_data.dataController->relationCreated(m_data.displayNameNode->id, nodeId);
    m_data.dataController->relationNameChanged(relationId, "", m_isDefaultStr);
}

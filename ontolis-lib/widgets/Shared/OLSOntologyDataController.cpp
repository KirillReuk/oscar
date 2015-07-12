#include "OLSOntologyDataController.h"

#include <QVariantMap>

OLSOntologyDataController::OLSOntologyDataController() {

  m_lastId = 0;
  m_namespaces["default"] = "http://knova.ru/user/" + QString::number(QDateTime::currentMSecsSinceEpoch());

  m_changedNodeIds = QSet<long>();
  m_changedRelationIds = QSet<long>();
}

OLSOntologyDataController::OLSOntologyDataController(const QVariant &json) {

  deserialize(json);

  m_changedNodeIds = QSet<long>();
  m_changedRelationIds = QSet<long>();
}

//OLSOntologyDataController::OLSOntologyDataController(QList<OLSOntologyNodeData *> nodeList,
//                                               QList<OLSOntologyRelationData *> relationList,
//                                               QMap<long, QPointF> nodePositions,
//                                               QSet<long> changedNodeIds,
//                                               QSet<long> changedRelationIds) {

//  m_lastId = -1;

//  foreach (OLSOntologyNodeData *nodeData, nodeList) {
//    m_nodesMap.insert(nodeData->id, nodeData);
//    m_nodesList.append(nodeData);
//    m_lastId = std::max(m_lastId, nodeData->id);
//  }

//  foreach (OLSOntologyRelationData *relationData, relationList) {
//    m_relationsMap.insert(relationData->id, relationData);
//    m_relationsList.append(relationData);
//    m_relationsMapByNodes.insert(QPair<long, long>(relationData->sourceNodeId, relationData->destinationNodeId), relationData);
//    m_lastId = std::max(m_lastId, relationData->id);
//  }

//  m_nodePositions = nodePositions;
//  m_changedNodeIds = changedNodeIds;
//  m_changedRelationIds = changedRelationIds;

//  qDebug() << "Snapshot created: " << m_changedNodeIds << "\n" << m_changedRelationIds;
//}

void OLSOntologyDataController::setVisualizeOntPath(const QString &visualizeOntPath) {

  m_visualizeOntPath = visualizeOntPath;
}

void OLSOntologyDataController::normalize() {

  double minX = INFINITY;
  double minY = INFINITY;

  foreach (long nodeId, m_nodePositions.keys()) {
    QPointF position = m_nodePositions.value(nodeId);
    minX = qMin(position.x(), minX);
    minY = qMin(position.y(), minY);
  }

  minX -= 200;
  minY -= 200;

  foreach (long nodeId, m_nodePositions.keys()) {
    QPointF position = m_nodePositions.value(nodeId);
    position.setX(position.x() - minX);
    position.setY(position.y() - minY);
    m_nodePositions.insert(nodeId, position);
  }
}

QVariant OLSOntologyDataController::serialize() {

  m_jsonMap["last_id"] = QVariant::fromValue(m_lastId);
  m_jsonMap["visualize_ont_path"] = QVariant::fromValue(m_visualizeOntPath);

  QVariantMap namespacesAsVariants;
  for (QString prefix : m_namespaces.keys()) {
      namespacesAsVariants[prefix] = m_namespaces.value(prefix);
  }
  QMap<QString, QString> defaultNamespacesCache = defaultNamespaces();
  for (QString prefix : defaultNamespacesCache.keys()) {
      namespacesAsVariants[prefix] = defaultNamespacesCache.value(prefix);
  }
  m_jsonMap["namespaces"] = namespacesAsVariants;

  QVariantList nodes;
  foreach (OLSOntologyNodeData *nodeData, m_nodesList) {
    QVariantMap node;
    node["id"] = QVariant::fromValue(nodeData->id);
    node["name"] = nodeData->name;
    node["namespace"] = nodeData->namespace_;
    node["position_x"] = QVariant::fromValue(nodePosition(nodeData->id).x());
    node["position_y"] = QVariant::fromValue(nodePosition(nodeData->id).y());
    node["attributes"] = nodeData->attributes;
    nodes.append(node);
  }
  m_jsonMap["nodes"] = nodes;

  QVariantList relations;
  foreach (OLSOntologyRelationData *relationData, m_relationsList) {
    QVariantMap relation;
    relation["id"] = QVariant::fromValue(relationData->id);
    relation["name"] = relationData->name;
    relation["namespace"] = relationData->namespace_;
    relation["source_node_id"] = QVariant::fromValue(relationData->sourceNodeId);
    relation["destination_node_id"] = QVariant::fromValue(relationData->destinationNodeId);
    relation["attributes"] = relationData->attributes;
    relations.append(relation);
  }
  m_jsonMap["relations"] = relations;

  return m_jsonMap;
}

void OLSOntologyDataController::deserialize(const QVariant &json) {

  qDebug() << "Ontology data controller deserialization";

  m_jsonMap = json.toMap();

  qDebug() << m_jsonMap;

  m_lastId = m_jsonMap["last_id"].toLongLong();
  m_visualizeOntPath = m_jsonMap["visualize_ont_path"].toString();

  QVariantMap namespacesAsVariants = m_jsonMap["namespaces"].toMap();
  QMap<QString, QString> newNamespaces;
  QMap<QString, QString> defaultNamespacesCache = defaultNamespaces();
  for (QString prefix : namespacesAsVariants.keys()) {
      if (!defaultNamespacesCache.contains(prefix))
        newNamespaces[prefix] = namespacesAsVariants.value(prefix).toString();
  }
  setNamespaces(newNamespaces);

  QVariantList nodes = m_jsonMap["nodes"].toList();
  foreach (QVariant node, nodes) {
    QVariantMap nodeMap = node.toMap();

    qDebug() << nodeMap;

    OLSOntologyNodeData *nodeData = new OLSOntologyNodeData();
    nodeData->id = nodeMap["id"].toLongLong();
    nodeData->name = nodeMap["name"].toString();
    nodeData->namespace_ = nodeMap["namespace"].toString();
    if (nodeMap.contains("attributes")) {
      nodeData->attributes = nodeMap["attributes"].toMap();
    }

    if (nodeMap.contains("position_x") && nodeMap.contains("position_y")) {
      double x = nodeMap["position_x"].toDouble();
      double y = nodeMap["position_y"].toDouble();
      setNodePosition(nodeData->id, QPointF(x, y));
    }
    m_nodesMap.insert(nodeData->id, nodeData);
    m_nodesList.append(nodeData);
  }

  QVariantList relations = m_jsonMap["relations"].toList();
  foreach (QVariant relation, relations) {
    QVariantMap relationMap = relation.toMap();
    OLSOntologyRelationData *relationData = new OLSOntologyRelationData();
    relationData->id = relationMap["id"].toLongLong();
    relationData->name = relationMap["name"].toString();
    relationData->namespace_ = relationMap["namespace"].toString();
    relationData->sourceNodeId = relationMap["source_node_id"].toLongLong();
    relationData->destinationNodeId = relationMap["destination_node_id"].toLongLong();
    if (relationMap.contains("attributes")) {
      relationData->attributes = relationMap["attributes"].toMap();
    }
    m_relationsMap.insert(relationData->id, relationData);
    m_relationsList.append(relationData);

    QPair<long, long> relationNodesPair(relationData->sourceNodeId, relationData->destinationNodeId);
    m_relationsMapByNodes.insert(relationNodesPair, relationData);

    OLSOntologyNodeData *sourceNode = m_nodesMap.value(relationData->sourceNodeId);
    sourceNode->relations.append(relationData->id);

    OLSOntologyNodeData *destinationNode = m_nodesMap.value(relationData->destinationNodeId);
    destinationNode->relations.append(relationData->id);
  }

  normalize();
}

// data source

int OLSOntologyDataController::nodeCount() {

  return m_nodesList.count();
}

int OLSOntologyDataController::relationCount() {

  return m_relationsList.count();
}

OLSOntologyNodeData *OLSOntologyDataController::getNodeByIndex(int index, bool asChanges) {

  OLSOntologyNodeData *nodeData = m_nodesList.at(index);
  if (asChanges) {
    m_changedNodeIds.insert(nodeData->id);
  }
  return nodeData;
}

OLSOntologyRelationData *OLSOntologyDataController::getRelationByIndex(int index, bool asChanges) {

  OLSOntologyRelationData *relationData = m_relationsList.at(index);
  if (asChanges) {
    m_changedRelationIds.insert(relationData->id);
  }
  return relationData;
}

OLSOntologyNodeData *OLSOntologyDataController::getNodeById(long id, bool asChanges) {

  if (asChanges) {
    m_changedNodeIds.insert(id);
  }
  return m_nodesMap.value(id);
}

OLSOntologyRelationData *OLSOntologyDataController::getRelationById(long id, bool asChanges) {

  if (asChanges) {
    m_changedRelationIds.insert(id);
  }
  return m_relationsMap.value(id);
}

OLSOntologyRelationData *OLSOntologyDataController::getRelationByNodes(long sourceNodeId, long destinationNodeId) {

  QPair<long, long> nodesPair(sourceNodeId, destinationNodeId);
  OLSOntologyRelationData *relationData = m_relationsMapByNodes.value(nodesPair);
  m_changedRelationIds.insert(relationData->id);
  return relationData;
}

QMap<OLSOntologyRelationData *, OLSOntologyNodeData *> OLSOntologyDataController::getRelatedOutgoingNodes(const QString &nodeNamespace, const QString &nodeName) {
    OLSOntologyNodeData *node = findNode(nodeNamespace, nodeName);
    if (node == nullptr)
        return QMap<OLSOntologyRelationData *, OLSOntologyNodeData *>();
    return getRelatedOutgoingNodes(node->id);
}

QMap<OLSOntologyRelationData *, OLSOntologyNodeData *> OLSOntologyDataController::getRelatedOutgoingNodes(long id) {
    QMap<OLSOntologyRelationData *, OLSOntologyNodeData *> result;
    foreach (OLSOntologyRelationData *relation, m_relationsList) {
        if (relation->sourceNodeId == id)
            result.insert(relation, getNodeById(relation->destinationNodeId));
    }
    return result;
}

OLSOntologyNodeData *OLSOntologyDataController::findNode(const QString &nodeNamespace, const QString &nodeName) const {

  foreach (OLSOntologyNodeData *nodeData, m_nodesList) {
    if (nodeData->namespace_.compare(nodeNamespace, Qt::CaseInsensitive) == 0 && nodeData->name.compare(nodeName, Qt::CaseInsensitive) == 0) {
      m_changedNodeIds.insert(nodeData->id);
      return nodeData;
    }
  }
  return nullptr;
}

QString OLSOntologyDataController::visualizeOntPath() const {

  return m_visualizeOntPath;
}

// delegate

long OLSOntologyDataController::nodeCreated(long newId, QPointF position)
{
  OLSOntologyNodeData *node = new OLSOntologyNodeData();
  if (newId == LONG_MIN) {
    m_lastId++;
    node->id = m_lastId;
  } else {
    node->id = newId;
  }
  node->name = "";

  m_nodesMap.insert(node->id, node);
  m_nodesList.append(node);
  m_changedNodeIds.insert(node->id);

  if (saveHistory) {
      OLSHistoryNodeCreatedRecord *newRecord = new OLSHistoryNodeCreatedRecord(node->id, position);
      addNewRecordOrRedo(node->id, newRecord);
  }

  return node->id;
}

long OLSOntologyDataController::relationCreated(long sourceNodeId, long destinationNodeId, long newId)
{
  OLSOntologyRelationData *relation = new OLSOntologyRelationData();
  if (newId == LONG_MIN) {
    m_lastId++;
    relation->id = m_lastId;
  } else {
    relation->id = newId;
  }
  relation->name = "";
  relation->sourceNodeId = sourceNodeId;
  relation->destinationNodeId = destinationNodeId;

  m_relationsMap.insert(relation->id, relation);
  m_relationsList.append(relation);

  QPair<long, long> relationNodesPair(relation->sourceNodeId, relation->destinationNodeId);
  m_relationsMapByNodes.insert(relationNodesPair, relation);

  OLSOntologyNodeData *sourceNode = m_nodesMap.value(relation->sourceNodeId);
  sourceNode->relations.append(relation->id);

  OLSOntologyNodeData *destinationNode = m_nodesMap.value(relation->destinationNodeId);
  destinationNode->relations.append(relation->id);

  m_changedRelationIds.insert(relation->id);

  if (saveHistory) {
      OLSHistoryRelationCreatedRecord *newRecord = new OLSHistoryRelationCreatedRecord(relation->id, relation->sourceNodeId, relation->destinationNodeId);
      addNewRecordOrRedo(sourceNodeId, newRecord);
  }

  return relation->id;
}

void OLSOntologyDataController::nodeNameChanged(long nodeId, const QString &namespace_, const QString &name) {

  OLSOntologyNodeData *node = m_nodesMap.value(nodeId);
  QString oldName = node->name;
  node->name = name;
  QString oldNamespace = node->namespace_;
  node->namespace_ = namespace_;

  m_changedNodeIds.insert(node->id);

  if (saveHistory) {
      OLSHistoryNodeNameChangedRecord *newRecord = new OLSHistoryNodeNameChangedRecord(node->id, oldNamespace, oldName, node->namespace_, node->name);
      addNewRecordOrRedo(nodeId, newRecord);
  }
}

void OLSOntologyDataController::relationNameChanged(long relationId, const QString &namespace_, const QString &name) {

  OLSOntologyRelationData *relation = m_relationsMap.value(relationId);  
  QString oldName = relation->name;
  relation->name = name;
  QString oldNamespace = relation->namespace_;
  relation->namespace_ = namespace_;

  m_changedRelationIds.insert(relation->id);

  if (saveHistory) {
    OLSHistoryRelationNameChangedRecord *newRecord = new OLSHistoryRelationNameChangedRecord(relation->id, oldNamespace, oldName, relation->namespace_, relation->name);
    addNewRecordOrRedo(relation->sourceNodeId, newRecord);
  }
}

void OLSOntologyDataController::nodeAttributesChanged(long nodeId, const QVariantMap &attributes) {

  OLSOntologyNodeData *node = m_nodesMap.value(nodeId);
  node->attributes = attributes;

  m_changedNodeIds.insert(node->id);
}

void OLSOntologyDataController::relationAttributesChanged(long relationId, const QVariantMap &attributes) {

  OLSOntologyRelationData *relation = m_relationsMap.value(relationId);
  relation->attributes = attributes;

  m_changedRelationIds.insert(relation->id);
}

void OLSOntologyDataController::nodeRemoved(long nodeId) {

  OLSOntologyNodeData *node = m_nodesMap.value(nodeId);
  removeRelatedRelations(node);
  m_nodesMap.remove(nodeId);
  m_nodesList.removeAll(node);

  if (saveHistory) {
      OLSHistoryNodeRemovedRecord *newRecord = new OLSHistoryNodeRemovedRecord(nodeId, node->name, nodePosition(nodeId));
      addNewRecordOrRedo(nodeId, newRecord);
  }
  delete node;
}

void OLSOntologyDataController::relationRemoved(long relationId) {

  qDebug() << m_relationsMap;

  OLSOntologyRelationData *relation = m_relationsMap.value(relationId);

  if (relation != nullptr) {
    m_relationsMap.remove(relationId);
    m_relationsList.removeAll(relation);

    QPair<long, long> relationNodesPair(relation->sourceNodeId, relation->destinationNodeId);
    m_relationsMapByNodes.remove(relationNodesPair);

    if (saveHistory) {
        OLSHistoryRelationRemovedRecord *newRecord = new OLSHistoryRelationRemovedRecord(relationId, relation->sourceNodeId, relation->destinationNodeId, relation->name);
        addNewRecordOrRedo(relation->sourceNodeId, newRecord);
    }

    delete relation;
  }
}

void OLSOntologyDataController::removeRelatedRelations(OLSOntologyNodeData *nodeData) {

  QList<OLSOntologyRelationData *> relationsToRemove;
  foreach (OLSOntologyRelationData *relationData, m_relationsList) {
    if (relationData->sourceNodeId == nodeData->id || relationData->destinationNodeId == nodeData->id) {
      relationsToRemove.append(relationData);
    }
  }

  foreach (OLSOntologyRelationData *relationData, relationsToRemove) {
    relationRemoved(relationData->id);
  }
}

QPointF OLSOntologyDataController::nodePosition(long nodeId) const {

  if (m_nodePositions.contains(nodeId)) {
    return m_nodePositions.value(nodeId);
  }
  else {
    return QPointF();
  }
}

void OLSOntologyDataController::setNodePosition(long nodeId, const QPointF &position) {

  qDebug() << "Position changed!";
  m_nodePositions.insert(nodeId, position);
}

bool OLSOntologyDataController::isNodeChanged(long id) const {

  return m_changedNodeIds.contains(id);
}

bool OLSOntologyDataController::isRelationChanged(long id) const {

  return m_changedRelationIds.contains(id);
}

bool OLSOntologyDataController::hasChanges() const {

  return m_changedNodeIds.count() > 0 || m_changedRelationIds.count() > 0;
}

int OLSOntologyDataController::changesCount() const {

  return m_changedNodeIds.count() + m_changedRelationIds.count();
}

void OLSOntologyDataController::clearChanges() {

  m_changedNodeIds.clear();
  m_changedRelationIds.clear();
}

bool OLSOntologyDataController::isHistorySaving()
{
    return saveHistory;
}

void OLSOntologyDataController::setHistorySavingState(bool shouldBeSaved)
{
    saveHistory = shouldBeSaved;
}

OLSHistoryRecord *OLSOntologyDataController::latestChange()
{
    if (historyOrder.isEmpty())
        return nullptr;
    else
        return history[historyOrder.top()].last();
}

OLSHistoryRecord *OLSOntologyDataController::latestUndoChange()
{
    if (undoHistoryOrder.isEmpty())
        return nullptr;
    else
        return undoHistory[undoHistoryOrder.top()].last();
}

void OLSOntologyDataController::shiftHistoryChangeBackward()
{
     if (historyOrder.isEmpty()) {
         qDebug() << "History is empty!";
         throw;
     }
     long id = historyOrder.top();
     undoHistory[id] << history[id].takeLast();
     if (history.value(id).isEmpty())
         history.remove(id);
     undoHistoryOrder.push(historyOrder.pop());
}

void OLSOntologyDataController::shiftHistoryChangeForward()
{
    if (undoHistoryOrder.isEmpty()) {
        qDebug() << "Undo history is empty!";
        throw;
    }
    long id = undoHistoryOrder.top();
    history[id] << undoHistory[id].takeLast();
    if (undoHistory.value(id).isEmpty())
        undoHistory.remove(id);
    historyOrder.push(undoHistoryOrder.pop());
}

void OLSOntologyDataController::addNewRecordOrRedo(long id, OLSHistoryRecord *newRecord)
{
    bool a = !undoHistoryOrder.isEmpty();
    bool b = a && undoHistoryOrder.top() == id;
    bool c = b && (*newRecord) == undoHistory.value(id).last();
    if (c) {
        shiftHistoryChangeForward();
    } else {
        clearUndoHistory();
        history[id] << newRecord;
        historyOrder.push(id);
    }
}

void OLSOntologyDataController::clearUndoHistory()
{
    undoHistory.clear();
    undoHistoryOrder.clear();
}

void OLSOntologyDataController::resetChangedState()
{
    m_changedNodeIds.clear();
    m_changedRelationIds.clear();
}

QMap<QString, QString> OLSOntologyDataController::namespaces()
{
    return m_namespaces;
}

void OLSOntologyDataController::setNamespaces(QMap<QString, QString> newNamespaces)
{
    m_namespaces = newNamespaces;
}

QMap<QString, QString> OLSOntologyDataController::defaultNamespaces()
{
    QMap<QString, QString> result {
        { "owl", "http://www.w3.org/2002/07/owl" },
        { "xsd", "http://www.w3.org/2001/XMLSchema" },
        { "rdfs", "http://www.w3.org/2000/01/rdf-schema" },
        { "rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns" },
        { "ontolis-avis", "http://knova.ru/ontolis-avis" }
    };
    return result;
}

QMap<QString, QString> OLSOntologyDataController::allNamespaces()
{
    return defaultNamespaces().unite(m_namespaces);
}

//OLSOntologyDataController *OLSOntologyDataController::makeSnapshot() {

//  OLSOntologyDataController *snapshot = new OLSOntologyDataController(m_nodesList,
//                                                                m_relationsList,
//                                                                m_nodePositions,
//                                                                m_changedNodeIds,
//                                                                m_changedRelationIds);

//  return snapshot;
//}

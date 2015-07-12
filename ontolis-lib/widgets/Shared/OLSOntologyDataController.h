#ifndef OLSONTOLOGYDATACONTROLLER_H
#define OLSONTOLOGYDATACONTROLLER_H

#include <QtGui>
#include <QStack>

#include "OLSOntologyNodeData.h"
#include "OLSOntologyRelationData.h"
#include "OLSHistoryRecord.h"

class OLSOntologyDataController {
  private:
    long m_lastId;

    // mapping
    QMap<long, OLSOntologyNodeData *> m_nodesMap;
    QMap<long, OLSOntologyRelationData *> m_relationsMap;
    QMap<QPair<long, long>, OLSOntologyRelationData *> m_relationsMapByNodes;
    // listing
    QList<OLSOntologyNodeData *> m_nodesList;
    QList<OLSOntologyRelationData *> m_relationsList;
    QMap<QString, QString> m_namespaces;
    QVariantMap m_jsonMap;

    QMap<long, QPointF> m_nodePositions;

    mutable QSet<long> m_changedNodeIds;
    mutable QSet<long> m_changedRelationIds;

    QString m_visualizeOntPath;

    QMap<long, QList<OLSHistoryRecord *>> history;
    QMap<long, QList<OLSHistoryRecord *>> undoHistory;
    QStack<long> historyOrder;
    QStack<long> undoHistoryOrder;

    bool saveHistory = true;

    void removeRelatedRelations(OLSOntologyNodeData *nodeData);
    void clearUndoHistory();
    void addNewRecordOrRedo(long id, OLSHistoryRecord *newRecord);

public:

//    OLSOntologyDataController(QList<OLSOntologyNodeData *> nodeList,
//                           QList<OLSOntologyRelationData *> relationList,
//                           QMap<long, QPointF> nodePositions = QMap<long, QPointF>(),
//                           QSet<long> changedNodeIds = QSet<long>(),
//                           QSet<long> changedRelationIds = QSet<long>());

    OLSOntologyDataController();
    OLSOntologyDataController(const QVariant &json);

    void setVisualizeOntPath(const QString &visualizeOntPath);

    void normalize();

//    OLSOntologyDataController *makeSnapshot();

    // data source
    int nodeCount();
    int relationCount();

    OLSOntologyNodeData *getNodeByIndex(int index, bool asChanges = false);
    OLSOntologyRelationData *getRelationByIndex(int index, bool asChanges = false);

    OLSOntologyNodeData *getNodeById(long id, bool asChanges = false);
    OLSOntologyRelationData *getRelationById(long id, bool asChanges = false);

    OLSOntologyRelationData *getRelationByNodes(long sourceNodeId, long destinationNodeId);

    OLSOntologyNodeData *findNode(const QString &nodeNamespace, const QString &nodeName) const;

    QMap<OLSOntologyRelationData *, OLSOntologyNodeData *> getRelatedOutgoingNodes(const QString &nodeNamespace, const QString &nodeName);
    QMap<OLSOntologyRelationData *, OLSOntologyNodeData *> getRelatedOutgoingNodes(long id);

    QString visualizeOntPath() const;

    bool isNodeChanged(long id) const;
    bool isRelationChanged(long id) const;
    bool hasChanges() const;
    int changesCount() const;
    void clearChanges();

    bool isHistorySaving();
    void setHistorySavingState(bool shouldBeSaved);

    OLSHistoryRecord *latestChange();
    OLSHistoryRecord *latestUndoChange();
    void shiftHistoryChangeBackward();
    void shiftHistoryChangeForward();

    void resetChangedState();

    void setNamespaces(QMap<QString, QString> newNamespaces);
    QMap<QString, QString> namespaces();
    QMap<QString, QString> defaultNamespaces();
    QMap<QString, QString> allNamespaces();

    // delegate
    long nodeCreated(long newId = LONG_MIN, QPointF position = QPointF());
    long relationCreated(long sourceNodeId, long destinationNodeId, long newId = LONG_MIN);
    void nodeNameChanged(long nodeId, const QString &namespace_, const QString &name);
    void relationNameChanged(long relationId, const QString &namespace_, const QString &name);
    void nodeAttributesChanged(long nodeId, const QVariantMap &attributes);
    void relationAttributesChanged(long relationId, const QVariantMap &attributes);
    void nodeRemoved(long nodeId);
    void relationRemoved(long relationId);

    QPointF nodePosition(long nodeId) const;
    void setNodePosition(long nodeId, const QPointF &position);

    QVariant serialize();
    void deserialize(const QVariant &json);
};

#endif // OLSONTOLOGYDATACONTROLLER_H

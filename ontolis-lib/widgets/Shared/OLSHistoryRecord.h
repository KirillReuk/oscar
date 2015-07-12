#ifndef OLSHISTORYRECORD_H
#define OLSHISTORYRECORD_H

#include <QString>
#include <QPointF>

enum class HistoryRecordType
{
    NodeCreated,
    RelationCreated,
    NodeNameChanged,
    RelationNameChanged,
    NodeRemoved,
    RelationRemoved
};

class OLSHistoryRecord
{
protected:
    OLSHistoryRecord();

public:    
    virtual bool operator ==(const OLSHistoryRecord *that) const = 0;
    virtual HistoryRecordType recordType() const = 0;
};

class OLSHistoryNodeRecord : public OLSHistoryRecord
{
protected:
    OLSHistoryNodeRecord(long nodeId);

    bool operator ==(const OLSHistoryNodeRecord *that) const;

public:
    const long nodeId;
};

class OLSHistoryRelationRecord : public OLSHistoryRecord
{
protected:
    OLSHistoryRelationRecord(long relationId);

    bool operator ==(const OLSHistoryRelationRecord *that) const;

public:
    const long relationId;
};

class OLSHistoryRenamedRecord
{
protected:
    OLSHistoryRenamedRecord(const QString &oldNamespace, const QString &oldName, const QString &newNamespace, const QString &newName);

    bool operator ==(const OLSHistoryRenamedRecord *that) const;

public:
    const QString oldNamespace;
    const QString oldName;
    const QString newNamespace;
    const QString newName;
};

class OLSHistoryNamedRecord
{
protected:
    OLSHistoryNamedRecord(const QString &name);

    bool operator ==(const OLSHistoryNamedRecord *that) const;

public:
    const QString name;
};

class OLSHistoryRelationConnectedRecord
{
protected:
    OLSHistoryRelationConnectedRecord(long sourceId, long destinationId);

    bool operator ==(const OLSHistoryRelationConnectedRecord *that) const;

public:
    const long sourceId;
    const long destinationId;
};

class OLSHistoryNodePositionedRecord
{
protected:
    OLSHistoryNodePositionedRecord(QPointF position);

public:
    const QPointF position;
};

class OLSHistoryNodeCreatedRecord : public OLSHistoryNodeRecord, public OLSHistoryNodePositionedRecord
{
public:
    OLSHistoryNodeCreatedRecord(long nodeId, QPointF position);

    virtual HistoryRecordType recordType() const override { return HistoryRecordType::NodeCreated; }
    virtual bool operator ==(const OLSHistoryRecord *that) const override;
};

class OLSHistoryRelationCreatedRecord : public OLSHistoryRelationRecord, public OLSHistoryRelationConnectedRecord
{
public:
    OLSHistoryRelationCreatedRecord(long relationId, long sourceId, long destinationId);

    virtual HistoryRecordType recordType() const override { return HistoryRecordType::RelationCreated; }
    virtual bool operator ==(const OLSHistoryRecord *that) const override;
};

class OLSHistoryNodeNameChangedRecord : public OLSHistoryNodeRecord, public OLSHistoryRenamedRecord
{
public:
    OLSHistoryNodeNameChangedRecord(long nodeId, const QString &oldNamespace, const QString &oldName, const QString &newNamespace, const QString &newName);

    virtual HistoryRecordType recordType() const override { return HistoryRecordType::NodeNameChanged; }
    bool operator ==(const OLSHistoryRecord *that) const;
};

class OLSHistoryRelationNameChangedRecord : public OLSHistoryRelationRecord, public OLSHistoryRenamedRecord
{
public:
    OLSHistoryRelationNameChangedRecord(long relationId, const QString &oldNamespace, const QString &oldName, const QString &newNamespace, const QString &newName);

    virtual HistoryRecordType recordType() const override { return HistoryRecordType::RelationNameChanged; }
    bool operator ==(const OLSHistoryRecord *that) const;
};

class OLSHistoryNodeRemovedRecord : public OLSHistoryNodeRecord, public OLSHistoryNamedRecord, public OLSHistoryNodePositionedRecord
{
public:
    OLSHistoryNodeRemovedRecord(long nodeId, const QString &name, QPointF position);

    virtual HistoryRecordType recordType() const override { return HistoryRecordType::NodeRemoved; }
    bool operator ==(const OLSHistoryRecord *that) const;
};

class OLSHistoryRelationRemovedRecord : public OLSHistoryRelationRecord, public OLSHistoryNamedRecord, public OLSHistoryRelationConnectedRecord
{
public:
    OLSHistoryRelationRemovedRecord(long relationId, long sourceId, long destinationId, const QString &name);

    virtual HistoryRecordType recordType() const override { return HistoryRecordType::RelationRemoved; }
    bool operator ==(const OLSHistoryRecord *that) const;
};


#endif // OLSHISTORYRECORD_H

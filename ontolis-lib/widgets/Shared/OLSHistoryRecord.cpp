#include "OLSHistoryRecord.h"

#include <QDebug>

OLSHistoryRecord::OLSHistoryRecord()
{

}

OLSHistoryNodeRecord::OLSHistoryNodeRecord(long nodeId)
    : nodeId(nodeId)
{

}

OLSHistoryRelationRecord::OLSHistoryRelationRecord(long relationId)
    : relationId(relationId)
{

}

OLSHistoryRenamedRecord::OLSHistoryRenamedRecord(const QString &oldNamespace, const QString &oldName, const QString &newNamespace, const QString &newName)
    : oldNamespace(oldNamespace), oldName(oldName), newNamespace(newNamespace), newName(newName)
{

}

OLSHistoryNamedRecord::OLSHistoryNamedRecord(const QString &name)
    : name(name)
{

}

OLSHistoryRelationConnectedRecord::OLSHistoryRelationConnectedRecord(long sourceId, long destinationId)
    : sourceId(sourceId), destinationId(destinationId)
{

}

OLSHistoryNodePositionedRecord::OLSHistoryNodePositionedRecord(QPointF position)
    : position(position)
{

}

OLSHistoryNodeCreatedRecord::OLSHistoryNodeCreatedRecord(long nodeId, QPointF position)
    : OLSHistoryNodeRecord(nodeId), OLSHistoryNodePositionedRecord(position)
{

}

OLSHistoryRelationCreatedRecord::OLSHistoryRelationCreatedRecord(long relationId, long sourceId, long destinationId)
    : OLSHistoryRelationRecord(relationId), OLSHistoryRelationConnectedRecord(sourceId, destinationId)
{

}

OLSHistoryNodeNameChangedRecord::OLSHistoryNodeNameChangedRecord(long nodeId, const QString &oldNamespace, const QString &oldName, const QString &newNamespace, const QString &newName)
    : OLSHistoryNodeRecord(nodeId), OLSHistoryRenamedRecord(oldNamespace, oldName, newNamespace, newName)
{

}

OLSHistoryRelationNameChangedRecord::OLSHistoryRelationNameChangedRecord(long relationId, const QString &oldNamespace, const QString &oldName, const QString &newNamespace, const QString &newName)
    : OLSHistoryRelationRecord(relationId), OLSHistoryRenamedRecord(oldNamespace, oldName, newNamespace, newName)
{

}

OLSHistoryNodeRemovedRecord::OLSHistoryNodeRemovedRecord(long nodeId, const QString &name, QPointF position)
    : OLSHistoryNodeRecord(nodeId), OLSHistoryNamedRecord(name), OLSHistoryNodePositionedRecord(position)
{

}

OLSHistoryRelationRemovedRecord::OLSHistoryRelationRemovedRecord(long relationId, long sourceId, long destinationId, const QString &name)
    : OLSHistoryRelationRecord(relationId), OLSHistoryNamedRecord(name), OLSHistoryRelationConnectedRecord(sourceId, destinationId)
{

}

bool OLSHistoryNodeRecord::operator ==(const OLSHistoryNodeRecord *that) const
{
    return that != nullptr && nodeId == that->nodeId;
}

bool OLSHistoryRelationRecord::operator ==(const OLSHistoryRelationRecord *that) const
{
    return that != nullptr && relationId == that->relationId;
}

bool OLSHistoryRenamedRecord::operator ==(const OLSHistoryRenamedRecord *that) const
{
    return that != nullptr && oldName == that->oldName && newName == that->newName;
}

bool OLSHistoryNamedRecord::operator ==(const OLSHistoryNamedRecord *that) const
{
    return that != nullptr && name == that->name;
}

bool OLSHistoryRelationConnectedRecord::operator ==(const OLSHistoryRelationConnectedRecord *that) const
{
    return that != nullptr && sourceId == that->sourceId && destinationId && that->destinationId;
}

bool OLSHistoryNodeCreatedRecord::operator ==(const OLSHistoryRecord *that) const
{
    if (that != nullptr && that->recordType() == recordType()) {
        const OLSHistoryNodeCreatedRecord *thatCasted = static_cast<const OLSHistoryNodeCreatedRecord *>(that);
        return OLSHistoryNodeRecord::operator ==(thatCasted);
    }
    return false;
}

bool OLSHistoryRelationCreatedRecord::operator ==(const OLSHistoryRecord *that) const
{
    if (that != nullptr && that->recordType() == recordType()) {
        const OLSHistoryRelationCreatedRecord *thatCasted = static_cast<const OLSHistoryRelationCreatedRecord *>(that);
        return OLSHistoryRelationRecord::operator ==(thatCasted) && OLSHistoryRelationConnectedRecord::operator ==(thatCasted);
    }
    return false;
}

bool OLSHistoryNodeNameChangedRecord::operator ==(const OLSHistoryRecord *that) const
{
    if (that != nullptr && that->recordType() == recordType()) {
        const OLSHistoryNodeNameChangedRecord *thatCasted = static_cast<const OLSHistoryNodeNameChangedRecord *>(that);
        return OLSHistoryNodeRecord::operator ==(thatCasted) && OLSHistoryRenamedRecord::operator ==(thatCasted) &&
                this->newNamespace == thatCasted->newNamespace && this->oldNamespace == thatCasted->oldNamespace;
    }
    return false;
}

bool OLSHistoryRelationNameChangedRecord::operator ==(const OLSHistoryRecord *that) const
{
    if (that != nullptr && that->recordType() == recordType()) {
        const OLSHistoryRelationNameChangedRecord *thatCasted = static_cast<const OLSHistoryRelationNameChangedRecord *>(that);
        return OLSHistoryRelationRecord::operator ==(thatCasted) && OLSHistoryRenamedRecord::operator ==(thatCasted);
    }
    return false;
}

bool OLSHistoryNodeRemovedRecord::operator ==(const OLSHistoryRecord *that) const
{
    if (that != nullptr && that->recordType() == recordType()) {
        const OLSHistoryNodeRemovedRecord *thatCasted = static_cast<const OLSHistoryNodeRemovedRecord *>(that);
        return OLSHistoryNodeRecord::operator ==(thatCasted) && OLSHistoryNamedRecord::operator ==(thatCasted);
    }
    return false;
}

bool OLSHistoryRelationRemovedRecord::operator ==(const OLSHistoryRecord *that) const
{
    if (that != nullptr && that->recordType() == recordType()) {
        const OLSHistoryRelationRemovedRecord *thatCasted = static_cast<const OLSHistoryRelationRemovedRecord *>(that);
        return OLSHistoryRelationRecord::operator ==(thatCasted) && OLSHistoryNamedRecord::operator ==(thatCasted) && OLSHistoryRelationConnectedRecord::operator ==(thatCasted);
    }
    return false;
}

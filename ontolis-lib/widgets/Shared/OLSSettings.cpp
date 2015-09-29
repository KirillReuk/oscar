#include "OLSSettings.h"
#include "../OntologyGraph/OLSOntologyGraphNodeItem.h"

OLSSettings::OLSSettings()
{
    QMap<QString, QString> defaultNodeEvents, defaultRelationEvents;

    defaultNodeEvents["OnClick"] = "Highlight";
    defaultNodeEvents["OnRightClick"] = "Open Options";

    defaultRelationEvents["OnClick"] = "Highlight";
    defaultRelationEvents["OnRightClick"] = "Open Options";

    m_nodeEvents = defaultNodeEvents;
    m_relationEvents = defaultRelationEvents;
}

QMap<QString, QString> OLSSettings::getNodeEvents()
{
    return m_nodeEvents;
}

QMap<QString, QString> OLSSettings::getRelationEvents()
{
    return m_relationEvents;
}

void OLSSettings::setNodeEvents(QMap<QString, QString> &newEvents)
{
    m_nodeEvents = newEvents;
    OLSOntologyGraphNodeItem::setEvents(newEvents);
}

void OLSSettings::setRelationEvents(QMap<QString, QString> &newEvents)
{
    m_relationEvents = newEvents;
}


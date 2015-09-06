#include "OLSSettings.h"

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

void OLSSettings::changeNodeEvents(QMap<QString, QString> &newEvents)
{
    m_nodeEvents = newEvents;
}

void OLSSettings::changeRelationEvents(QMap<QString, QString> &newEvents)
{
    m_relationEvents = newEvents;
}


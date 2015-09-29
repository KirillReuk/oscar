#ifndef OLSSETTINGS
#define OLSSETTINGS

#include <QVariantMap>

class OLSSettings
{
    private:

        QMap<QString, QString> m_nodeEvents;
        QMap<QString, QString> m_relationEvents;

    public:

        OLSSettings();

        QMap<QString, QString> getNodeEvents();
        QMap<QString, QString> getRelationEvents();

        void setNodeEvents(QMap<QString, QString> &);
        void setRelationEvents(QMap<QString, QString> &);

};

#endif // OLSSETTINGS


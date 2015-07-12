#ifndef OLSONTOLOGYGRAPHELEMENT_H
#define OLSONTOLOGYGRAPHELEMENT_H

#include <QString>
#include <QMap>

#include "widgets/Shared/OLSOntologyDataController.h"

class OLSOntologyGraphElement {
  protected:
    long m_id;
    QVariantMap m_visualizeParameters;

    OLSOntologyDataController *m_dataController;

    virtual void attributesChanged() = 0;
    virtual OLSOntologyData *relatedModel() = 0;

  public:
    OLSOntologyGraphElement(OLSOntologyDataController *dataController);

    void setId(long id);
    long id() const;

    void setVisualizeParameter(QString name, QVariant value);
    void renameVisualizeParameter(QString oldName, QString newName);
    QVariant getVisualizeParameter(QString name, QVariant defaultValue = QVariant());
    QVariant getAndRemoveVisualizeParameter(QString name, QVariant defaultValue = QVariant());

    QString attributesAsText();
    QVariantMap attributes();
    void setAttributesFromData(const QByteArray &data);
    void setAttributes(const QVariantMap &attributes);

    QString name();
    QString namespace_();
    QString uri();
    QString qualifiedName();

    OLSOntologyDataController *relatedDataController() const;    

    static const QColor defaultSelectedColor;
    static const QColor defaultChangedColor;
    static const QColor defaultNormalColor;
};

#endif // OLSONTOLOGYGRAPHELEMENT_H

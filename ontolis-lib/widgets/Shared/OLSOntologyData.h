#ifndef OLSONTOLOGYDATA_H
#define OLSONTOLOGYDATA_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QList>
#include <QVariantMap>

struct OLSOntologyData {
    long id;
    QString name;
    QString namespace_;
    QVariantMap attributes;

    QString attributesAsText() const {

      QJsonObject object = QJsonObject::fromVariantMap(attributes);
      QJsonDocument document = QJsonDocument(object);
      return QString::fromUtf8(document.toJson());
    }

    inline QString uri()
    {
        return namespace_ + '#' + name;
    }

    void setAttributesFromData(const QByteArray &data) {

      QJsonDocument document = QJsonDocument::fromJson(data);
      QJsonObject object = document.object();
      attributes = object.toVariantMap();
    }
};

#endif // OLSONTOLOGYDATA_H

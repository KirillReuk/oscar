#include "OLSOntologyGraphElement.h"

#include <QDebug>
#include <QString>

// http://ethanschoonover.com/solarized
const QColor OLSOntologyGraphElement::defaultSelectedColor = QColor::fromRgb(133, 153, 0); // green
const QColor OLSOntologyGraphElement::defaultChangedColor = QColor::fromRgb(211, 54, 130); // magenta
const QColor OLSOntologyGraphElement::defaultNormalColor = QColor::fromRgb(38, 139, 210); // blue

OLSOntologyGraphElement::OLSOntologyGraphElement(OLSOntologyDataController *dataController) : m_dataController(dataController) {

  m_id = -1;
}

void OLSOntologyGraphElement::setId(long id) {

  m_id = id;
}

long OLSOntologyGraphElement::id() const {

  return m_id;
}

QString OLSOntologyGraphElement::name()
{
  return relatedModel()->name;
}

QString OLSOntologyGraphElement::namespace_()
{
  return relatedModel()->namespace_;
}

QString OLSOntologyGraphElement::uri()
{
  return relatedModel()->uri();
}

QString OLSOntologyGraphElement::qualifiedName()
{
  return m_dataController->allNamespaces().key(namespace_(), namespace_()) + "#" + name();
}

OLSOntologyDataController *OLSOntologyGraphElement::relatedDataController() const {

  return m_dataController;
}

void OLSOntologyGraphElement::setVisualizeParameter(QString name, QVariant value)
{
    m_visualizeParameters[name] = value;
}

void OLSOntologyGraphElement::renameVisualizeParameter(QString oldName, QString newName)
{
    if (!m_visualizeParameters.contains(oldName))
        throw;
    m_visualizeParameters[newName] = m_visualizeParameters[oldName];
    m_visualizeParameters.remove(oldName);
}

QVariant OLSOntologyGraphElement::getVisualizeParameter(QString name, QVariant defaultValue)
{
    return m_visualizeParameters.value(name, defaultValue);
}

QVariant OLSOntologyGraphElement::getAndRemoveVisualizeParameter(QString name, QVariant defaultValue)
{
    QVariant result = m_visualizeParameters.value(name, defaultValue);
    m_visualizeParameters.remove(name);
    return result;
}

QString OLSOntologyGraphElement::attributesAsText()
{
  return relatedModel()->attributesAsText();
}

QVariantMap OLSOntologyGraphElement::attributes()
{
  return relatedModel()->attributes;
}

void OLSOntologyGraphElement::setAttributesFromData(const QByteArray &data)
{
  relatedModel()->setAttributesFromData(data);
  attributesChanged();
}

void OLSOntologyGraphElement::setAttributes(const QVariantMap &attributes)
{
  relatedModel()->attributes = attributes;
  attributesChanged();
}

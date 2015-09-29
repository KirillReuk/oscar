#include "OLSOntologyGraphRelationItem.h"

#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QGraphicsScene>

#include <math.h>

#include "OLSOntologyGraphNodeItem.h"

OLSOntologyGraphRelationItem::OLSOntologyGraphRelationItem(
        OLSOntologyDataController *dataController, OLSAdaptiveRelationVisualizerMaster *visualizerMaster, QGraphicsItem *parent) :
  QGraphicsLineItem(parent), OLSOntologyGraphElement(dataController), m_sourceNode(nullptr), m_destinationNode(nullptr), m_visualizerMaster(visualizerMaster) {

  setPen(QPen(Qt::black, 1));
  setZValue(-1);
  m_width = 1.0;

  setFlag(ItemIsSelectable);

  setData(OLSOntologyGraphItemDataKey::kType, OLSOntologyGraphItemType::kRelation);
}

OLSOntologyGraphRelationItem::~OLSOntologyGraphRelationItem() {

  qDebug() << "Relation destructor";
}

QRectF OLSOntologyGraphRelationItem::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath OLSOntologyGraphRelationItem::shape() const
{
    QPainterPath path;
    QPainterPathStroker stroker;
    path.moveTo(line().p1());
    path.lineTo(line().p2());
    stroker.setWidth(20);
    return stroker.createStroke(path);
}
/*
void OLSOntologyGraphRelationItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  if (event->button() != Qt::LeftButton) {
    event->accept();
    return;
  }
  QGraphicsLineItem::mousePressEvent(event);
}
*/
void OLSOntologyGraphRelationItem::setSourceNode(OLSOntologyGraphNodeItem *node) {

  m_sourceNode = node;
  if (m_sourceNode != nullptr) {
    m_sourceNode->appendRelation(this);
  }
  adjust();
}

OLSOntologyGraphNodeItem *OLSOntologyGraphRelationItem::sourceNode() const {

  return m_sourceNode;
}

void OLSOntologyGraphRelationItem::setDestinationNode(OLSOntologyGraphNodeItem *node) {

  m_destinationNode = node;
  if (m_destinationNode != nullptr) {
    m_destinationNode->appendRelation(this);
  }
  adjust();
}

OLSOntologyGraphNodeItem *OLSOntologyGraphRelationItem::destinationNode() const {

  return m_destinationNode;
}

void OLSOntologyGraphRelationItem::removeFromNodes() {

  if (m_sourceNode != nullptr) {
    m_sourceNode->removeRelation(this);
    m_sourceNode = nullptr;
  }

  if (m_destinationNode != nullptr) {
    m_destinationNode->removeRelation(this);
    m_destinationNode = nullptr;
  }
}

void OLSOntologyGraphRelationItem::adjust() {

  if (m_sourceNode != nullptr && m_destinationNode != nullptr) {
    setLine(QLineF(m_sourceNode->pos(), m_destinationNode->pos()));
  }

  if (this->scene() != nullptr) {
    this->scene()->invalidate();
  }
}

void OLSOntologyGraphRelationItem::attributesChanged() {

  OLSOntologyRelationData *data = relatedDataController()->getRelationById(m_id);
  relatedDataController()->relationAttributesChanged(m_id, data->attributes);

  m_dashPattern.clear();

  if (data->attributes.keys().contains("gui-attributes")) {
    QVariantMap guiAttributes = data->attributes.value("gui-attributes").toMap();

    if (guiAttributes.contains("line_width")) {
      QString lineWidth = guiAttributes["line_width"].toString();
      m_width = lineWidth.toDouble();
    }

    if (guiAttributes.contains("dash")) {
      QString dash = guiAttributes["dash"].toString();
      if (dash == "solid") {
//        m_dashPattern.append(1.0);
      }
      else if (dash == "dash") {
        m_dashPattern.append(3.0);
        m_dashPattern.append(2.0);
      }
      else if (dash == "dot") {
        m_dashPattern.append(1.0);
        m_dashPattern.append(2.0);
      }
    }
  }
}

void OLSOntologyGraphRelationItem::relationEventHandler(QString eventName)
{
    if (getEvents().find(eventName)==getEvents().end())
        return;

    QMap<QString, QString> eventList = getEvents();
    QString eventToExecute = eventList[eventName];


    if (eventToExecute == "Highlight")
        setSelected(true);
    else
        if (eventToExecute == "Highlight With Adjacent")
            highlightWithAdjacent();
        else
            if (eventToExecute == "Open options"){
            } else
                if (eventToExecute == "Show Tip"){
                } else
                    if (eventToExecute == "Delete"){
                    }
}

void OLSOntologyGraphRelationItem::highlightWithAdjacent(){
    this->sourceNode()->setSelected(true);
    this->destinationNode()->setSelected(true);
}



void OLSOntologyGraphRelationItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
      relationEventHandler("OnClick");
      event->accept();
      return;
    } else if (event->button() == Qt::RightButton) {
        relationEventHandler("OnRightClick");
        event->accept();
        return;
      }
   //QGraphicsLineItem::mousePressEvent(event);
}

void OLSOntologyGraphRelationItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //emit nodeItemDoubleClickSignal(this->id(), event->buttons());
    if (event->button() == Qt::LeftButton) {
      relationEventHandler("OnDoubleClick");
      event->accept();
      return;
    }
}

void OLSOntologyGraphRelationItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    relationEventHandler("OnMouseEnter");
    event->accept();
    return;
}

void OLSOntologyGraphRelationItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    relationEventHandler("OnMouseLeave");
    event->accept();
    return;
}

OLSOntologyData *OLSOntologyGraphRelationItem::relatedModel()
{
    return m_dataController->getRelationById(m_id);
}

void OLSOntologyGraphRelationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    m_visualizerMaster->tryPerformAdaptableVisualization(this, painter);
}

QMap<QString, QString> OLSOntologyGraphRelationItem::getEvents()
{
    return m_relationEvents;
}

void OLSOntologyGraphRelationItem::setEvents(QMap<QString, QString> &newEvents)
{
    m_relationEvents = newEvents;
}

QMap<QString, QString> OLSOntologyGraphRelationItem::m_relationEvents;

#include "OLSOntologyGraphRelationVisualizedLine.h"

#include <QPen>
#include <QPainter>
#include <QDebug>

#include "widgets/OntologyGraph/OLSOntologyGraphNodeItem.h"

OLSOntologyGraphRelationVisualizedLine::OLSOntologyGraphRelationVisualizedLine(
        OLSOntologyDataController *dataController, OLSAdaptiveRelationVisualizerMaster *visualizerMaster, QGraphicsItem *parent) :
  OLSOntologyGraphRelationItem(dataController, visualizerMaster, parent), m_sourceNode(nullptr) {

  setData(OLSOntologyGraphItemDataKey::kType, OLSOntologyGraphItemType::kOther);
  setPen(QPen(Qt::black, 2));
  m_endPointEmpty = true;
}

void OLSOntologyGraphRelationVisualizedLine::setSourceNode(OLSOntologyGraphNodeItem *node) {

  m_sourceNode = node;
  if (m_sourceNode != nullptr) {
    m_sourceNode->appendRelation(this);
  }
  adjust();
}

OLSOntologyGraphNodeItem *OLSOntologyGraphRelationVisualizedLine::sourceNode() const {

  return m_sourceNode;
}

void OLSOntologyGraphRelationVisualizedLine::setEndPoint(QPointF point) {

  m_endPoint = point;
  m_endPointEmpty = false;
  adjust();
}

QPointF OLSOntologyGraphRelationVisualizedLine::endPoint() const {

  return m_endPoint;
}

void OLSOntologyGraphRelationVisualizedLine::adjust() {

  if (m_sourceNode != nullptr && !m_endPointEmpty) {
    setLine(QLineF(m_sourceNode->scenePos(), m_endPoint));
  }
}

void OLSOntologyGraphRelationVisualizedLine::removeFromNodes() {

  if (m_sourceNode != nullptr) {
    m_sourceNode->removeRelation(this);
    m_sourceNode = nullptr;
  }
}

void OLSOntologyGraphRelationVisualizedLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

  QPen pen = this->pen();

  QVector<qreal> dashPattern;
  dashPattern.append(2.0);
  dashPattern.append(2.0);
  pen.setDashPattern(dashPattern);
  pen.setColor(Qt::lightGray);

  painter->setPen(pen);
  painter->drawLine(line());
}

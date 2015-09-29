#include "OLSOntologyGraphNodeItem.h"

#include <QDebug>
#include <QBrush>
#include <QPainter>
#include <QFontMetrics>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "OLSOntologyGraphRelationItem.h"
#include "extensions/OLSAdaptiveNodeVisualizerMaster.h"

OLSOntologyGraphNodeItem::OLSOntologyGraphNodeItem(OLSOntologyDataController *dataController, QGraphicsItem *parent) :
  QGraphicsRectItem(parent), OLSOntologyGraphElement(dataController) {

  m_backgroundColor = QColor::fromRgb(253, 246, 227);
  m_textColor = Qt::black;
  m_shapeName = "rect";
  m_expandState = ExpandState::NotExpandable;

  m_textRect = QRectF();

  setFlag(ItemIsMovable);
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges);

  QBrush brush = QBrush(Qt::SolidPattern);
  brush.setColor(Qt::white);
  setBrush(brush);

  setData(OLSOntologyGraphItemDataKey::kType, OLSOntologyGraphItemType::kNode);
}

OLSOntologyGraphNodeItem::~OLSOntologyGraphNodeItem() {

  qDebug() << "OLSOntologyGraphNodeItem destructor";
}

void OLSOntologyGraphNodeItem::appendRelation(OLSOntologyGraphRelationItem *relation) {

  if (!m_relations.contains(relation)) {
    m_relations.append(relation);
  }
}

void OLSOntologyGraphNodeItem::removeRelation(OLSOntologyGraphRelationItem *relation) {

  m_relations.removeAll(relation);
}

void OLSOntologyGraphNodeItem::removeAllRelations() {

  foreach (OLSOntologyGraphRelationItem *item, m_relations) {
    item->removeFromNodes();
  }
}

QVariant OLSOntologyGraphNodeItem::itemChange(GraphicsItemChange change, const QVariant &value) {

  if (change == ItemPositionHasChanged) {
    foreach (OLSOntologyGraphRelationItem *relation, m_relations) {
      relation->adjust();
    }

    emit nodeItemPositionChangedSignal(this->id(), value.toPointF());
  }

  return QGraphicsRectItem::itemChange(change, value);
}

void OLSOntologyGraphNodeItem::attributesChanged() {

  OLSOntologyNodeData *data = relatedDataController()->getNodeById(m_id);
  relatedDataController()->nodeAttributesChanged(m_id, data->attributes);

  if (data->attributes.keys().contains("gui-attributes")) {
    QVariantMap guiAttributes = data->attributes.value("gui-attributes").toMap();

    if (guiAttributes.contains("text_color")) {
      QString textColorHex = guiAttributes["text_color"].toString();
      m_textColor = QColor(textColorHex);
    }

    if (guiAttributes.contains("bg_color")) {
      QString bgColorHex = guiAttributes["bg_color"].toString();
      m_backgroundColor = QColor(bgColorHex);
    }

    if (guiAttributes.contains("shape_name")) {
      m_shapeName = guiAttributes["shape_name"].toString();
    }
  }

  if (this->scene() != nullptr) {
    this->scene()->invalidate();
  }
}

OLSOntologyData *OLSOntologyGraphNodeItem::relatedModel()
{
    return m_dataController->getNodeById(m_id);
}

QList<OLSOntologyGraphRelationItem *> OLSOntologyGraphNodeItem::getOutgoingRelations() {
    QList<OLSOntologyGraphRelationItem *> result;
    for (OLSOntologyGraphRelationItem *relation : m_relations)
        if (relation->sourceNode() == this)
            result << relation;
    return result;
}

QList<OLSOntologyGraphRelationItem *> OLSOntologyGraphNodeItem::getOutgoingRelationsByName(QString neededName)  {
    QList<OLSOntologyGraphRelationItem *> result;
    for (OLSOntologyGraphRelationItem *relation : getOutgoingRelations())
        if (relation->id() != -1 && relation->name() == neededName)
            result << relation;
    return result;
}

QList<OLSOntologyGraphRelationItem *> OLSOntologyGraphNodeItem::getOutgoingRelationsByName(QString nodeNamespace, QString neededName)  {
    QList<OLSOntologyGraphRelationItem *> result;
    for (OLSOntologyGraphRelationItem *relation : getOutgoingRelations())
        if (relation->id() != -1 && relation->namespace_() == nodeNamespace && relation->name() == neededName)
            result << relation;
    return result;
}

QList<OLSOntologyGraphRelationItem *> OLSOntologyGraphNodeItem::getOutgoingRelationsByNameBeginning(QString nodeNamespace, QString neededBeginning)  {
    QList<OLSOntologyGraphRelationItem *> result;
    for (OLSOntologyGraphRelationItem *relation : getOutgoingRelations())
        if (relation->id() != -1 && relation->namespace_() == nodeNamespace && relation->name().startsWith(neededBeginning))
            result << relation;
    return result;
}


void OLSOntologyGraphNodeItem::nodeEventHandler(QString eventName)
{
    if (OLSOntologyGraphNodeItem::getEvents().find(eventName)==OLSOntologyGraphNodeItem::getEvents().end())
        return;

    QMap<QString, QString> eventList = OLSOntologyGraphNodeItem::getEvents();
    QString eventToExecute = eventList[eventName];


    if (eventToExecute == "Highlight")
        setSelected(true);
    else
        if (eventToExecute == "Highlight With Adjacent Edges")
            highlightWithAdjacentRelations();
        else
            if (eventToExecute == "Highlight With Adjacent Nodes")
                highlightWithAdjacentNodes();
            else
                if (eventToExecute == "Open options"){
                } else
                    if (eventToExecute == "Show Tip"){
                    } else
                        if (eventToExecute == "Delete"){
                        }
}

void OLSOntologyGraphNodeItem::highlightWithAdjacentRelations(){
    foreach (OLSOntologyGraphRelationItem* item, m_relations)
        item->setSelected(true);
}

void OLSOntologyGraphNodeItem::highlightWithAdjacentNodes(){
    foreach (OLSOntologyGraphRelationItem* item, m_relations){
        item->sourceNode()->setSelected(true);
        item->destinationNode()->setSelected(true);
    }
}


void OLSOntologyGraphNodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
      nodeEventHandler("OnClick");
      event->accept();
      return;
    } else if (event->button() == Qt::RightButton) {
        nodeEventHandler("OnRightClick");
        event->accept();
        return;
      }
}

void OLSOntologyGraphNodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
      nodeEventHandler("OnDoubleClick");
      event->accept();
      return;
    }
}

void OLSOntologyGraphNodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    nodeEventHandler("OnMouseEnter");
    event->accept();
    return;
}

void OLSOntologyGraphNodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    nodeEventHandler("OnMouseLeave");
    event->accept();
    return;
}


void OLSOntologyGraphNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {

    QString nameRegime = qApp->property(ONTOLIS_NAME_REGIME).toString();
    QString displayName = (nameRegime == ONTOLIS_NAME_REGIME_LOCAL) ? name() : ((nameRegime == ONTOLIS_NAME_REGIME_QNAME) ? qualifiedName() : uri());

    if (displayName.length() > 0) {
      QFontMetrics metrics = painter->fontMetrics();
      m_textRect = metrics.boundingRect(displayName);
    }
    else {
      m_textRect = QRectF(-60.0, -35.0, 120.0, 70.0);
    }

  QPen shapePen = this->pen();
  QPen textPen = this->pen();
  QBrush shapeBrush = this->brush();

  if (isSelected()) {
    QVector<qreal> dashPattern;
    dashPattern.append(2.0);
    dashPattern.append(2.0);
    shapePen.setDashPattern(dashPattern);
    shapePen.setColor(defaultSelectedColor);
    textPen.setColor(defaultSelectedColor);
    shapeBrush.setColor(m_backgroundColor);
  }
  else if (m_dataController->isNodeChanged(m_id)) {
    QVector<qreal> dashPattern;
    dashPattern.append(2.0);
    dashPattern.append(2.0);
    shapePen.setDashPattern(dashPattern);
    shapePen.setColor(defaultChangedColor);
    textPen.setColor(defaultChangedColor);
    shapeBrush.setColor(m_backgroundColor);
  }
  else {
    shapeBrush.setColor(m_backgroundColor);
    textPen.setColor(defaultNormalColor);
    shapePen.setColor(defaultNormalColor);
  }

  QTextOption textOption;
  textOption.setAlignment(Qt::AlignCenter);
  textOption.setWrapMode(QTextOption::WordWrap);
  painter->setPen(shapePen);
  painter->setBrush(shapeBrush);

  if (!OLSAdaptiveNodeVisualizerMaster::tryPerformAdaptableVisualization(this, painter)) {
      setRect(m_textRect);
      painter->drawRect(this->rect());
  }

  painter->setPen(textPen);
  painter->drawText(boundingRect(), displayName, textOption);
}


QRectF OLSOntologyGraphNodeItem::textRect() {
    return m_textRect;
}

ExpandState OLSOntologyGraphNodeItem::expandState() {
    return m_expandState;
}

void OLSOntologyGraphNodeItem::setExpandState(ExpandState expandState){
    m_expandState = expandState;
}

QMap<QString, QString> OLSOntologyGraphNodeItem::getEvents()
{
    return m_nodeEvents;
}

void OLSOntologyGraphNodeItem::setEvents(QMap<QString, QString> &newEvents)
{
    m_nodeEvents = newEvents;
}

QMap<QString, QString> OLSOntologyGraphNodeItem::m_nodeEvents;



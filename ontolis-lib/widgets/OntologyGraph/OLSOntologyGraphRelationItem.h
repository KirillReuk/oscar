#ifndef OLSONTOLOGYGRAPHRELATIONITEM_H
#define OLSONTOLOGYGRAPHRELATIONITEM_H

#include <QGraphicsLineItem>

#include "OLSOntologyGraphItemDataKey.h"
#include "OLSOntologyGraphItemType.h"
#include "OLSOntologyGraphElement.h"
#include "extensions/OLSAdaptiveRelationVisualizerMaster.h"

class OLSOntologyGraphNodeItem;
class OLSAdaptiveRelationVisualizerMaster;

class OLSOntologyGraphRelationItem : public QGraphicsLineItem, public OLSOntologyGraphElement {
  private:
    OLSOntologyGraphNodeItem *m_sourceNode;
    OLSOntologyGraphNodeItem *m_destinationNode;
    OLSAdaptiveRelationVisualizerMaster *m_visualizerMaster;

    double m_width;
    QVector<qreal> m_dashPattern;
    static QMap<QString, QString> m_relationEvents;

    //events and stuff
    void relationEventHandler(QString eventName, QPoint point);
    void highlightWithAdjacent();

  protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void attributesChanged() override;
    virtual OLSOntologyData *relatedModel() override;

  public:
    explicit OLSOntologyGraphRelationItem(OLSOntologyDataController *dataController, OLSAdaptiveRelationVisualizerMaster *visualizerMaster, QGraphicsItem *parent = 0);
    ~OLSOntologyGraphRelationItem();

    QRectF boundingRect() const;
    QPainterPath shape() const;

    virtual void adjust();

    virtual void removeFromNodes();

    virtual void setSourceNode(OLSOntologyGraphNodeItem *node);
    virtual OLSOntologyGraphNodeItem *sourceNode() const;

    virtual void setDestinationNode(OLSOntologyGraphNodeItem *node);
    virtual OLSOntologyGraphNodeItem *destinationNode() const;

    static void setEvents(QMap<QString, QString> &);
    static QMap<QString, QString> getEvents();
};

#endif // OLSONTOLOGYGRAPHRELATIONITEM_H

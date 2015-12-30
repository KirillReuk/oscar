#ifndef OLSONTOLOGYGRAPHNODEITEM_H
#define OLSONTOLOGYGRAPHNODEITEM_H

#include <QGraphicsRectItem>

#include "OLSOntologyGraphItemDataKey.h"
#include "OLSOntologyGraphItemType.h"
#include "OLSOntologyGraphElement.h"



class OLSOntologyGraphRelationItem;

enum class ExpandState { NotExpandable, IsExpanded, NotExpanded };

class OLSOntologyGraphNodeItem : public QObject, public QGraphicsRectItem, public OLSOntologyGraphElement {
    Q_OBJECT

  private:
    QList<OLSOntologyGraphRelationItem *> m_relations;
    QColor m_backgroundColor;
    QColor m_textColor;
    QString m_shapeName;
    QRectF m_textRect;
    ExpandState m_expandState;
    static QMap<QString, QString> m_nodeEvents;

    //events and stuff
    void nodeEventHandler(QString eventName, QPoint point);
    void highlightWithAdjacentNodes();
    void highlightWithAdjacentRelations();

  protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    virtual void attributesChanged() override;
    virtual OLSOntologyData *relatedModel() override;

  public:
    explicit OLSOntologyGraphNodeItem(OLSOntologyDataController *dataController, QGraphicsItem *parent = 0);
    ~OLSOntologyGraphNodeItem();

    void appendRelation(OLSOntologyGraphRelationItem *relation);
    void removeRelation(OLSOntologyGraphRelationItem *relation);

    void removeAllRelations();

    QRectF textRect();
    ExpandState expandState();
    void setExpandState(ExpandState expandState);
    //void showContextMenuSlot(const QPoint &pos);

    QList<OLSOntologyGraphRelationItem *> getOutgoingRelations();
    QList<OLSOntologyGraphRelationItem *> getOutgoingRelationsByName(QString name);
    QList<OLSOntologyGraphRelationItem *> getOutgoingRelationsByName(QString nodeNamespace, QString name);
    QList<OLSOntologyGraphRelationItem *> getOutgoingRelationsByNameBeginning(QString nodeNamespace, QString neededBeginning);

    static void setEvents(QMap<QString, QString> &);
    static QMap<QString, QString> getEvents();

    bool openOptionFlag = false;

  signals:
    void nodeItemPositionChangedSignal(long id, const QPointF &newPosition);
    void nodeItemDoubleClickSignal(long id, Qt::MouseButtons mouseButtons);
};

#endif // OLSONTOLOGYGRAPHNODEITEM_H

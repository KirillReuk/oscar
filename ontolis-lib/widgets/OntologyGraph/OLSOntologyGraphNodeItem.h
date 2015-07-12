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

  protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
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

    QList<OLSOntologyGraphRelationItem *> getOutgoingRelations();
    QList<OLSOntologyGraphRelationItem *> getOutgoingRelationsByName(QString name);
    QList<OLSOntologyGraphRelationItem *> getOutgoingRelationsByName(QString nodeNamespace, QString name);
    QList<OLSOntologyGraphRelationItem *> getOutgoingRelationsByNameBeginning(QString nodeNamespace, QString neededBeginning);

  signals:
    void nodeItemPositionChangedSignal(long id, const QPointF &newPosition);
    void nodeItemDoubleClickSignal(long id, Qt::MouseButtons mouseButtons);
};

#endif // OLSONTOLOGYGRAPHNODEITEM_H

#ifndef OLSONTOLOGYGRAPHWIDGET_H
#define OLSONTOLOGYGRAPHWIDGET_H

#include <QTextEdit>

#include "widgets/Shared/OLSOntologyDataController.h"
#include "extensions/OLSAdaptiveRelationVisualizerMaster.h"

#include "OLSOntologyGraphView.h"

class OLSOntologyGraphRelationVisualizedLine;
class OLSOntologyGraphNodeItem;

namespace Ui {
  class OLSOntologyGraphWidget;
}

class OLSOntologyGraphWidget : public QWidget {
    Q_OBJECT
    
  public:
    explicit OLSOntologyGraphWidget(QWidget *parent = 0);
    ~OLSOntologyGraphWidget();

    void setDataController(OLSOntologyDataController *dataController);
    OLSOntologyDataController *dataController() const;

    QImage makeScreenshot() const;

    void updateData();

    OLSAdaptiveRelationVisualizerMaster *relationVisualizeMaster();


  protected:
//    void dropEvent(QDropEvent *event);
//    void dragEnterEvent(QDragEnterEvent *event);
    
  private:
    Ui::OLSOntologyGraphWidget *ui;

    QTextEdit *m_sourceCodeViewer;
    OLSOntologyGraphView *m_ontologyView;
    QPointF m_lastRightClickScenePosition;
    OLSOntologyGraphRelationVisualizedLine *m_relationVisualizedLine;
    OLSOntologyDataController *m_dataController;
    OLSAdaptiveRelationVisualizerMaster *m_relationVisualizeMaster;

    bool m_editRelationMode;
    bool m_sourceCodeShown;

    QSet<long> m_nodesToBeDisplayed;

    void setEditRelationMode(bool on);
    void setRelation(OLSOntologyGraphNodeItem *sourceNode, OLSOntologyGraphNodeItem *destinationNode, long newId = LONG_MIN);
    OLSOntologyGraphNodeItem *addNode(QPointF scenePos, long newId);
    OLSOntologyGraphElement *getGraphElementById(long id);
    void adjustZoom();

  signals:
    void dataChangedSignal();
    void undoAvailabilityChangedSignal(bool available);
    void redoAvailabilityChangedSignal(bool available);

  public slots:
    void showContextMenuSlot(const QPoint &pos);

    void addNodeSlot(long newId = LONG_MIN);
    void setRelationSlot();
    void editNodeSlot(QString newNamespace = QString(), QString newName = QString());
    void editRelationSlot(QString newNamespace = QString(), QString newName = QString());
    void removeSelectedSlot();
    void editAttrsSlot();

    void setSolidDashPatternSlot();
    void setDashDashPatternSlot();
    void setDotDashPatternSlot();

    void expandAllSlot();
    void collapseAllSlot();

    void sceneSelectionChangedSlot();

    void ontologyViewMousePositionChangedSlot(const QPoint &pos);

    void dataChangedSlot();

    void itemSelectedSlot(long id);
    void itemsSelectedSlot(const QSet<long> ids);

    void zoomInSlot();
    void zoomOutSlot();

    void undoSlot();
    void redoSlot();
    void editNamespacesSlot();

    void nodeItemPositionChangedSlot(long id, const QPointF &newPosition);
    void nodeItemDoubleClickSlot(long id, Qt::MouseButtons mouseButtons);

    void showOntologySlot();
};

#endif // OLSONTOLOGYGRAPHWIDGET_H

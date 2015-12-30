#include "OLSOntologyGraphWidget.h"

#include "ui_OLSOntologyGraphWidget.h"
#include "ui_OLSOntologyGraphAttributeEditor.h"

#include <QtWidgets>

#include "widgets/OntologyGraph/OLSOntologyGraphNodeItem.h"
#include "widgets/OntologyGraph/OLSOntologyGraphRelationItem.h"
#include "widgets/OntologyGraph/OLSOntologyGraphRelationVisualizedLine.h"
#include "extensions/OLSAdaptiveNodeVisualizerMaster.h"

#include "DataControllerHelpers.hpp"
#include "IOHelpers.hpp"

OLSOntologyGraphWidget::OLSOntologyGraphWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::OLSOntologyGraphWidget) {

  ui->setupUi(this);

  setAcceptDrops(true);

  m_ontologyView = new OLSOntologyGraphView(this);
  m_ontologyView->setDragMode(QGraphicsView::RubberBandDrag);
  m_ontologyView->setContextMenuPolicy(Qt::CustomContextMenu);
  m_ontologyView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

  m_sourceCodeViewer = new QTextEdit(this);
  m_sourceCodeViewer->setReadOnly(true);
  m_sourceCodeViewer->setVisible(false);

  QGraphicsScene *scene = new QGraphicsScene(m_ontologyView);
  QBrush bgBrush = QBrush(Qt::Dense7Pattern);
  bgBrush.setColor(QColor::fromRgb(214, 209, 192));
  scene->setBackgroundBrush(bgBrush);

  m_ontologyView->setScene(scene);

  connect(m_ontologyView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showContextMenuSlot(QPoint)));

  m_dataController = nullptr;
  m_relationVisualizeMaster = new OLSAdaptiveRelationVisualizerMaster;
  m_relationVisualizedLine = nullptr;
  m_editRelationMode = false;
  m_sourceCodeShown = false;

  ui->verticalLayout->addWidget(m_ontologyView);
}


OLSOntologyGraphWidget::~OLSOntologyGraphWidget() {

  delete ui;
}

void OLSOntologyGraphWidget::setDataController(OLSOntologyDataController *dataController) {

  m_dataController = dataController;
}

OLSOntologyDataController *OLSOntologyGraphWidget::dataController() const {

  return m_dataController;
}

OLSAdaptiveRelationVisualizerMaster* OLSOntologyGraphWidget::relationVisualizeMaster()
{
    return m_relationVisualizeMaster;
}

void OLSOntologyGraphWidget::showContextMenuSlot(const QPoint &pos) {


  QList<QGraphicsItem *> selectedItems = m_ontologyView->scene()->selectedItems();

  QMenu contextMenu;
  QAction *addNodeAction = contextMenu.addAction(tr("Add node..."));

  if (selectedItems.count() == 1) {
    QGraphicsItem *selectedItem = selectedItems.at(0);
    if (selectedItem->data(OLSOntologyGraphItemDataKey::kType).toInt() == OLSOntologyGraphItemType::kNode){
        OLSOntologyGraphNodeItem *nodeItem = static_cast<OLSOntologyGraphNodeItem *>(selectedItem);
        if (!nodeItem->openOptionFlag)
            return;
    }
    else if (selectedItem->data(OLSOntologyGraphItemDataKey::kType).toInt() == OLSOntologyGraphItemType::kRelation) {
        OLSOntologyGraphRelationItem *relationItem = static_cast<OLSOntologyGraphRelationItem *>(selectedItem);
        if (!relationItem->openOptionFlag)
            return;
    }
  }


  if (selectedItems.count() == 1) {
    QGraphicsItem *selectedItem = selectedItems.at(0);
    if (selectedItem->data(OLSOntologyGraphItemDataKey::kType).toInt() == OLSOntologyGraphItemType::kNode) {
      QAction *addRelationAction = contextMenu.addAction(tr("Set relation with..."));
      connect(addRelationAction, SIGNAL(triggered()), SLOT(setRelationSlot()));
    }
  }
  contextMenu.addSeparator();

  connect(addNodeAction, SIGNAL(triggered()), SLOT(addNodeSlot()));

  if (selectedItems.count() == 1) {
    QGraphicsItem *selectedItem = selectedItems.at(0);
    if (selectedItem->data(OLSOntologyGraphItemDataKey::kType).toInt() == OLSOntologyGraphItemType::kNode) {
      QAction *editNodeAction = contextMenu.addAction(tr("Edit node..."));
      connect(editNodeAction, SIGNAL(triggered()), SLOT(editNodeSlot()));
    }
    else if (selectedItem->data(OLSOntologyGraphItemDataKey::kType).toInt() == OLSOntologyGraphItemType::kRelation) {
      QAction *editRelationAction = contextMenu.addAction(tr("Edit relation..."));
      connect(editRelationAction, SIGNAL(triggered()), SLOT(editRelationSlot()));

      QMenu *dashMenu = contextMenu.addMenu("Dash");
      QAction *solidDashAction = dashMenu->addAction("Solid");
      QAction *dashDashAction = dashMenu->addAction("Dash");
      QAction *dotDashAction = dashMenu->addAction("Dot");

      connect(solidDashAction, SIGNAL(triggered()), SLOT(setSolidDashPatternSlot()));
      connect(dashDashAction, SIGNAL(triggered()), SLOT(setDashDashPatternSlot()));
      connect(dotDashAction, SIGNAL(triggered()), SLOT(setDotDashPatternSlot()));
    }
  }
  else {
    QAction *editAction = contextMenu.addAction(tr("Edit..."));
    editAction->setEnabled(false);

  }
  contextMenu.addSeparator();

  QAction *removeAction = contextMenu.addAction(tr("Remove..."));

  if (selectedItems.count() == 0) {
    removeAction->setEnabled(false);
  }
  else {
    connect(removeAction, SIGNAL(triggered()), SLOT(removeSelectedSlot()));
  }

  if (m_ontologyView->scene()->selectedItems().count() == 1) {
    contextMenu.addSeparator();
    QAction *editAttrsAction = contextMenu.addAction(tr("Edit attributes..."));
    connect(editAttrsAction, SIGNAL(triggered()), SLOT(editAttrsSlot()));
  }

  contextMenu.addSeparator();
  QAction *expandAllAction = contextMenu.addAction(tr("Expand all"));
  connect(expandAllAction, SIGNAL(triggered()), SLOT(expandAllSlot()));

  QAction *collapseAllAction = contextMenu.addAction(tr("Collapse all"));
  connect(collapseAllAction, SIGNAL(triggered()), SLOT(collapseAllSlot()));

  QMenu *arrangeMenu = contextMenu.addMenu(tr("Arrange"));
  auto applyArrange = [this](){
      QList<QGraphicsItem *> items = m_ontologyView->scene()->items();
      foreach (QGraphicsItem *item, items) {
          if (item->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kNode) {
            OLSOntologyGraphNodeItem *nodeItem = static_cast<OLSOntologyGraphNodeItem *>(item);
            QPointF pos = m_dataController->nodePosition(nodeItem->id());
            nodeItem->setPos(pos);
          }
      }
  };

  QAction *gursoyAtunArrangeAction = arrangeMenu->addAction(tr("Gursoy-Atun"));
  connect(gursoyAtunArrangeAction, &QAction::triggered, [applyArrange, this](){
       ontolis::datacontroller::helpers::arrangeNodes(m_dataController, ontolis::datacontroller::helpers::ArrangeMethods::GursoyAtun);
       applyArrange();
  });

  QAction *fruchtermanReingoldArrangeAction = arrangeMenu->addAction(tr("Fruchterman-Reingold"));
  connect(fruchtermanReingoldArrangeAction, &QAction::triggered, [applyArrange, this](){
       ontolis::datacontroller::helpers::arrangeNodes(m_dataController, ontolis::datacontroller::helpers::ArrangeMethods::FruchtermanReingold);
       applyArrange();
  });

  m_lastRightClickScenePosition = m_ontologyView->mapToScene(pos);
  QPoint globalPos = m_ontologyView->mapToGlobal(pos);
  contextMenu.exec(globalPos);
}

void OLSOntologyGraphWidget::expandAllSlot()
{
    m_nodesToBeDisplayed.clear();
    int nodeCount = m_dataController->nodeCount();
    for (int i = 0; i < nodeCount; ++i) {
        OLSOntologyNodeData *nodeData = m_dataController->getNodeByIndex(i);
        m_nodesToBeDisplayed << nodeData->id;
    }
    updateData();
}

void OLSOntologyGraphWidget::collapseAllSlot()
{
    m_nodesToBeDisplayed.clear();
    updateData();
}

void OLSOntologyGraphWidget::setEditRelationMode(bool on) {

  if (on) {
    m_ontologyView->scene()->clearSelection();
    m_editRelationMode = true;
    m_ontologyView->setMouseTracking(true);
    m_ontologyView->setDragMode(QGraphicsView::NoDrag);
    connect(m_ontologyView, SIGNAL(mousePositionChangedSignal(QPoint)), SLOT(ontologyViewMousePositionChangedSlot(QPoint)));
    connect(m_ontologyView->scene(), SIGNAL(selectionChanged()), SLOT(sceneSelectionChangedSlot()));
  }
  else {
    m_editRelationMode = false;
    m_ontologyView->setMouseTracking(false);
    m_ontologyView->setDragMode(QGraphicsView::RubberBandDrag);
    disconnect(this, SLOT(ontologyViewMousePositionChangedSlot(QPoint)));

    if (m_relationVisualizedLine != nullptr) {
      m_relationVisualizedLine->removeFromNodes();
      m_ontologyView->scene()->removeItem(m_relationVisualizedLine);
      delete m_relationVisualizedLine;
      m_relationVisualizedLine = nullptr;
    }
  }
}

OLSOntologyGraphNodeItem *OLSOntologyGraphWidget::addNode(QPointF scenePos, long newId) {

  long newNodeId = -1;
  if (m_dataController != nullptr) {
    newNodeId = m_dataController->nodeCreated(newId, scenePos);
    m_nodesToBeDisplayed << newNodeId;
    emit dataChangedSignal();
    emit undoAvailabilityChangedSignal(true);
  }

  OLSOntologyGraphNodeItem *newNode = new OLSOntologyGraphNodeItem(m_dataController);
  newNode->setId(newNodeId);
  newNode->setPos(scenePos);
  connect(newNode, SIGNAL(nodeItemPositionChangedSignal(long, QPointF)), SLOT(nodeItemPositionChangedSlot(long, QPointF)));
  connect(newNode, SIGNAL(nodeItemDoubleClickSignal(long, Qt::MouseButtons)), SLOT(nodeItemDoubleClickSlot(long, Qt::MouseButtons)));
  m_ontologyView->scene()->addItem(newNode);
  return newNode;
}

void OLSOntologyGraphWidget::addNodeSlot(long newId) {

  QPointF scenePos = m_lastRightClickScenePosition;
  OLSOntologyGraphNodeItem *node = addNode(scenePos, newId);
  if (newId == LONG_MIN) {
    itemSelectedSlot(node->id());
    editNodeSlot();
  }
}

void OLSOntologyGraphWidget::setRelation(OLSOntologyGraphNodeItem *sourceNode, OLSOntologyGraphNodeItem *destinationNode, long newId) {

  if (sourceNode->id() == destinationNode->id())
      return;
  long newRelationId = -1;
  if (m_dataController != nullptr) {
    newRelationId = m_dataController->relationCreated(sourceNode->id(), destinationNode->id(), newId);
    emit dataChangedSignal();
    emit undoAvailabilityChangedSignal(true);
  }

  OLSOntologyGraphRelationItem *relationItem = new OLSOntologyGraphRelationItem(m_dataController, m_relationVisualizeMaster);
  relationItem->setId(newRelationId);
  relationItem->setSourceNode(sourceNode);
  relationItem->setDestinationNode(destinationNode);
  m_ontologyView->scene()->addItem(relationItem);
}

void OLSOntologyGraphWidget::updateData() {

  if (m_dataController == nullptr) {
    this->setEnabled(false);
  }
  else {
    this->setEnabled(true);

    QMap<long, OLSOntologyGraphNodeItem *> invalidatedNodesMap;
    QMap<long, OLSOntologyGraphRelationItem *> invalidatedRelationsMap;

    QList<QGraphicsItem *> items = m_ontologyView->scene()->items();
    foreach (QGraphicsItem *item, items) {
      if (item->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kNode) {
        OLSOntologyGraphNodeItem *nodeItem = static_cast<OLSOntologyGraphNodeItem *>(item);
        invalidatedNodesMap.insert(nodeItem->id(), nodeItem);
      }
      else if (item->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kRelation) {
        OLSOntologyGraphRelationItem *relationItem = static_cast<OLSOntologyGraphRelationItem *>(item);
        invalidatedRelationsMap.insert(relationItem->id(), relationItem);
      }
    }

    int nodeCount = m_dataController->nodeCount();
    if (m_nodesToBeDisplayed.isEmpty()) {
        int min = INT_MAX;
        for (int i = 0; i < nodeCount; ++i) {
            OLSOntologyNodeData *nodeData = m_dataController->getNodeByIndex(i);
            int relationsCount = std::count_if(nodeData->relations.cbegin(), nodeData->relations.cend(), [nodeData, this](long relationId){
                return m_dataController->getRelationById(relationId) != nullptr && m_dataController->getRelationById(relationId)->destinationNodeId == nodeData->id;});
            if (relationsCount < min) {
                min = relationsCount;
                m_nodesToBeDisplayed.clear();
                m_nodesToBeDisplayed << nodeData->id;
            } else if (relationsCount == min) {
                m_nodesToBeDisplayed << nodeData->id;
            }
        }
    }

    QMap<long, OLSOntologyGraphNodeItem *> existedNodes;
    for (int id: m_nodesToBeDisplayed) {
      OLSOntologyNodeData *nodeData = m_dataController->getNodeById(id);
      OLSOntologyGraphNodeItem *nodeItem = nullptr;
      if (invalidatedNodesMap.contains(nodeData->id)) {
        nodeItem = invalidatedNodesMap.value(nodeData->id);
        invalidatedNodesMap.remove(nodeItem->id());
        existedNodes.insert(nodeItem->id(), nodeItem);
        QPointF oldPos = m_dataController->nodePosition(nodeData->id);
        QPointF newPos = nodeItem->pos();
        if (oldPos != newPos)
            m_dataController->setNodePosition(nodeData->id, newPos);
      }
      else {
        QPointF pos = m_dataController->nodePosition(nodeData->id);

        nodeItem = new OLSOntologyGraphNodeItem(m_dataController);
        nodeItem->setId(nodeData->id);
        nodeItem->setPos(pos);
        m_ontologyView->scene()->addItem(nodeItem);
        existedNodes.insert(nodeItem->id(), nodeItem);
        connect(nodeItem, SIGNAL(nodeItemPositionChangedSignal(long, QPointF)), SLOT(nodeItemPositionChangedSlot(long, QPointF)));
        connect(nodeItem, SIGNAL(nodeItemDoubleClickSignal(long, Qt::MouseButtons)), SLOT(nodeItemDoubleClickSlot(long, Qt::MouseButtons)));
      }

      bool isExpandable = std::any_of(nodeData->relations.cbegin(), nodeData->relations.cend(), [nodeData, this](long relationId){
          OLSOntologyRelationData *relation = m_dataController->getRelationById(relationId);
          return relation != nullptr && relation->sourceNodeId == nodeData->id;
      });

      if (!isExpandable)
          nodeItem->setExpandState(ExpandState::NotExpandable);
      else {
          bool isExpanded = std::all_of(nodeData->relations.cbegin(), nodeData->relations.cend(), [nodeData, this](long relationId){
              OLSOntologyRelationData *relation = m_dataController->getRelationById(relationId);
              return relation != nullptr && m_nodesToBeDisplayed.contains(relation->sourceNodeId) && m_nodesToBeDisplayed.contains(relation->destinationNodeId);
          });
          if (isExpanded)
              nodeItem->setExpandState(ExpandState::IsExpanded);
          else
              nodeItem->setExpandState(ExpandState::NotExpanded);
      }
    }

    int relationCount = m_dataController->relationCount();
    for (int i = 0; i < relationCount; ++i) {
      OLSOntologyRelationData *relationData = m_dataController->getRelationByIndex(i);
      if (!m_nodesToBeDisplayed.contains(relationData->sourceNodeId) || !m_nodesToBeDisplayed.contains(relationData->destinationNodeId))
          continue;

      if (invalidatedRelationsMap.contains(relationData->id)) {
        OLSOntologyGraphRelationItem *relationItem = invalidatedRelationsMap.value(relationData->id);
        invalidatedRelationsMap.remove(relationItem->id());
      }
      else {
        OLSOntologyGraphRelationItem *relationItem = new OLSOntologyGraphRelationItem(m_dataController, m_relationVisualizeMaster);
        relationItem->setId(relationData->id);

        OLSOntologyGraphNodeItem *sourceNode = existedNodes.value(relationData->sourceNodeId);
        OLSOntologyGraphNodeItem *destinationNode = existedNodes.value(relationData->destinationNodeId);

        relationItem->setSourceNode(sourceNode);
        relationItem->setDestinationNode(destinationNode);

        m_ontologyView->scene()->addItem(relationItem);
      }
    }

    foreach (OLSOntologyGraphRelationItem *invalidRelation, invalidatedRelationsMap.values()) {
      m_ontologyView->scene()->removeItem(invalidRelation);
      if (invalidRelation->destinationNode() != nullptr)
        invalidRelation->destinationNode()->removeRelation(invalidRelation);
      if (invalidRelation->sourceNode() != nullptr)
        invalidRelation->sourceNode()->removeRelation(invalidRelation);
      delete invalidRelation;
    }

    foreach (const long &invalidNodeId, invalidatedNodesMap.keys()) {
      OLSOntologyGraphNodeItem *invalidNode = invalidatedNodesMap.value(invalidNodeId);
      m_dataController->setNodePosition(invalidNodeId, invalidNode->pos());
      m_ontologyView->scene()->removeItem(invalidNode);
      delete invalidNode;
    }
  }

  m_ontologyView->scene()->invalidate();
//  OLSAdaptiveNodeVisualizerMaster::findAndSetVisualizers(m_dataController);
}

void OLSOntologyGraphWidget::setRelationSlot() {

  QList<QGraphicsItem *> selectedItems = m_ontologyView->scene()->selectedItems();
  if (selectedItems.count() == 1) {
    QGraphicsItem *selectedItem = selectedItems.at(0);
    if (selectedItem->data(OLSOntologyGraphItemDataKey::kType).toInt() == OLSOntologyGraphItemType::kNode) {
      OLSOntologyGraphNodeItem *sourceNodeItem = static_cast<OLSOntologyGraphNodeItem *>(selectedItem);

      if (m_relationVisualizedLine != nullptr) {
        m_relationVisualizedLine->removeFromNodes();
        m_ontologyView->scene()->removeItem(m_relationVisualizedLine);
        delete m_relationVisualizedLine;
        m_relationVisualizedLine = nullptr;
      }

      m_relationVisualizedLine = new OLSOntologyGraphRelationVisualizedLine(m_dataController, m_relationVisualizeMaster);
      m_relationVisualizedLine->setSourceNode(sourceNodeItem);
      m_ontologyView->scene()->addItem(m_relationVisualizedLine);

      setEditRelationMode(true);
    }
  }
}

void OLSOntologyGraphWidget::editNodeSlot(QString newNamespace, QString newName) {

  QList<QGraphicsItem *> selectedItems = m_ontologyView->scene()->selectedItems();
  if (selectedItems.count() == 1) {
    QGraphicsItem *selectedItem = selectedItems.at(0);
    if (selectedItem->data(OLSOntologyGraphItemDataKey::kType).toInt() == OLSOntologyGraphItemType::kNode) {
      OLSOntologyGraphNodeItem *nodeItem = static_cast<OLSOntologyGraphNodeItem *>(selectedItem);
      bool ok = false;
      if (newName.isNull()) {
          QDialog dialog;
          dialog.setWindowTitle(tr("Enter node name"));
          QFormLayout *layout = new QFormLayout;
          QComboBox *namespaceComboBox = new QComboBox;
          for (QString prefix : dataController()->namespaces().keys() + dataController()->defaultNamespaces().keys())
            namespaceComboBox->insertItem(namespaceComboBox->count(), prefix);
          int currentNamespaceIndex = namespaceComboBox->findText(m_dataController->allNamespaces().key(m_dataController->getNodeById(nodeItem->id())->namespace_));
          if (currentNamespaceIndex != -1)
            namespaceComboBox->setCurrentIndex(currentNamespaceIndex);
          else
            namespaceComboBox->setCurrentIndex(namespaceComboBox->findText("default"));
          QLabel *namespaceLabel = new QLabel(tr("Namespace:"));
          QLineEdit *nameLineEdit = new QLineEdit(m_dataController->getNodeById(nodeItem->id())->name);
          QLabel *nameLabel = new QLabel(tr("Name:"));
          QDialogButtonBox *okCancelButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
          connect(okCancelButtonBox, &QDialogButtonBox::accepted, [&dialog, &ok, &newName, &newNamespace, nameLineEdit, namespaceComboBox, this](){
              newName = nameLineEdit->text().trimmed();
              if (newName.isEmpty()) {
                  QMessageBox(QMessageBox::Warning, tr("Rename error"), tr("Concept name is empty"), QMessageBox::Ok).exec();
                  return;
              }
              if (namespaceComboBox->currentIndex() == -1) {
                  QMessageBox(QMessageBox::Warning, tr("Rename error"), tr("Namespace is not selected"), QMessageBox::Ok).exec();
                  return;
              }
              newNamespace = dataController()->allNamespaces().value(namespaceComboBox->itemText(namespaceComboBox->currentIndex()));
              ok = true;
              dialog.close();
          });
          connect(okCancelButtonBox, &QDialogButtonBox::rejected, [&dialog](){
              dialog.close();
          });
          layout->addRow(namespaceLabel, namespaceComboBox);
          layout->addRow(nameLabel, nameLineEdit);
          layout->addWidget(okCancelButtonBox);
          dialog.setLayout(layout);
          dialog.exec();
      } else {
          ok = true;
      }
      if (ok) {
        if (m_dataController != nullptr) {
          m_dataController->nodeNameChanged(nodeItem->id(), newNamespace, newName);
          emit dataChangedSignal();
          emit undoAvailabilityChangedSignal(true);
        }
        m_ontologyView->scene()->invalidate();
      }
    }
  }
}

void OLSOntologyGraphWidget::editRelationSlot(QString newNamespace, QString newName) {

  QList<QGraphicsItem *> selectedItems = m_ontologyView->scene()->selectedItems();
  if (selectedItems.count() == 1) {
    QGraphicsItem *selectedItem = selectedItems.at(0);
    if (selectedItem->data(OLSOntologyGraphItemDataKey::kType).toInt() == OLSOntologyGraphItemType::kRelation) {
      OLSOntologyGraphRelationItem *relationItem = static_cast<OLSOntologyGraphRelationItem *>(selectedItem);
      bool ok = false;
      if (newName.isNull()) {
          QDialog dialog;
          dialog.setWindowTitle(tr("Enter relation name"));
          QFormLayout *layout = new QFormLayout;
          QComboBox *namespaceComboBox = new QComboBox;
          for (QString prefix : dataController()->namespaces().keys() + dataController()->defaultNamespaces().keys())
            namespaceComboBox->insertItem(namespaceComboBox->count(), prefix);
          int currentNamespaceIndex = namespaceComboBox->findText(m_dataController->allNamespaces().key(m_dataController->getRelationById(relationItem->id())->namespace_));
          if (currentNamespaceIndex != -1)
            namespaceComboBox->setCurrentIndex(currentNamespaceIndex);
          else
            namespaceComboBox->setCurrentIndex(namespaceComboBox->findText("default"));
          QLabel *namespaceLabel = new QLabel(tr("Namespace:"));
          QComboBox *nameComboBox = new QComboBox;
          nameComboBox->setEditable(true);
          int relationsCount = m_dataController->relationCount();
          for (int i = 0; i < relationsCount; ++i) {
              OLSOntologyRelationData *relation = m_dataController->getRelationByIndex(i);
              if (nameComboBox->findText(relation->name) == -1) {
                  nameComboBox->insertItem(nameComboBox->count(), relation->name);
              }
          }
          nameComboBox->setCurrentIndex(nameComboBox->findText(m_dataController->getRelationById(relationItem->id())->name));
          QLabel *nameLabel = new QLabel(tr("Name:"));
          QDialogButtonBox *okCancelButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
          connect(okCancelButtonBox, &QDialogButtonBox::accepted, [&dialog, &ok, &newName, &newNamespace, nameComboBox, namespaceComboBox, this](){
              newName = nameComboBox->currentText().trimmed();
              if (newName.isEmpty()) {
                  QMessageBox(QMessageBox::Warning, tr("Rename error"), tr("Relation name is empty"), QMessageBox::Ok).exec();
                  return;
              }
              if (namespaceComboBox->currentIndex() == -1) {
                  QMessageBox(QMessageBox::Warning, tr("Rename error"), tr("Namespace is not selected"), QMessageBox::Ok).exec();
                  return;
              }
              newNamespace = dataController()->allNamespaces().value(namespaceComboBox->itemText(namespaceComboBox->currentIndex()));
              ok = true;
              dialog.close();
          });
          connect(okCancelButtonBox, &QDialogButtonBox::rejected, [&dialog](){
              dialog.close();
          });
          layout->addRow(namespaceLabel, namespaceComboBox);
          layout->addRow(nameLabel, nameComboBox);
          layout->addWidget(okCancelButtonBox);
          dialog.setLayout(layout);
          dialog.exec();
      } else {
          ok = true;
      }
      if (ok) {
        if (m_dataController != nullptr) {
          m_dataController->relationNameChanged(relationItem->id(), newNamespace, newName);
          emit dataChangedSignal();
          emit undoAvailabilityChangedSignal(true);
        }
        m_ontologyView->scene()->invalidate();
      }
    }
  }
}

void OLSOntologyGraphWidget::removeSelectedSlot() {

  QList<QGraphicsItem *> selectedItems = m_ontologyView->scene()->selectedItems();
  foreach (QGraphicsItem *item, selectedItems) {
    if (item->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kNode) {
      OLSOntologyGraphNodeItem *nodeItem = static_cast<OLSOntologyGraphNodeItem *>(item);
      nodeItem->removeAllRelations();
      m_nodesToBeDisplayed.remove(nodeItem->id());
      if (m_dataController != nullptr) {
        m_dataController->nodeRemoved(nodeItem->id());
      }
    }
    else if (item->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kRelation) {
      OLSOntologyGraphRelationItem *relationItem = static_cast<OLSOntologyGraphRelationItem *>(item);
      relationItem->removeFromNodes();
      if (m_dataController != nullptr) {
        m_dataController->relationRemoved(relationItem->id());
      }
    }
    m_ontologyView->scene()->removeItem(item);
    delete item;
  }
  updateData();
  emit dataChangedSignal();
  emit undoAvailabilityChangedSignal(true);
}

void OLSOntologyGraphWidget::editAttrsSlot() {

  QGraphicsItem *selectedItem = m_ontologyView->scene()->selectedItems().at(0);
  OLSOntologyGraphElement *element = nullptr;
  if (selectedItem->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kNode) {
    element = static_cast<OLSOntologyGraphElement *>(static_cast<OLSOntologyGraphNodeItem *>(selectedItem));
  }
  else if (selectedItem->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kRelation) {
    element = static_cast<OLSOntologyGraphElement *>(static_cast<OLSOntologyGraphRelationItem *>(selectedItem));
  }

  Ui::OLSOntologyGraphAttributeEditor editor = Ui::OLSOntologyGraphAttributeEditor();

  QDialog *dialog = new QDialog(this);
  editor.setupUi(dialog);
  editor.textEdit->appendPlainText(element->attributesAsText());

  if (dialog->exec() == QDialog::Accepted) {
    element->setAttributesFromData(editor.textEdit->toPlainText().toLocal8Bit());
  }
}

void OLSOntologyGraphWidget::setSolidDashPatternSlot() {

  QGraphicsItem *selectedItem = m_ontologyView->scene()->selectedItems().at(0);
  OLSOntologyGraphElement *element = static_cast<OLSOntologyGraphElement *>(static_cast<OLSOntologyGraphRelationItem *>(selectedItem));
  QVariantMap attributesMap = element->attributes();
  QVariantMap dashMap;
  dashMap["dash"] = "solid";
  attributesMap["gui-attributes"] = dashMap;
  element->setAttributes(attributesMap);
}

void OLSOntologyGraphWidget::setDashDashPatternSlot() {

  QGraphicsItem *selectedItem = m_ontologyView->scene()->selectedItems().at(0);
  OLSOntologyGraphElement *element = static_cast<OLSOntologyGraphElement *>(static_cast<OLSOntologyGraphRelationItem *>(selectedItem));
  QVariantMap attributesMap = element->attributes();
  QVariantMap dashMap;
  dashMap["dash"] = "dash";
  attributesMap["gui-attributes"] = dashMap;
  element->setAttributes(attributesMap);
}

void OLSOntologyGraphWidget::setDotDashPatternSlot() {

  QGraphicsItem *selectedItem = m_ontologyView->scene()->selectedItems().at(0);
  OLSOntologyGraphElement *element = static_cast<OLSOntologyGraphElement *>(static_cast<OLSOntologyGraphRelationItem *>(selectedItem));
  QVariantMap attributesMap = element->attributes();
  QVariantMap dashMap;
  dashMap["dash"] = "dot";
  attributesMap["gui-attributes"] = dashMap;
  element->setAttributes(attributesMap);
}

void OLSOntologyGraphWidget::sceneSelectionChangedSlot() {

  if (m_editRelationMode) {
    QList<QGraphicsItem *> selectedItems = m_ontologyView->scene()->selectedItems();
    if (selectedItems.count() == 1) {
      QGraphicsItem *selectedItem = selectedItems.at(0);
      if (selectedItem->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kNode) {
        setRelation(m_relationVisualizedLine->sourceNode(), static_cast<OLSOntologyGraphNodeItem *>(selectedItem));
        setEditRelationMode(false);
      }
      else {
        m_ontologyView->scene()->clearSelection();
      }
    }
  }
}

void OLSOntologyGraphWidget::ontologyViewMousePositionChangedSlot(const QPoint &pos) {

  if (m_relationVisualizedLine != nullptr) {
    QPointF scenePos = m_ontologyView->mapToScene(pos);
    m_relationVisualizedLine->setEndPoint(scenePos);
  }
}

void OLSOntologyGraphWidget::dataChangedSlot() {

  updateData();
}

void OLSOntologyGraphWidget::itemSelectedSlot(long id) {

  m_ontologyView->scene()->clearSelection();

  foreach (QGraphicsItem *item, m_ontologyView->scene()->items()) {
    if (item->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kNode) {
      OLSOntologyGraphNodeItem *nodeItem = static_cast<OLSOntologyGraphNodeItem *>(item);
      if (nodeItem->id() == id) {
        item->setSelected(true);
        m_ontologyView->centerOn(item);
      }
    }
    else if (item->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kRelation) {
      OLSOntologyGraphRelationItem *relationItem = static_cast<OLSOntologyGraphRelationItem *>(item);
      if (relationItem->id() == id) {
        item->setSelected(true);
        m_ontologyView->centerOn(item);
      }
    }
  }
}

void OLSOntologyGraphWidget::itemsSelectedSlot(const QSet<long> ids) {

  m_ontologyView->scene()->clearSelection();

  bool alreadyCentered = false;
  foreach (QGraphicsItem *item, m_ontologyView->scene()->items()) {
    if (item->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kNode) {
      OLSOntologyGraphNodeItem *nodeItem = static_cast<OLSOntologyGraphNodeItem *>(item);
      if (ids.contains(nodeItem->id())) {
        item->setSelected(true);
        if (!alreadyCentered) {
          m_ontologyView->centerOn(item);
          alreadyCentered = true;
        }
      }
    }
//    else if (item->data(OLSOntologyGraphItemDataKey::kType) == OLSOntologyGraphItemType::kRelation) {
//      RelationItem *relationItem = static_cast<RelationItem *>(item);
//      if (relationItem->id() == id) {
//        item->setSelected(true);
//        m_ontologyView->centerOn(item);
//      }
//    }
  }
}

void OLSOntologyGraphWidget::zoomInSlot() {

  m_ontologyView->scale(1.2, 1.2);
  adjustZoom();
}

void OLSOntologyGraphWidget::zoomOutSlot() {

  m_ontologyView->scale(0.8, 0.8);
  adjustZoom();
}

void OLSOntologyGraphWidget::adjustZoom()
{
    m_ontologyView->scene()->setBackgroundBrush(QBrush(Qt::NoBrush));
    QRectF unity = m_ontologyView->matrix().mapRect(QRectF(0, 0, 1, 1));
    if (fabs(unity.width() - 1.0) < 0.19) {
        QBrush bgBrush = QBrush(Qt::Dense7Pattern);
        bgBrush.setColor(QColor::fromRgb(214, 209, 192));
        m_ontologyView->scene()->setBackgroundBrush(bgBrush);
        m_ontologyView->scale(1 / unity.width(), 1 / unity.height());
    }
}

OLSOntologyGraphElement *OLSOntologyGraphWidget::getGraphElementById(long id)
{
    OLSOntologyGraphNodeItem *graphElement;
    for (QGraphicsItem *item: m_ontologyView->scene()->items()) {
        graphElement = static_cast<OLSOntologyGraphNodeItem *>(item);
        if (graphElement->id() == id)
            return graphElement;
    }
    qDebug() << "getGraphElementById not exist error";
    throw;
}

void OLSOntologyGraphWidget::undoSlot()
{
    OLSHistoryRecord *historyRecord = m_dataController->latestChange();
    if (historyRecord == nullptr)
        return;
    bool originalHistorySavingState = m_dataController->isHistorySaving();
    m_dataController->setHistorySavingState(false);
    switch (historyRecord->recordType()) {
    case HistoryRecordType::NodeCreated: {
        OLSHistoryNodeCreatedRecord *record = static_cast<OLSHistoryNodeCreatedRecord *>(historyRecord);
        itemSelectedSlot(record->nodeId);
        removeSelectedSlot();
        break;
    }
    case HistoryRecordType::NodeNameChanged: {
        OLSHistoryNodeNameChangedRecord *record = static_cast<OLSHistoryNodeNameChangedRecord *>(historyRecord);
        itemSelectedSlot(record->nodeId);
        editNodeSlot(record->oldName);
        break;
    }
    case HistoryRecordType::NodeRemoved: {
        OLSHistoryNodeRemovedRecord *record = static_cast<OLSHistoryNodeRemovedRecord *>(historyRecord);
        addNode(record->position, record->nodeId);
        itemSelectedSlot(record->nodeId);
        editNodeSlot(record->name);
        break;
    }
    case HistoryRecordType::RelationCreated: {
        OLSHistoryRelationCreatedRecord *record = static_cast<OLSHistoryRelationCreatedRecord *>(historyRecord);
        itemSelectedSlot(record->relationId);
        removeSelectedSlot();
        break;
    }
    case HistoryRecordType::RelationNameChanged: {
        OLSHistoryRelationNameChangedRecord *record = static_cast<OLSHistoryRelationNameChangedRecord *>(historyRecord);
        itemSelectedSlot(record->relationId);
        editRelationSlot(record->oldName);
        break;
    }
    case HistoryRecordType::RelationRemoved: {
        OLSHistoryRelationRemovedRecord *record = static_cast<OLSHistoryRelationRemovedRecord *>(historyRecord);
        OLSOntologyGraphNodeItem *source = static_cast<OLSOntologyGraphNodeItem *>(getGraphElementById(record->sourceId));
        OLSOntologyGraphNodeItem *destination = static_cast<OLSOntologyGraphNodeItem *>(getGraphElementById(record->destinationId));
        setRelation(source, destination, record->relationId);
        itemSelectedSlot(record->relationId);
        editRelationSlot(record->name);
        break;
    }
    default: {
        qDebug() << "Unsupported HistoryRecordType";
        throw;
    }
    }
    m_dataController->setHistorySavingState(originalHistorySavingState);
    m_dataController->shiftHistoryChangeBackward();
    emit undoAvailabilityChangedSignal(m_dataController->latestChange() != nullptr);
    emit redoAvailabilityChangedSignal(m_dataController->latestUndoChange() != nullptr);
}

void OLSOntologyGraphWidget::redoSlot()
{
    OLSHistoryRecord *historyRecord = m_dataController->latestUndoChange();
    if (historyRecord == nullptr)
        return;
    switch (historyRecord->recordType()) {
    case HistoryRecordType::NodeCreated: {
        OLSHistoryNodeCreatedRecord *record = static_cast<OLSHistoryNodeCreatedRecord *>(historyRecord);
        addNode(record->position, record->nodeId);
        itemSelectedSlot(record->nodeId);
        break;
    }
    case HistoryRecordType::NodeNameChanged: {
        OLSHistoryNodeNameChangedRecord *record = static_cast<OLSHistoryNodeNameChangedRecord *>(historyRecord);
        itemSelectedSlot(record->nodeId);
        editNodeSlot(record->newNamespace, record->newName);
        break;
    }
    case HistoryRecordType::NodeRemoved: {
        OLSHistoryNodeRemovedRecord *record = static_cast<OLSHistoryNodeRemovedRecord *>(historyRecord);
        itemSelectedSlot(record->nodeId);
        removeSelectedSlot();
        break;
    }
    case HistoryRecordType::RelationCreated: {
        OLSHistoryRelationCreatedRecord *record = static_cast<OLSHistoryRelationCreatedRecord *>(historyRecord);
        OLSOntologyGraphNodeItem *source = static_cast<OLSOntologyGraphNodeItem *>(getGraphElementById(record->sourceId));
        OLSOntologyGraphNodeItem *destination = static_cast<OLSOntologyGraphNodeItem *>(getGraphElementById(record->destinationId));
        setRelation(source, destination, record->relationId);
        itemSelectedSlot(record->relationId);
        break;
    }
    case HistoryRecordType::RelationNameChanged: {
        OLSHistoryRelationNameChangedRecord *record = static_cast<OLSHistoryRelationNameChangedRecord *>(historyRecord);
        itemSelectedSlot(record->relationId);
        editRelationSlot(record->newName);
        break;
    }
    case HistoryRecordType::RelationRemoved: {
        OLSHistoryRelationRemovedRecord *record = static_cast<OLSHistoryRelationRemovedRecord *>(historyRecord);
        itemSelectedSlot(record->relationId);
        removeSelectedSlot();
        break;
    }
    default: {
        qDebug() << "Unsupported HistoryRecordType";
        throw;
    }
    }
    emit undoAvailabilityChangedSignal(m_dataController->latestChange() != nullptr);
    emit redoAvailabilityChangedSignal(m_dataController->latestUndoChange() != nullptr);
}

void OLSOntologyGraphWidget::editNamespacesSlot()
{
    QDialog dialog;
    QTableWidget *table = new QTableWidget(0, 2);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setHorizontalHeaderLabels(QStringList { tr("Prefix"), tr("Value") });
    table->horizontalHeader()->setStretchLastSection(true);
    QMap<QString, QString> defaultNamespaces = dataController()->defaultNamespaces();
    for (QString prefix : defaultNamespaces.keys()) {
        int i = table->rowCount();
        table->insertRow(i);
        auto prefixItem = new QTableWidgetItem(prefix);
        auto namespaceItem = new QTableWidgetItem(defaultNamespaces.value(prefix));
        prefixItem->setFlags(prefixItem->flags() & ~Qt::ItemIsEditable);
        namespaceItem->setFlags(namespaceItem->flags() & ~Qt::ItemIsEditable);
        table->setItem(i, 0, prefixItem);
        table->setItem(i, 1, namespaceItem);
    }
    QMap<QString, QString> oldNamespaces = dataController()->namespaces();
    for (QString prefix : oldNamespaces.keys()) {
        int i = table->rowCount();
        table->insertRow(i);
        auto prefixItem = new QTableWidgetItem(prefix);
        auto namespaceItem = new QTableWidgetItem(oldNamespaces.value(prefix));
        table->setItem(i, 0, prefixItem);
        table->setItem(i, 1, namespaceItem);
    }
    QSet<QString> namespacesWithoutPrefixes;
    for (int i = 0; i < dataController()->nodeCount(); ++i) {
        OLSOntologyNodeData *node = dataController()->getNodeByIndex(i);
        if (defaultNamespaces.key(node->namespace_).isEmpty() && oldNamespaces.key(node->namespace_).isEmpty())
            namespacesWithoutPrefixes << node->namespace_;
    }
    for (QString namespace_ : namespacesWithoutPrefixes) {
        int i = table->rowCount();
        table->insertRow(i);
        auto prefixItem = new QTableWidgetItem(namespace_);
        auto namespaceItem = new QTableWidgetItem(namespace_);
        table->setItem(i, 0, prefixItem);
        table->setItem(i, 1, namespaceItem);
    }
    table->resizeColumnsToContents();
    QVBoxLayout *layout = new QVBoxLayout;
    QDialogButtonBox *addRemoveButtonBox = new QDialogButtonBox;
    QPushButton *addButton = new QPushButton(tr("Add row"));
    addRemoveButtonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    connect(addButton, &QPushButton::pressed, [table](){
        table->insertRow(table->rowCount());
    });
    QPushButton *removeButton = new QPushButton(tr("Remove row"));
    addRemoveButtonBox->addButton(removeButton, QDialogButtonBox::ActionRole);
    connect(removeButton, &QPushButton::pressed, [table, defaultNamespaces](){
        for (QTableWidgetSelectionRange range : table->selectedRanges()) {
            if (range.topRow() > defaultNamespaces.size() - 1)
                table->removeRow(range.topRow());
            else
                QMessageBox(QMessageBox::Warning, tr("Remove error"), tr("Removing default namespaces is forbidden"), QMessageBox::Ok).exec();
        }
    });
    QDialogButtonBox *okCancelButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(okCancelButtonBox, &QDialogButtonBox::accepted, [&dialog, defaultNamespaces, table, this](){
        QMap<QString, QString> newNamespaces;
        for (int row = defaultNamespaces.size(); row < table->rowCount(); ++row) {
            QString prefix = table->item(row, 0) != nullptr ? table->item(row, 0)->text().trimmed().toLower() : QString();
            QString value = table->item(row, 1) != nullptr ? table->item(row, 1)->text().trimmed() : QString();
            if (prefix.isEmpty() || value.isEmpty()) {
                QMessageBox(QMessageBox::Warning, tr("Namespace save error"), tr("Prefix or value can't be empty"), QMessageBox::Ok).exec();
                return;
            }
            if (defaultNamespaces.contains(prefix) || newNamespaces.contains(prefix)) {
                QMessageBox(QMessageBox::Warning, tr("Namespace save error"), tr("Prefix name duplication"), QMessageBox::Ok).exec();
                return;
            }
            newNamespaces.insert(prefix, value);
        }
        dataController()->setNamespaces(newNamespaces);
        dialog.close();
    });
    connect(okCancelButtonBox, &QDialogButtonBox::rejected, [&dialog](){
        dialog.close();
    });
    layout->addWidget(table);
    layout->addWidget(addRemoveButtonBox, 0, Qt::AlignLeft);
    layout->addWidget(okCancelButtonBox);
    dialog.setLayout(layout);
    dialog.resize(400, 300);
    dialog.exec();
}

void OLSOntologyGraphWidget::nodeItemDoubleClickSlot(long id, Qt::MouseButtons mouseButtons)
{
    if (mouseButtons == Qt::LeftButton) {
        for (long relationId: m_dataController->getNodeById(id)->relations) {
            OLSOntologyRelationData *relation = m_dataController->getRelationById(relationId);
            if (relation != nullptr)
                m_nodesToBeDisplayed << relation->sourceNodeId << relation->destinationNodeId;
        }
        updateData();
    }
    else if (mouseButtons == Qt::MiddleButton) {
        m_nodesToBeDisplayed.remove(id);
        QList<long> nodesToBeNotDisplayed;
        for (long nodeId: m_nodesToBeDisplayed) {
            bool toRemove = true;
            OLSOntologyNodeData *nodeData = m_dataController->getNodeById(nodeId);
            if (nodeData == nullptr)
                continue;
            if (std::all_of(nodeData->relations.cbegin(), nodeData->relations.cend(), [nodeId, this](long relationId){
                            return m_dataController->getRelationById(relationId) != nullptr && m_dataController->getRelationById(relationId)->sourceNodeId == nodeId;}))
                continue;
            for (long relationId: nodeData->relations) {
                OLSOntologyRelationData *relation = m_dataController->getRelationById(relationId);
                if (relation != nullptr)
                    if ((m_nodesToBeDisplayed.contains(relation->sourceNodeId) && (relation->sourceNodeId != nodeData->id)) ||
                        (m_nodesToBeDisplayed.contains(relation->destinationNodeId) && (relation->destinationNodeId != nodeData->id))) {
                        toRemove = false;
                        break;
                    }
            }
            if (toRemove)
                nodesToBeNotDisplayed << nodeId;
        }
        for (long id: nodesToBeNotDisplayed)
            m_nodesToBeDisplayed.remove(id);
        m_nodesToBeDisplayed << id;
        updateData();
    } else {
        return;
    }
}

void OLSOntologyGraphWidget::nodeItemPositionChangedSlot(long id, const QPointF &newPosition) {

  m_dataController->setNodePosition(id, newPosition);
}

QImage OLSOntologyGraphWidget::makeScreenshot() const {

  QImage image(m_ontologyView->scene()->width(), m_ontologyView->scene()->height(), QImage::Format_ARGB32_Premultiplied);
  QPainter painter(&image);
  m_ontologyView->scene()->render(&painter);
  return image;
}

void OLSOntologyGraphWidget::showOntologySlot() {

  if (m_sourceCodeShown) {
    m_sourceCodeShown = false;
    m_sourceCodeViewer->setVisible(false);
    ui->verticalLayout->removeWidget(m_sourceCodeViewer);
    ui->verticalLayout->addWidget(m_ontologyView);
    m_ontologyView->setVisible(true);
  }
}

//void OLSOntologyGraphWidget::dropEvent(QDropEvent *event) {

//  QByteArray jsonData = event->mimeData()->data("application/node-attrs");
//  QPointF globalPos = m_ontologyView->mapToScene(event->pos());
//  OLSOntologyGraphNodeItem *newNode = addNode(globalPos);
//  newNode->setAttributesFromData(jsonData);
//}

//void OLSOntologyGraphWidget::dragEnterEvent(QDragEnterEvent *event) {

//  if (event->mimeData()->hasFormat("application/node-attrs")) {
//    event->acceptProposedAction();
//  }
//  else {
//    event->ignore();
//  }
//}

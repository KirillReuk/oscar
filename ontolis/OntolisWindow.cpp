#include "OntolisWindow.h"
#include "ui_OntolisWindow.h"

#include <QFile>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QTextStream>
#include <QTableWidget>
#include <QDialogButtonBox>
#include "IOHelpers.hpp"
#include "../ontolis-lib/extensions/OLSAdaptiveRelationVisualizerMaster.h"
#include "../ontolis-lib/widgets/Shared/OLSSettings.h"

OntolisWindow::OntolisWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::OntolisWindow)
{

  ui->setupUi(this);
  ui->tabWidget->setTabsClosable(true);

  m_ontologyTreeViewController = new OLSOntologyTreeViewController();
  ui->ontologyTreeLayout->addWidget(m_ontologyTreeViewController->treeView());

  m_logTreeView = new QTreeView();
  m_logTreeView->setHeaderHidden(true);
  m_logTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_logTreeView->hide();

  m_zoomInShortcut = new QShortcut(this);
  m_zoomInShortcut->setKey(QKeySequence::ZoomIn);
  m_zoomInShortcut->setEnabled(true);

  m_zoomOutShortcut = new QShortcut(this);
  m_zoomOutShortcut->setKey(QKeySequence::ZoomOut);
  m_zoomOutShortcut->setEnabled(true);

  m_removeShortcut = new QShortcut(this);
  m_removeShortcut->setKey(QKeySequence::Delete);
  m_removeShortcut->setEnabled(true);

  m_currentSnapshotIndex = 0;

  connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(currentTabChangedSlot(int)));
  connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &OntolisWindow::closeOntologySlot);

  m_settings = new OLSSettings();

  setupMenu();
  setupConverters();
  setupTransformators();
}

OntolisWindow::~OntolisWindow() {

  delete ui;
}

void OntolisWindow::closeOntologySlot(int index)
{
    if (index < 0)
        return;
    m_currentProject.deleteProjectFile(index);
    m_openOntologyWidgets.removeAt(index);
    if (ui->tabWidget->count() == 1)
        disconnect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChangedSlot(int)));
    ui->tabWidget->removeTab(index);
    if (ui->tabWidget->count() == 0) {
        connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(currentTabChangedSlot(int)));
        m_ontologyTreeViewController->setDataController(nullptr);
        m_ontologyTreeViewController->updateData();
        m_undoAction->setEnabled(false);
        m_redoAction->setEnabled(false);
        m_editNamespaces->setEnabled(false);
    }
}

void OntolisWindow::exitSlot()
{
    qApp->exit(0);
}

OLSOntologyGraphWidget * OntolisWindow::createNewOntologyWidget(OLSProjectFile *file) {

  OLSOntologyGraphWidget *newWidget = new OLSOntologyGraphWidget(this);
  newWidget->setDataController(file->ontologyController());
  m_openOntologyWidgets.append(newWidget);
  ui->tabWidget->addTab(newWidget, file->name());

  return newWidget;
}

void OntolisWindow::clearConnections() {

  m_ontologyTreeViewController->disconnect();
  m_zoomInShortcut->disconnect();
  m_zoomOutShortcut->disconnect();
  m_removeShortcut->disconnect();
  m_undoAction->disconnect();
  m_redoAction->disconnect();
  m_editNamespaces->disconnect();
}

void OntolisWindow::updateOntologyTreeData() {

  int index = ui->tabWidget->currentIndex();
  OLSOntologyGraphWidget *widget = m_openOntologyWidgets[index];
  m_ontologyTreeViewController->setDataController(widget->dataController());
  m_ontologyTreeViewController->updateData();
}

void OntolisWindow::setupConverters() {

  QDir dir(QApplication::applicationDirPath() + "/../Scripts");

  qDebug() << dir.path();

  foreach (QString fileName, dir.entryList(QDir::Files)) {
    if (fileName.contains("converter")) {
      QString cmd;
#ifdef Q_OS_WIN32
      cmd.append("python ");
#endif
      cmd.append(dir.absoluteFilePath(fileName));
      cmd.append(" --method=supported_extensions");

      QProcess process;
      process.start(cmd, QIODevice::ReadOnly);
      process.waitForFinished();
      QString result(process.readAllStandardOutput());
      qDebug() << result;

      QStringList extensions = result.split(":");
      foreach (QString extension, extensions) {
        m_convertersMapping.insert(extension.trimmed(), dir.absoluteFilePath(fileName));
      }
    }
  }
}

void OntolisWindow::setupTransformators() {

  // do nothing now
}

void OntolisWindow::setupMenu() {

  QMenu *fileMenu = ui->menubar->addMenu(tr("&File"));

  //QAction *importSourceFileAction = fileMenu->addAction(tr("Import source file..."));

  fileMenu->addSeparator();

  QAction *createOntologyAction = fileMenu->addAction(tr("&New Ontology..."));
  createOntologyAction->setShortcut(QKeySequence::New);
  QAction *openOntologyAction = fileMenu->addAction(tr("&Open Ontology..."));
  openOntologyAction->setShortcut(QKeySequence::Open);
  QAction *saveOntologyAction = fileMenu->addAction(tr("&Save Ontology..."));
  saveOntologyAction->setShortcut(QKeySequence::Save);
  QAction *saveOntologyAsAction = fileMenu->addAction(tr("Save Ontology &As..."));
  saveOntologyAsAction->setShortcut(QKeySequence::SaveAs);

  fileMenu->addSeparator();

//  QAction *exportFileAction = fileMenu->addAction(tr("Export file..."));

//  fileMenu->addSeparator();

//  QAction *screenshotAction = fileMenu->addAction(tr("Screenshot..."));

//  fileMenu->addSeparator();

  QAction *closeOntologyAction = fileMenu->addAction(tr("Close"));
  closeOntologyAction->setShortcut(QKeySequence::Close);

  QAction *exitAction = fileMenu->addAction(tr("E&xit"));
  exitAction->setShortcut(QKeySequence::Quit);

  QMenu *editMenu = ui->menubar->addMenu(tr("&Edit"));
  m_undoAction = editMenu->addAction(style()->standardIcon(QStyle::SP_ArrowBack), tr("&Undo"));
  m_undoAction->setShortcut(QKeySequence::Undo);
  m_undoAction->setEnabled(false);
  m_redoAction = editMenu->addAction(style()->standardIcon(QStyle::SP_ArrowForward), tr("&Redo"));
  m_redoAction->setShortcut(QKeySequence::Redo);
  m_redoAction->setEnabled(false);
  editMenu->addSeparator();
  m_editNamespaces = editMenu->addAction(tr("Namespaces..."));
  m_editNamespaces->setEnabled(false);
  editMenu->addSeparator();
  m_editEvents = editMenu->addAction(tr("Edit events"));
  connect(m_editEvents, &QAction::triggered, this, &OntolisWindow::editEventsSlot);

  QMenu *viewMenu = ui->menubar->addMenu(tr("&View"));
  QMenu *nameRegimeMenu = viewMenu->addMenu(tr("&Name regime"));
  QAction *localNameRegimeAction = nameRegimeMenu->addAction(tr("&Local"));
  QAction *qnameNameRegimeAction = nameRegimeMenu->addAction(tr("&Qualified name"));
  QAction *uriNameRegimeAction = nameRegimeMenu->addAction(tr("&URI"));
  localNameRegimeAction->setCheckable(true);
  qnameNameRegimeAction->setCheckable(true);
  uriNameRegimeAction->setCheckable(true);
  QActionGroup *group = new QActionGroup(this);
  localNameRegimeAction->setActionGroup(group);
  qnameNameRegimeAction->setActionGroup(group);
  uriNameRegimeAction->setActionGroup(group);
  connect(localNameRegimeAction, &QAction::changed, [this](){
      qApp->setProperty(ONTOLIS_NAME_REGIME, ONTOLIS_NAME_REGIME_LOCAL);
      if (ui->tabWidget->currentIndex() != -1)
          m_openOntologyWidgets[ui->tabWidget->currentIndex()]->repaint();
  });
  connect(qnameNameRegimeAction, &QAction::changed, [this](){
      qApp->setProperty(ONTOLIS_NAME_REGIME, ONTOLIS_NAME_REGIME_QNAME);
      if (ui->tabWidget->currentIndex() != -1)
          m_openOntologyWidgets[ui->tabWidget->currentIndex()]->repaint();
  });
  connect(uriNameRegimeAction, &QAction::changed, [this](){
      qApp->setProperty(ONTOLIS_NAME_REGIME, ONTOLIS_NAME_REGIME_URI);
      if (ui->tabWidget->currentIndex() != -1)
          m_openOntologyWidgets[ui->tabWidget->currentIndex()]->repaint();
  });
  localNameRegimeAction->setChecked(true);

  QMenu *graphicsMenu = ui->menubar->addMenu(tr("Graphics"));
  m_modifyMenu = graphicsMenu->addMenu(tr("Modify"));

  QMenu *relationsMenu = m_modifyMenu->addMenu(tr("Relations"));
  QAction *defaultRelationAction = relationsMenu->addAction(tr("Default"));
  QAction *customRelationsAction = relationsMenu->addAction(tr("Custom"));

  QAction *applyAction = graphicsMenu->addAction(tr("Apply"));

  //connect(importSourceFileAction, SIGNAL(triggered()), SLOT(importSourceFileSlot()));

  connect(createOntologyAction, &QAction::triggered, this, &OntolisWindow::createOntologyFileSlot);
  connect(openOntologyAction, &QAction::triggered, this, &OntolisWindow::openOntologyFileSlot);
  connect(saveOntologyAction, &QAction::triggered, this, &OntolisWindow::saveOntologyFileSlot);
  connect(saveOntologyAsAction, &QAction::triggered, this, &OntolisWindow::saveOntologyAsFileSlot);

//  connect(exportFileAction, SIGNAL(triggered()), SLOT(exportFileSlot()));

//  connect(screenshotAction, SIGNAL(triggered()), SLOT(screenshotSlot()));

  connect(closeOntologyAction, &QAction::triggered, this, [this]() { closeOntologySlot(ui->tabWidget->currentIndex()); });
  connect(exitAction, &QAction::triggered, this, &OntolisWindow::exitSlot);

  connect(defaultRelationAction, &QAction::triggered, [this](){editRelationVisualizationParametersSlot(OLSAdaptiveRelationVisualizerMaster::defaultVisualizerId);});
  connect(customRelationsAction, &QAction::triggered, this, &OntolisWindow::selectRelationTypeSlot);

  connect(applyAction, SIGNAL(triggered()), SLOT(applyGraphicSettingsSlot()));
}

void OntolisWindow::editRelationVisualizationParametersSlot(RelationTypeId typeId)
{
    if (ui->tabWidget->count() == 0)
        return;
    OLSAdaptiveRelationVisualizerMaster *relationVisualizerMaster = m_openOntologyWidgets[ui->tabWidget->currentIndex()]->relationVisualizeMaster();
    QDialog settingsWindow;
    OLSAdaptiveRelationVisualizerParameters *params = relationVisualizerMaster->createOrGetVisualizationParameters(typeId);
    OLSAdaptiveRelationVisualizerParameters *defaults = relationVisualizerMaster->getVisualizationParameters(OLSAdaptiveRelationVisualizerMaster::defaultVisualizerId);

    QComboBox *cbArrowType = new QComboBox(&settingsWindow);
    cbArrowType->addItem(tr("Two Lines"), ArrowType::TwoLines);
    cbArrowType->addItem(tr("Empty Triangle"), ArrowType::EmptyTriangle);
    cbArrowType->addItem(tr("Filled Triangle"), ArrowType::FilledTriangle);
    QCheckBox *chbArrowType = new QCheckBox(tr("Arrow type:"), &settingsWindow);
    connect(chbArrowType, &QCheckBox::toggled, [cbArrowType](bool checked){
        cbArrowType->setEnabled(checked);
    });
    if (params->isParameterSet(RelationAvisParameter::ArrowType)) {
        chbArrowType->setChecked(true);
        cbArrowType->setCurrentIndex(cbArrowType->findData(params->arrowType()));
        if (typeId == OLSAdaptiveRelationVisualizerMaster::defaultVisualizerId) {
            chbArrowType->setStyleSheet("QCheckBox{color: black;}");
            chbArrowType->setEnabled(false);
        }
    } else {
        cbArrowType->setEnabled(false);
        cbArrowType->setCurrentIndex(cbArrowType->findData(defaults->arrowType()));
    }

    QSlider *sArrowPosition = new QSlider(Qt::Horizontal, &settingsWindow);
    QCheckBox *chbArrowPosition = new QCheckBox(tr("Arrow position:"), &settingsWindow);
    connect(chbArrowPosition, &QCheckBox::toggled, [sArrowPosition](bool checked){
        sArrowPosition->setEnabled(checked);
    });
    if (params->isParameterSet(RelationAvisParameter::ArrowPosition)) {
        chbArrowPosition->setChecked(true);
        sArrowPosition->setValue(sArrowPosition->maximum() * params->arrowPosition());
        if (typeId == OLSAdaptiveRelationVisualizerMaster::defaultVisualizerId) {
            chbArrowPosition->setStyleSheet("QCheckBox{color: black;}");
            chbArrowPosition->setEnabled(false);
        }
    } else {
        sArrowPosition->setEnabled(false);
        sArrowPosition->setValue((sArrowPosition->maximum() - sArrowPosition->minimum()) * defaults->arrowPosition());
    }

    QSlider *sTextPosition = new QSlider(Qt::Horizontal, &settingsWindow);
    QCheckBox *chbTextPosition = new QCheckBox(tr("Text position:"), &settingsWindow);
    connect(chbTextPosition, &QCheckBox::toggled, [sTextPosition](bool checked){
        sTextPosition->setEnabled(checked);
    });
    if (params->isParameterSet(RelationAvisParameter::TextPosition)) {
        chbTextPosition->setChecked(true);
        sTextPosition->setValue(sTextPosition->maximum() * params->textPosition());
        if (typeId == OLSAdaptiveRelationVisualizerMaster::defaultVisualizerId) {
            chbTextPosition->setStyleSheet("QCheckBox{color: black;}");
            chbTextPosition->setEnabled(true);
        }
    } else {
        sTextPosition->setEnabled(false);
        sTextPosition->setValue((sTextPosition->maximum() - sTextPosition->minimum()) * defaults->textPosition());
    }


    QSlider *sEdgeWidth = new QSlider(Qt::Horizontal, &settingsWindow);
    QCheckBox *chbEdgeWidth = new QCheckBox(tr("Edge width:"), &settingsWindow);
    connect(chbEdgeWidth, &QCheckBox::toggled, [sEdgeWidth](bool checked){
        sEdgeWidth->setEnabled(checked);
    });
    if (params->isParameterSet(RelationAvisParameter::EdgeWidth)) {
        chbEdgeWidth->setChecked(true);
        sEdgeWidth->setValue(sEdgeWidth->maximum() * params->edgeWidth());
        if (typeId == OLSAdaptiveRelationVisualizerMaster::defaultVisualizerId) {
            chbEdgeWidth->setStyleSheet("QCheckBox{color: black;}");
            chbEdgeWidth->setEnabled(true);
        }
    } else {
        sEdgeWidth->setEnabled(true);
        sEdgeWidth->setValue((sEdgeWidth->maximum() - sEdgeWidth->minimum()) * defaults->edgeWidth());
    }

    QDialogButtonBox *settingsOkCancelButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &settingsWindow);
    connect(settingsOkCancelButtonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, &settingsWindow, &QDialog::accept);
    connect(settingsOkCancelButtonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, &settingsWindow, &QDialog::reject);

    QFormLayout *formLayout = new QFormLayout(&settingsWindow);
    formLayout->addRow(chbArrowType, cbArrowType);
    formLayout->addRow(chbArrowPosition, sArrowPosition);
    formLayout->addRow(chbTextPosition, sTextPosition);
    formLayout->addRow(chbEdgeWidth, sEdgeWidth);
    formLayout->addRow(settingsOkCancelButtonBox);
    settingsWindow.setLayout(formLayout);
    settingsWindow.setWindowTitle(tr("Visualization parameters"));

    if (settingsWindow.exec() == QDialog::Accepted) {
        if (chbArrowType->isChecked())
            relationVisualizerMaster->setArrowType(typeId, static_cast<ArrowType>(cbArrowType->itemData(cbArrowType->currentIndex()).toInt()));
        else
            relationVisualizerMaster->resetArrowType(typeId);
        if (chbArrowPosition->isChecked())
            relationVisualizerMaster->setArrowPosition(typeId, (sArrowPosition->value() - sArrowPosition->minimum()) * 1.0 / (sArrowPosition->maximum() - sArrowPosition->minimum()));
        else
            relationVisualizerMaster->resetArrowPosition(typeId);
        if (chbTextPosition->isChecked())
            relationVisualizerMaster->setTextPosition(typeId, (sTextPosition->value() - sTextPosition->minimum()) * 1.0 / (sTextPosition->maximum() - sTextPosition->minimum()));
        else
            relationVisualizerMaster->resetTextPosition(typeId);
        if (chbEdgeWidth->isChecked())
            relationVisualizerMaster->setEdgeWidth(typeId, (sEdgeWidth->value() - sEdgeWidth->minimum()) * 1.0 / (sEdgeWidth->maximum() - sEdgeWidth->minimum()));
        else
            relationVisualizerMaster->resetEdgeWidth(typeId);
    }
}

void OntolisWindow::editEventsSlot()
{
    if (ui->tabWidget->count() == 0)
        return; //если онтологий не открыто, выйти

    QWidget *window = new QWidget();

    QSet<QString> relationEventIds;
    relationEventIds << "OnClick" << "OnRightClick" << "OnDoubleClick" << "OnFocus" << "OnMouseEnter" << "OnMouseLeave";

    QListView *relationEventList = new QListView(window);
    relationEventList->setSelectionMode(QListView::SingleSelection);
    QStandardItemModel *relationEventsModel = new QStandardItemModel(window);
    QSortFilterProxyModel *relationEventsProxyModel = new QSortFilterProxyModel(window);
    relationEventsProxyModel->setSourceModel(relationEventsModel);
    relationEventsProxyModel->setFilterKeyColumn(0);
    relationEventsProxyModel->sort(0, Qt::AscendingOrder);
    relationEventList->setModel(relationEventsProxyModel);
    int relationEventTypesCount = 0;
    for (const QString &typeId : relationEventIds) {
        auto item = new QStandardItem(typeId);
        item->setEditable(false);
        relationEventsModel->setItem(relationEventTypesCount++, item);
    }
    QLineEdit *relationEventsSearchLine = new QLineEdit(window);
    relationEventsSearchLine->setClearButtonEnabled(true);
    QObject::connect(relationEventsSearchLine, &QLineEdit::textChanged, [relationEventsProxyModel](const QString & text){
        relationEventsProxyModel->setFilterRegExp(QRegExp(text.trimmed(), Qt::CaseInsensitive, QRegExp::FixedString));
    });
    QGroupBox *relationEventsGroupBox = new QGroupBox(tr("Events"), window);
    QVBoxLayout *relationEventsGroupBoxLayout = new QVBoxLayout(window);
    relationEventsGroupBoxLayout->addWidget(relationEventsSearchLine);
    relationEventsGroupBox->setLayout(relationEventsGroupBoxLayout);

    QVBoxLayout *relationEventsLayout = new QVBoxLayout();
    relationEventsLayout->addWidget(relationEventsGroupBox);
    relationEventsLayout->addWidget(relationEventList);

    //  ^  events  ^--------------------------------------------------------------

    QSet<QString> relationActionIds;
    relationActionIds << "Highlight" << "Highlight With Adjacent" << "Open Options" << "Show Tip" << "Delete";

    QListView *relationActionList = new QListView(window);
    relationActionList->setSelectionMode(QListView::SingleSelection);
    QStandardItemModel *relationActionsModel = new QStandardItemModel(window);
    QSortFilterProxyModel *relationActionsProxyModel = new QSortFilterProxyModel(window);
    relationActionsProxyModel->setSourceModel(relationActionsModel);
    relationActionsProxyModel->setFilterKeyColumn(0);
    relationActionsProxyModel->sort(0, Qt::AscendingOrder);
    relationActionList->setModel(relationActionsProxyModel);
    int relationActionTypesCount = 0;
    for (const QString &typeId : relationActionIds) {
        auto item = new QStandardItem(typeId);
        item->setEditable(false);
        relationActionsModel->setItem(relationActionTypesCount++, item);
    }
    QLineEdit *relationActionsSearchLine = new QLineEdit(window);
    relationActionsSearchLine->setClearButtonEnabled(true);
    QObject::connect(relationActionsSearchLine, &QLineEdit::textChanged, [relationActionsProxyModel](const QString & text){
        relationActionsProxyModel->setFilterRegExp(QRegExp(text.trimmed(), Qt::CaseInsensitive, QRegExp::FixedString));
    });
    QGroupBox *relationActionsGroupBox = new QGroupBox(tr("Actions"), window);
    QVBoxLayout *relationActionsGroupBoxLayout = new QVBoxLayout(window);
    relationActionsGroupBoxLayout->addWidget(relationActionsSearchLine);
    relationActionsGroupBox->setLayout(relationActionsGroupBoxLayout);
    QVBoxLayout *relationActionsLayout = new QVBoxLayout();
    relationActionsLayout->addWidget(relationActionsGroupBox);
    relationActionsLayout->addWidget(relationActionList);
    //actions ^---------------------------------------------------------------------

    QTableWidget *activeRelationEventsList = new QTableWidget(window);
    QTableWidget *activeNodeEventList = new QTableWidget(window);

    activeRelationEventsList->setColumnCount(2);
    activeRelationEventsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList relationTableLabels;
    relationTableLabels << "Event" << "Action";
    activeRelationEventsList->setHorizontalHeaderLabels(relationTableLabels);
    QHeaderView* relationHeader = activeRelationEventsList->horizontalHeader();
    relationHeader->setSectionResizeMode(QHeaderView::Stretch);
    relationHeader->setHighlightSections(false);

    QMap<QString, QString> defaultRelationEventMap = m_settings->getRelationEvents();
    QMap<QString, QString>::iterator eventIter;
    for (eventIter = defaultRelationEventMap.begin(); eventIter != defaultRelationEventMap.end(); ++eventIter)
        populateEventList(eventIter.key(),eventIter.value(),activeRelationEventsList);


    QPushButton *addRelationButton = new QPushButton(tr("&Add"));
    addRelationButton->setDefault(true);

    QPushButton *removeRelationButton = new QPushButton(tr("&Remove"));
    removeRelationButton->setDefault(true);

    QPushButton *applyRelationButton = new QPushButton(tr("&Apply"));
    applyRelationButton->setDefault(true);

    QDialogButtonBox *relationButtonBox = new QDialogButtonBox();
    relationButtonBox->addButton(addRelationButton, QDialogButtonBox::AcceptRole);
    relationButtonBox->addButton(removeRelationButton, QDialogButtonBox::RejectRole);
    relationButtonBox->addButton(applyRelationButton, QDialogButtonBox::HelpRole);
    //не смог рассовать сигналы по кнопкам, сделел через стандартные роли :(

    connect(relationButtonBox, &QDialogButtonBox::accepted, [relationEventList, relationActionList, activeRelationEventsList, this](){
        //add
        if ((relationEventList->selectionModel()->hasSelection())&&(relationActionList->selectionModel()->hasSelection()))
        {
            QString eventName = relationEventList->selectionModel()->selection().indexes()[0].data().toString();
            QString actionName = relationActionList->selectionModel()->selection().indexes()[0].data().toString();

            QString eventData = eventName;
            QString actionData = actionName;
            if (!activeRelationEventsList->findItems(eventData, Qt::MatchFixedString).isEmpty())
                return;

            populateEventList(eventData, actionData, activeRelationEventsList);
        }
    });
    connect(relationButtonBox, &QDialogButtonBox::rejected, [activeRelationEventsList](){
        //delete
        foreach (QTableWidgetItem *item, activeRelationEventsList->selectedItems())
            activeRelationEventsList->removeRow(item->row());
    });
    connect(relationButtonBox, &QDialogButtonBox::helpRequested, [window, activeRelationEventsList, activeNodeEventList, this](){
        //apply
        QMap<QString, QString> newNodeEvents, newRelationEvents;
        for (int i = 0; i < activeRelationEventsList->rowCount(); i++)
            newRelationEvents[activeRelationEventsList->item(i,0)->text()] = activeRelationEventsList->item(i,1)->text();
        for (int i = 0; i < activeNodeEventList->rowCount(); i++)
            newNodeEvents[activeNodeEventList->item(i,0)->text()] = activeNodeEventList->item(i,1)->text();

        m_settings->changeNodeEvents(newNodeEvents);
        m_settings->changeRelationEvents(newRelationEvents);

        window->close();
    });

//------------------------------------------------------------------------------------------
    QSet<QString> nodeEventIds;
    nodeEventIds << "OnClick" << "OnRightClick" << "OnDoubleClick" << "OnFocus" << "OnMouseEnter" << "OnMouseLeave";

    QListView *nodeEventList = new QListView(window);
    nodeEventList->setSelectionMode(QListView::SingleSelection);
    QStandardItemModel *nodeEventsModel = new QStandardItemModel(window);
    QSortFilterProxyModel *nodeEventsProxyModel = new QSortFilterProxyModel(window);
    nodeEventsProxyModel->setSourceModel(nodeEventsModel);
    nodeEventsProxyModel->setFilterKeyColumn(0);
    nodeEventsProxyModel->sort(0, Qt::AscendingOrder);
    nodeEventList->setModel(nodeEventsProxyModel);
    int nodeEventTypesCount = 0;
    for (const QString &typeId : nodeEventIds) {
        auto item = new QStandardItem(typeId);
        item->setEditable(false);
        nodeEventsModel->setItem(nodeEventTypesCount++, item);
    }
    QLineEdit *nodeEventsSearchLine = new QLineEdit(window);
    nodeEventsSearchLine->setClearButtonEnabled(true);
    QObject::connect(nodeEventsSearchLine, &QLineEdit::textChanged, [nodeEventsProxyModel](const QString & text){
        nodeEventsProxyModel->setFilterRegExp(QRegExp(text.trimmed(), Qt::CaseInsensitive, QRegExp::FixedString));
    });
    QGroupBox *nodeEventsGroupBox = new QGroupBox(tr("Events"), window);
    QVBoxLayout *nodeEventsGroupBoxLayout = new QVBoxLayout(window);
    nodeEventsGroupBoxLayout->addWidget(nodeEventsSearchLine);
    nodeEventsGroupBox->setLayout(nodeEventsGroupBoxLayout);

    QVBoxLayout *nodeEventsLayout = new QVBoxLayout();
    nodeEventsLayout->addWidget(nodeEventsGroupBox);
    nodeEventsLayout->addWidget(nodeEventList);

    //  ^  NodeEvents  ^--------------------------------------------------------------

    QSet<QString> nodeActionIds;
    nodeActionIds << "Highlight" << "Highlight With Adjacent Edges" << "Highlight With Adjacent Nodes" << "Open options" << "Show Tip" << "Delete";

    QListView *nodeActionList = new QListView(window);
    nodeActionList->setSelectionMode(QListView::SingleSelection);
    QStandardItemModel *nodeActionsModel = new QStandardItemModel(window);
    QSortFilterProxyModel *nodeActionsProxyModel = new QSortFilterProxyModel(window);
    nodeActionsProxyModel->setSourceModel(nodeActionsModel);
    nodeActionsProxyModel->setFilterKeyColumn(0);
    nodeActionsProxyModel->sort(0, Qt::AscendingOrder);
    nodeActionList->setModel(nodeActionsProxyModel);
    int nodeActionTypesCount = 0;
    for (const QString &typeId : nodeActionIds) {
        auto item = new QStandardItem(typeId);
        item->setEditable(false);
        nodeActionsModel->setItem(nodeActionTypesCount++, item);
    }
    QLineEdit *nodeActionsSearchLine = new QLineEdit(window);
    nodeActionsSearchLine->setClearButtonEnabled(true);
    QObject::connect(nodeActionsSearchLine, &QLineEdit::textChanged, [nodeActionsProxyModel](const QString & text){
        nodeActionsProxyModel->setFilterRegExp(QRegExp(text.trimmed(), Qt::CaseInsensitive, QRegExp::FixedString));
    });
    QGroupBox *nodeActionsGroupBox = new QGroupBox(tr("Actions"), window);
    QVBoxLayout *nodeActionsGroupBoxLayout = new QVBoxLayout(window);
    nodeActionsGroupBoxLayout->addWidget(nodeActionsSearchLine);
    nodeActionsGroupBox->setLayout(nodeActionsGroupBoxLayout);
    QVBoxLayout *nodeActionsLayout = new QVBoxLayout();
    nodeActionsLayout->addWidget(nodeActionsGroupBox);
    nodeActionsLayout->addWidget(nodeActionList);
    //NodeActions ^---------------------------------------------------------------------

    //QTableWidget *nodeActiveList = new QTableWidget(window);
    activeNodeEventList->setColumnCount(2);
    activeNodeEventList->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList nodeTableLabels;
    nodeTableLabels << "Event" << "Action";
    activeNodeEventList->setHorizontalHeaderLabels(nodeTableLabels);
    QHeaderView* nodeHeader = activeNodeEventList->horizontalHeader();
    nodeHeader->setSectionResizeMode(QHeaderView::Stretch);
    nodeHeader->setHighlightSections(false);

    QMap<QString, QString> defaultNodeEventMap = m_settings->getNodeEvents();
    QMap<QString, QString>::iterator i;
    for (i = defaultNodeEventMap.begin(); i != defaultNodeEventMap.end(); ++i)
        populateEventList(i.key(),i.value(),activeNodeEventList);


    QPushButton *addNodeButton = new QPushButton(tr("&Add"));
    addNodeButton->setDefault(true);

    QPushButton *removeNodeButton = new QPushButton(tr("&Remove"));
    removeNodeButton->setDefault(true);

    QPushButton *applyNodeButton = new QPushButton(tr("&Apply"));
    applyNodeButton->setDefault(true);

    QDialogButtonBox *nodeButtonBox = new QDialogButtonBox();
    nodeButtonBox->addButton(addNodeButton, QDialogButtonBox::AcceptRole);
    nodeButtonBox->addButton(removeNodeButton, QDialogButtonBox::RejectRole);
    nodeButtonBox->addButton(applyNodeButton, QDialogButtonBox::HelpRole);
    //не смог рассовать сигналы по кнопкам, сделел через стандартные роли :(

    connect(nodeButtonBox, &QDialogButtonBox::accepted, [nodeEventList, nodeActionList, activeNodeEventList, this](){
        if (((nodeEventList->selectionModel()->hasSelection())&&(nodeActionList->selectionModel()->hasSelection())))
        {
            QString nodeEventName = nodeEventList->selectionModel()->selection().indexes()[0].data().toString();
            QString nodeActionName = nodeActionList->selectionModel()->selection().indexes()[0].data().toString();

            QString nodeEventData = nodeEventName;
            QString nodeActionData = nodeActionName;
            if (!activeNodeEventList->findItems(nodeEventData, Qt::MatchFixedString).isEmpty())
                return;
            populateEventList(nodeEventData, nodeActionData, activeNodeEventList);
        }
    });
    connect(nodeButtonBox, &QDialogButtonBox::rejected, [activeNodeEventList](){
        foreach (QTableWidgetItem *item, activeNodeEventList->selectedItems())
            activeNodeEventList->removeRow(item->row());
    });
    connect(nodeButtonBox, &QDialogButtonBox::helpRequested, [window, activeRelationEventsList, activeNodeEventList, this](){
        //connecting actions

        QMap<QString, QString> newNodeEvents, newRelationEvents;
        for (int i = 0; i < activeRelationEventsList->rowCount(); i++)
            newRelationEvents[activeRelationEventsList->item(i,0)->text()] = activeRelationEventsList->item(i,1)->text();
        for (int i = 0; i < activeNodeEventList->rowCount(); i++)
            newNodeEvents[activeNodeEventList->item(i,0)->text()] = activeNodeEventList->item(i,1)->text();

        m_settings->changeNodeEvents(newNodeEvents);
        m_settings->changeRelationEvents(newRelationEvents);

        window->close();
    });

    QGridLayout *edgesPageLayout = new QGridLayout();
    //edgesPageLayout->addLayout(edgesLayout, 0, 0);
    edgesPageLayout->addLayout(relationEventsLayout, 0, 0);
    edgesPageLayout->addLayout(relationActionsLayout, 0, 1);
    edgesPageLayout->addWidget(activeRelationEventsList, 1, 0, 1, 2);
    edgesPageLayout->addWidget(relationButtonBox, 2, 0, 1, 2);
    QWidget *edgesPageWidget  = new QWidget(window);
    edgesPageWidget->setLayout(edgesPageLayout);

    QGridLayout *nodesPageLayout = new QGridLayout();
    nodesPageLayout->addLayout(nodeEventsLayout, 0, 0);
    nodesPageLayout->addLayout(nodeActionsLayout, 0, 1);
    nodesPageLayout->addWidget(activeNodeEventList, 1, 0, 1, 2);
    nodesPageLayout->addWidget(nodeButtonBox, 2, 0, 1, 2);
    QWidget *nodesPageWidget  = new QWidget(window);
    nodesPageWidget->setLayout(nodesPageLayout);

    QTabWidget *tabs = new QTabWidget(window);
    tabs->addTab(edgesPageWidget, tr("Edges"));
    tabs->addTab(nodesPageWidget, tr("Nodes"));

    QGridLayout *mainGridLayout = new QGridLayout(window);

    mainGridLayout->addWidget(tabs);

    window->setWindowTitle(tr("Edit events"));
    window->setMinimumSize(QSize(400, 400));
    window->show();
}


void OntolisWindow::selectRelationTypeSlot()
{
    if (ui->tabWidget->count() == 0)
        return;
    OLSOntologyDataController *dataController = m_openOntologyWidgets[ui->tabWidget->currentIndex()]->dataController();
    int relationsCount = dataController->relationCount();
    QSet<RelationTypeId> relationTypeIds;
    for (int i=0; i < relationsCount; i++)
    {
        relationTypeIds << dataController->getRelationByIndex(i)->name;
    }

    QWidget *window = new QWidget();
    QListView *idsList = new QListView(window);
    idsList->setSelectionMode(QListView::SingleSelection);
    QStandardItemModel *model = new QStandardItemModel(window);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(window);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(0);
    proxyModel->sort(0, Qt::AscendingOrder);
    idsList->setModel(proxyModel);
    int relationsTypesCount = 0;
    for (const RelationTypeId &relationTypeId : relationTypeIds) {
        auto item = new QStandardItem(relationTypeId);
        item->setEditable(false);
        model->setItem(relationsTypesCount++, item);
    }
    QLineEdit *searchLine = new QLineEdit(window);
    searchLine->setClearButtonEnabled(true);
    QObject::connect(searchLine, &QLineEdit::textChanged, [proxyModel](const QString & text){
        proxyModel->setFilterRegExp(QRegExp(text.trimmed(), Qt::CaseInsensitive, QRegExp::FixedString));
    });
    QGroupBox *groupBox = new QGroupBox(tr("Filter"), window);
    QVBoxLayout *groupBoxLayout = new QVBoxLayout(window);
    groupBoxLayout->addWidget(searchLine);
    groupBox->setLayout(groupBoxLayout);
    QDialogButtonBox *openCloseButtonBox = new QDialogButtonBox(QDialogButtonBox::Open | QDialogButtonBox::Close, window);

    connect(openCloseButtonBox, &QDialogButtonBox::accepted, [this, model, proxyModel, idsList, window](){
        if (idsList->selectionModel()->selectedRows().count() != 1)
            return;
        QString relationTypeId = model->itemFromIndex(proxyModel->mapToSource(idsList->selectionModel()->selectedIndexes().first()))->text();
        editRelationVisualizationParametersSlot(relationTypeId);
    });
    connect(openCloseButtonBox, &QDialogButtonBox::rejected, [window](){
        window->close();
    });

    QVBoxLayout *layout = new QVBoxLayout(window);
    layout->addWidget(groupBox);
    layout->addWidget(idsList);
    layout->addWidget(openCloseButtonBox);
    window->setWindowTitle(tr("Choose relation to edit"));
    window->show();
}

QPair<QPushButton*, QPushButton*> buildCheckUncheckButtons(QStandardItemModel *model, QSortFilterProxyModel *proxyModel, QWidget *parent = 0)
{
    auto setCheckStateInModel = [model, proxyModel](Qt::CheckState checkState){
        return [model, proxyModel, checkState](){
            int rowCount = proxyModel->rowCount();
            for (int i = 0; i < rowCount; i++)
                model->item(proxyModel->mapToSource(proxyModel->index(i, 0)).row())->setCheckState(checkState);
        };
    };
    QPushButton *checkAllButton = new QPushButton(QObject::tr("Check all"), parent);
    QPushButton *uncheckAllButton = new QPushButton(QObject::tr("Uncheck all"), parent);
    QObject::connect(checkAllButton, &QPushButton::clicked, setCheckStateInModel(Qt::Checked));
    QObject::connect(uncheckAllButton, &QPushButton::clicked, setCheckStateInModel(Qt::Unchecked));
    return QPair<QPushButton*, QPushButton*>(checkAllButton, uncheckAllButton);
}

void OntolisWindow::applyGraphicSettingsSlot(int defaultIndex) {
    if (ui->tabWidget->count() > 0) {
        int index = defaultIndex != -1 ? defaultIndex : ui->tabWidget->currentIndex();
        OLSOntologyGraphWidget *w = m_openOntologyWidgets[index];
        OLSAdaptiveNodeVisualizerMaster::findAndSetVisualizers(w->dataController());
        if (m_nodesMenu != nullptr)
           m_modifyMenu->removeAction(m_nodesMenu->menuAction());
        m_nodesMenu = m_modifyMenu->addMenu(tr("Nodes"));
        OLSAdaptiveNodeVisualizerMaster::fillVisualizersMenu(m_nodesMenu, this, SLOT(applyGraphicSettingsSlot()));
        w->dataChangedSlot();
        updateOntologyTreeData();
    }
}

void OntolisWindow::importSourceFileSlot() {

  QString filePath = QFileDialog::getOpenFileName(this, tr("Open dialog"), QString(), "*");

  QFileInfo info(filePath);
  qDebug() <<  info.completeSuffix();
  QString converterPath = m_convertersMapping["." + info.completeSuffix()];

  if (converterPath.length() > 0) {
    QString cmd;
#ifdef Q_OS_WIN32
      cmd.append("python ");
#endif
    cmd.append(converterPath);
    cmd.append(" --method=import");
    cmd.append(" --source-path=");
    cmd.append(filePath);

    QProcess process;
    process.start(cmd, QIODevice::ReadOnly);
    process.waitForFinished();
    QString result(process.readAllStandardOutput());

    OLSProjectFile *file = m_currentProject.createFile(result);
    if (file != nullptr) {
      OLSOntologyGraphWidget *widget = createNewOntologyWidget(file);
      widget->dataChangedSlot();

      updateOntologyTreeData();
    }
  }
}

void OntolisWindow::createOntologyFileSlot() {

  OLSProjectFile *file = m_currentProject.createFile();
  if (file != nullptr) {
    OLSOntologyGraphWidget *widget = createNewOntologyWidget(file);
    widget->dataChangedSlot();
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

    updateOntologyTreeData();
  }
}

bool OntolisWindow::loadProjectFile (OLSProjectFile *file) {
    if (file != nullptr) {
      OLSOntologyGraphWidget *widget = createNewOntologyWidget(file);
      widget->dataChangedSlot();
      widget->expandAllSlot();

      updateOntologyTreeData();

    }
    return file != nullptr;
}

void OntolisWindow::openOntologyFileSlot() {
    static bool loadVisOnt = true;
    if (loadVisOnt) {
        QString oldPath = QDir::currentPath();
        QDir::setCurrent(QDir::currentPath().append("/../../ontolis-meta/ontolis/examples"));
        if (loadProjectFile(m_currentProject.openFile("_visualizeMaster.vis.ont"))) {
            QDir::setCurrent(oldPath);
            applyGraphicSettingsSlot(ui->tabWidget->count() - 1);
#ifdef Q_OS_LINUX
            // I don't know why, but at the first time some calls of library.load at
            // applyGraphicSettingsSlot->findAndSetVisualizers->addVisualizer->tryLoadVisualizerLib
            // leads to cleaning OLSAdaptiveNodeVisualizerMaster's m_drawVisualizers & m_nonDrawVisualizers
            applyGraphicSettingsSlot(ui->tabWidget->count() - 1);
#endif
        }
        QDir::setCurrent(oldPath);
        loadVisOnt = false;
        closeOntologySlot(ui->tabWidget->count() - 1);
    }
    QDir::setCurrent(QDir::currentPath().append("/../../ontolis-meta/ontolis/examples"));
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open ontology"), QString(), "*");
    OLSProjectFile *projectFile = m_currentProject.openFile(filePath);
    if (loadProjectFile(projectFile)) {
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
        if (!projectFile->ontologyController()->visualizeOntPath().isEmpty()) {
            if (loadProjectFile(m_currentProject.openFile(projectFile->ontologyController()->visualizeOntPath()))) {
                applyGraphicSettingsSlot(ui->tabWidget->count()-1);
#ifdef Q_OS_LINUX
                // I don't know why, but at the first time some calls of library.load at
                // applyGraphicSettingsSlot->findAndSetVisualizers->addVisualizer->tryLoadVisualizerLib
                // leads to cleaning OLSAdaptiveNodeVisualizerMaster's m_drawVisualizers & m_nonDrawVisualizers
                applyGraphicSettingsSlot(ui->tabWidget->count()-1);
#endif
            }
        }
    }
}

void OntolisWindow::saveOntologyFileSlot() {

  int index = ui->tabWidget->currentIndex();
  OLSProjectFile *file = m_currentProject.getProjectFileByIndex(index);
  if (file->path().length() > 0) {
    m_currentProject.saveFile(file);
  }
  else {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Open dialog"), QString(), "*");
    m_currentProject.saveFile(file, filePath);
  }

  ui->tabWidget->setTabText(index, file->name());
  updateOntologyTreeData();
}

void OntolisWindow::saveOntologyAsFileSlot() {

  int index = ui->tabWidget->currentIndex();
  OLSProjectFile *file = m_currentProject.getProjectFileByIndex(index);
  QString filePath = QFileDialog::getSaveFileName(this, tr("Open dialog"), QString(), "*");
  m_currentProject.saveFile(file, filePath);

  ui->tabWidget->setTabText(index, file->name());
  updateOntologyTreeData();
}

void OntolisWindow::openProjectSlot() {

  QString filePath = QFileDialog::getOpenFileName(this, tr("Open dialog"), QString(), "*.pjs");
  bool result = m_currentProject.openProject(filePath);

  if (result) {
    for (int i = 0; i < m_currentProject.filesCount(); ++i) {
      OLSProjectFile *file = m_currentProject.getProjectFileByIndex(i);

      OLSOntologyGraphWidget *widget = createNewOntologyWidget(file);
      widget->dataChangedSlot();
    }

    updateOntologyTreeData();
  }
}

void OntolisWindow::saveProjectSlot() {

  QString filePath = QFileDialog::getSaveFileName(this, tr("Save dialog"), QString(), "*.pjs");
  m_currentProject.saveProject(filePath);
}

void OntolisWindow::exportFileSlot() {

  QString filePath = QFileDialog::getSaveFileName(this, tr("Open dialog"), QString(), "*");

  QFileInfo info(filePath);
  QString converterPath = m_convertersMapping["." + info.completeSuffix()];

  if (converterPath.length() > 0) {
    int index = ui->tabWidget->currentIndex();
    OLSProjectFile *file = m_currentProject.getProjectFileByIndex(index);

    QTemporaryFile tmpFile;
    tmpFile.setAutoRemove(true);
    tmpFile.open();
    QVariant json = file->ontologyController()->serialize();
    QByteArray data = QJsonDocument::fromVariant(json).toJson();
    tmpFile.write(data);
    tmpFile.flush();

    QString cmd;
#ifdef Q_OS_WIN32
      cmd.append("python ");
#endif
    cmd.append(converterPath);
    cmd.append(" --method=export");
    cmd.append(" --source-path=");
    cmd.append(tmpFile.fileName());

    QProcess process;
    process.start(cmd, QIODevice::ReadOnly);
    process.waitForFinished();
    QString result(process.readAllStandardOutput());

    QFile dstFile(filePath);
    dstFile.open(QIODevice::WriteOnly | QIODevice::Text);
    dstFile.write(result.toLocal8Bit());
  }
}

void OntolisWindow::screenshotSlot() {

  QString filePath = QFileDialog::getSaveFileName(this, tr("Save dialog"), QString(), "*.png");
  if (filePath != nullptr) {
    if (ui->tabWidget->currentIndex() >= 0 && ui->tabWidget->currentIndex() < m_openOntologyWidgets.count()) {
      OLSOntologyGraphWidget *widget = m_openOntologyWidgets[ui->tabWidget->currentIndex()];
      QImage screenshot = widget->makeScreenshot();
      screenshot.save(filePath.endsWith(".png", Qt::CaseInsensitive) ? filePath : filePath + ".png");
    }
  }
}

void OntolisWindow::currentTabChangedSlot(int index) {

  OLSOntologyGraphWidget *widget = m_openOntologyWidgets[index];

  clearConnections();

  m_ontologyTreeViewController->setDataController(widget->dataController());

  connect(widget, SIGNAL(dataChangedSignal()), m_ontologyTreeViewController, SLOT(dataChangedSlot()));
  connect(m_ontologyTreeViewController, SIGNAL(dataChangedSignal()), widget, SLOT(dataChangedSlot()));

  connect(m_ontologyTreeViewController, SIGNAL(dataChangedSignal()), widget, SLOT(dataChangedSlot()));

  connect(m_ontologyTreeViewController, SIGNAL(itemSelectedSignal(long)), widget, SLOT(itemSelectedSlot(long)));

  connect(m_zoomInShortcut, SIGNAL(activated()), widget, SLOT(zoomInSlot()));
  connect(m_zoomOutShortcut, SIGNAL(activated()), widget, SLOT(zoomOutSlot()));
  connect(m_removeShortcut, SIGNAL(activated()), widget, SLOT(removeSelectedSlot()));
  connect(m_undoAction, &QAction::triggered, widget, &OLSOntologyGraphWidget::undoSlot);
  connect(m_redoAction, &QAction::triggered, widget, &OLSOntologyGraphWidget::redoSlot);
  connect(m_editNamespaces, &QAction::triggered, widget, &OLSOntologyGraphWidget::editNamespacesSlot);

  m_undoAction->setEnabled(widget->dataController()->latestChange() != nullptr);
  m_redoAction->setEnabled(widget->dataController()->latestUndoChange() != nullptr);
  m_editNamespaces->setEnabled(true);

  connect(widget, &OLSOntologyGraphWidget::undoAvailabilityChangedSignal, [this](bool available){
    m_undoAction->setEnabled(available);
  });
  connect(widget, &OLSOntologyGraphWidget::redoAvailabilityChangedSignal, [this](bool available){
    m_redoAction->setEnabled(available);
  });

  connect(this, SIGNAL(itemsSelectedSignal(QSet<long>)), widget, SLOT(itemsSelectedSlot(QSet<long>)));

  m_ontologyTreeViewController->updateData();
  m_ontologyTreeViewController->setDragEnabled(true);
  widget->updateData();
}

void OntolisWindow::currentFileChangedSlot(const QString &fileName) {

  for (int i = 0; i < m_currentProject.filesCount(); ++i) {
    OLSProjectFile *file = m_currentProject.getProjectFileByIndex(i);
    if (file->name() == fileName) {
      ui->tabWidget->setCurrentIndex(i);
    }
  }
}

void OntolisWindow::categorySelectedSlot(const QString &fileName, const QString &categoryName) {

  OLSProjectFile *file = nullptr;

  for (int i = 0; i < m_currentProject.filesCount(); ++i) {
    OLSProjectFile *projFile = m_currentProject.getProjectFileByIndex(i);
    if (projFile->name() == fileName) {
      file = projFile;
      break;
    }
  }

  if (file != nullptr) {
    OLSProjectFileCategory *category = file->getCategoryByName(categoryName);
    if (category != nullptr) {
      emit itemsSelectedSignal(category->relatedNodeIds());
    }
  }
}

void OntolisWindow::transformSlot() {

}

void OntolisWindow::moveToStartSlot() {

  qDebug() << "Move to start";

//  ProjectFile *currentFile = m_currentProject.getProjectFileByName(m_currentFileName);

//  if (currentFile != nullptr) {
//    if (m_currentSnapshotIndex > 0)  {
//      m_currentSnapshotIndex = 0;

//      Snapshot *snapshot = m_transformationHelper->snapshots().at(m_currentSnapshotIndex);

//      OntologyDataController *sourceDataController = snapshot->sourceOntologySnapshot();
//      OntologyDataController *destinationDataController = snapshot->destinationOntologySnapshot();
//      OntologyDataController *problemsDataController = snapshot->problemsOntologySnapshot();

//      m_sourceOntologyWidget->setDataController(sourceDataController);
//      m_sourceOntologyWidget->dataChangedSlot();

//      m_destinationOntologyWidget->setDataController(destinationDataController);
//      m_destinationOntologyWidget->dataChangedSlot();

//      m_problemsOntologyWidget->setDataController(problemsDataController);
//      m_problemsOntologyWidget->dataChangedSlot();

//      m_logTreeView->setModel(snapshot->logModelSnapshot());
//      m_logTreeView->expandAll();

//      if (sourceDataController->changesCount() > destinationDataController->changesCount()) {
//        if (sourceDataController->changesCount() > problemsDataController->changesCount()) {
//          ui->tabWidget->setCurrentIndex(0);
//        }
//        else {
//          ui->tabWidget->setCurrentIndex(4);
//        }
//      }
//      else {
//        if (destinationDataController->changesCount() > problemsDataController->changesCount()) {
//          ui->tabWidget->setCurrentIndex(1);
//        }
//        else {
//          ui->tabWidget->setCurrentIndex(4);
//        }
//      }
//    }
//  }
}

void OntolisWindow::moveForwardSlot() {

//  ProjectFile *currentFile = m_currentProject.getProjectFileByName(m_currentFileName);

//  if (currentFile != nullptr) {
//    if (m_currentSnapshotIndex < m_transformationHelper->snapshots().count() - 1) {
//      m_currentSnapshotIndex++;

//      qDebug() << "Move to " << m_currentSnapshotIndex << " snapshot";

//      Snapshot *snapshot = m_transformationHelper->snapshots().at(m_currentSnapshotIndex);

//      OntologyDataController *sourceDataController = snapshot->sourceOntologySnapshot();
//      OntologyDataController *destinationDataController = snapshot->destinationOntologySnapshot();
//      OntologyDataController *problemsDataController = snapshot->problemsOntologySnapshot();

//      m_sourceOntologyWidget->setDataController(sourceDataController);
//      m_sourceOntologyWidget->dataChangedSlot();

//      m_destinationOntologyWidget->setDataController(destinationDataController);
//      m_destinationOntologyWidget->dataChangedSlot();

//      m_problemsOntologyWidget->setDataController(problemsDataController);
//      m_problemsOntologyWidget->dataChangedSlot();

//      m_logTreeView->setModel(snapshot->logModelSnapshot());
//      m_logTreeView->expandAll();

//      if (sourceDataController->changesCount() > destinationDataController->changesCount()) {
//        if (sourceDataController->changesCount() > problemsDataController->changesCount()) {
//          ui->tabWidget->setCurrentIndex(0);
//        }
//        else {
//          ui->tabWidget->setCurrentIndex(4);
//        }
//      }
//      else {
//        if (destinationDataController->changesCount() > problemsDataController->changesCount()) {
//          ui->tabWidget->setCurrentIndex(1);
//        }
//        else {
//          ui->tabWidget->setCurrentIndex(4);
//        }
//      }
//    }
//    else {
//      moveToEndSlot();
//    }
//  }
}

void OntolisWindow::moveBackwardSlot() {

//  ProjectFile *currentFile = m_currentProject.getProjectFileByName(m_currentFileName);

//  if (currentFile != nullptr) {
//    if (m_currentSnapshotIndex > 0) {
//      m_currentSnapshotIndex--;

//      qDebug() << "Move to " << m_currentSnapshotIndex << " snapshot";

//      Snapshot *snapshot = m_transformationHelper->snapshots().at(m_currentSnapshotIndex);

//      OntologyDataController *sourceDataController = snapshot->sourceOntologySnapshot();
//      OntologyDataController *destinationDataController = snapshot->destinationOntologySnapshot();
//      OntologyDataController *problemsDataController = snapshot->problemsOntologySnapshot();

//      m_sourceOntologyWidget->setDataController(sourceDataController);
//      m_sourceOntologyWidget->dataChangedSlot();

//      m_destinationOntologyWidget->setDataController(destinationDataController);
//      m_destinationOntologyWidget->dataChangedSlot();

//      m_problemsOntologyWidget->setDataController(problemsDataController);
//      m_problemsOntologyWidget->dataChangedSlot();

//      m_logTreeView->setModel(snapshot->logModelSnapshot());
//      m_logTreeView->expandAll();

//      if (sourceDataController->changesCount() > destinationDataController->changesCount()) {
//        if (sourceDataController->changesCount() > problemsDataController->changesCount()) {
//          ui->tabWidget->setCurrentIndex(0);
//        }
//        else {
//          ui->tabWidget->setCurrentIndex(4);
//        }
//      }
//      else {
//        if (destinationDataController->changesCount() > problemsDataController->changesCount()) {
//          ui->tabWidget->setCurrentIndex(1);
//        }
//        else {
//          ui->tabWidget->setCurrentIndex(4);
//        }
//      }
//    }
//  }
}

void OntolisWindow::moveToEndSlot() {

  qDebug() << "Move to the end";

//  ProjectFile *currentFile = m_currentProject.getProjectFileByName(m_currentFileName);
//  if (m_currentFileName != nullptr) {
//    m_currentSnapshotIndex = m_transformationHelper->snapshots().count();

//    m_sourceOntologyWidget->setDataController(currentFile->sourceOntologyController());
//    m_sourceOntologyWidget->dataChangedSlot();

//    m_destinationOntologyWidget->setDataController(currentFile->destinationOntologyController());
//    m_destinationOntologyWidget->dataChangedSlot();

//    m_problemsOntologyWidget->setDataController(m_currentProject.problemsOntologyController());
//    m_problemsOntologyWidget->dataChangedSlot();

//    m_logTreeView->setModel(m_transformationHelper->logModel());
//    m_logTreeView->expandAll();

//    ui->tabWidget->setCurrentIndex(1);
//  }
}

void OntolisWindow::populateEventList(QString first, QString second, QTableWidget *tableWidget)
{
    int newRowIndex = tableWidget->rowCount();
    tableWidget->insertRow(newRowIndex);
    QTableWidgetItem *newFirstItem = new QTableWidgetItem(first);
    QTableWidgetItem *newSecondItem = new QTableWidgetItem(second);
    tableWidget->setItem(newRowIndex, 0, newFirstItem);
    tableWidget->setItem(newRowIndex, 1, newSecondItem);
}

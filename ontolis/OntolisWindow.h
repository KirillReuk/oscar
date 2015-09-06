#ifndef ONTOLISWINDOW_H
#define ONTOLISWINDOW_H

#include <QMainWindow>

#include "widgets/Shared/OLSOntologyDataController.h"
#include "widgets/OntologyGraph/OLSOntologyGraphWidget.h"
#include "widgets/OntologyTree/OLSOntologyTreeViewController.h"
#include "widgets/OntologyPallete/OLSOntologyPalleteWidget.h"
#include "widgets/ProjectTree/OLSProject.h"
#include "widgets/Shared/OLSSettings.h"

#include "extensions/OLSAdaptiveNodeVisualizerMaster.h"

namespace Ui {
class OntolisWindow;
}

class OntolisWindow : public QMainWindow {
    Q_OBJECT
    
  public:
    explicit OntolisWindow(QWidget *parent = 0);
    ~OntolisWindow();
    
  private:
    Ui::OntolisWindow *ui;

    OLSProject m_currentProject;

    QList<OLSOntologyGraphWidget *> m_openOntologyWidgets;

    QTreeView *m_logTreeView;

    OLSOntologyPalleteWidget *m_palleteWidget;

    OLSOntologyTreeViewController *m_ontologyTreeViewController;

    OLSSettings *m_settings;

    long m_currentSnapshotIndex;

    QShortcut *m_zoomInShortcut;
    QShortcut *m_zoomOutShortcut;
    QShortcut *m_removeShortcut;

    QMenu *m_nodesMenu = nullptr;
    QMenu *m_modifyMenu = nullptr;

    QAction *m_undoAction = nullptr;
    QAction *m_redoAction = nullptr;

    QAction *m_editNamespaces = nullptr;
    QAction *m_editEvents = nullptr;

    QMap<QString, QString> m_convertersMapping;
    QMap<QString, QString> m_transformatorsMapping;

    void setupMenu();
    void setupConverters();
    void setupTransformators();

    OLSOntologyGraphWidget *createNewOntologyWidget(OLSProjectFile *file);

    void clearConnections();
    void updateOntologyTreeData();

    bool loadProjectFile (OLSProjectFile *file);

  private slots:
    void closeOntologySlot(int index);
    void exitSlot();
    void importSourceFileSlot();

    void createOntologyFileSlot();
    void openOntologyFileSlot();
    void saveOntologyFileSlot();
    void saveOntologyAsFileSlot();

    void openProjectSlot();
    void saveProjectSlot();

    void exportFileSlot();
    void screenshotSlot();

    void transformSlot();

    void editEventsSlot();

    void moveToStartSlot();
    void moveForwardSlot();
    void moveBackwardSlot();
    void moveToEndSlot();

    void currentTabChangedSlot(int index);
    void currentFileChangedSlot(const QString &fileName);
    void categorySelectedSlot(const QString &fileName, const QString &categoryName);

    void selectRelationTypeSlot();
    void editRelationVisualizationParametersSlot(RelationTypeId typeId);

    void applyGraphicSettingsSlot(int defaultIndex = -1);
    void populateEventList(QString first, QString second, QTableWidget *tableWidget);

  signals:
    void itemsSelectedSignal(const QSet<long> ids);
};

#endif // ONTOLISWINDOW_H

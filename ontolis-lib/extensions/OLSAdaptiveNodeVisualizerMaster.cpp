#include "OLSAdaptiveNodeVisualizerMaster.h"
#include <QMenu>

QList<OLSAdaptiveNodeVisualizer *> OLSAdaptiveNodeVisualizerMaster::m_drawVisualizers;
QList<OLSAdaptiveNodeVisualizer *> OLSAdaptiveNodeVisualizerMaster::m_nonDrawVisualizers;

bool OLSAdaptiveNodeVisualizerMaster::tryPerformAdaptableVisualization(
        OLSOntologyGraphNodeItem *nodeToVisualize, QPainter *painter)
{
    return stepOfAdaptableVisualization(m_drawVisualizers, nodeToVisualize, painter);
}

bool OLSAdaptiveNodeVisualizerMaster::stepOfAdaptableVisualization(QList<OLSAdaptiveNodeVisualizer *> visualizers,
        OLSOntologyGraphNodeItem *nodeToVisualize, QPainter *painter)
{
    bool wasVisualized = false;
    foreach (OLSOntologyGraphRelationItem *relation, nodeToVisualize->getOutgoingRelationsByName(ONTOLIS_AVIS_NS, "hasVisMethod")) {
        OLSOntologyGraphNodeItem *nodeWithVisualizeInfo = relation->destinationNode();
        QList<OLSOntologyGraphRelationItem *> methodNames = nodeWithVisualizeInfo->getOutgoingRelationsByName(ONTOLIS_AVIS_NS, "methodName");
        if (methodNames.length() != 1) {
            qDebug() << "Not a visualization node";
            continue;
        }
        QString methodName = methodNames[0]->destinationNode()->name();
        foreach (OLSAdaptiveNodeVisualizer *vis, visualizers) {
            if ( vis->displayName() == methodName) {
                wasVisualized |= vis->tryPerformAdaptableVisualization(nodeToVisualize,
                                                          relation->destinationNode(), painter);
                break;
            }
        }
    }
    if (!wasVisualized) {
        foreach (OLSAdaptiveNodeVisualizer *vis, visualizers) {
            if (vis->isDefault()) {
                wasVisualized |= vis->tryPerformAdaptableVisualization(nodeToVisualize,
                                                          nullptr, painter);
                if (wasVisualized)
                    break;
            }
        }
    }
    return wasVisualized;
}

void OLSAdaptiveNodeVisualizerMaster::resolveDependencies(QList<OLSAdaptiveNodeVisualizer *> visualizers, OLSAdaptiveNodeVisualizer *newVisualizer)
{
    foreach (OLSAdaptiveNodeVisualizer *visualizer, visualizers) {
        foreach (QString dependencyName, visualizer->getDependencies()) {
            if (dependencyName == newVisualizer->className())
                visualizer->setDependency(dependencyName, newVisualizer);
        }
    }
}

void OLSAdaptiveNodeVisualizerMaster::addVisualizer(OLSAdaptiveVisualizerDataAsNodes visualizerData, bool drawer)
{
    int lastSeparator = visualizerData.libraryPathNode->name.lastIndexOf('/');
    QString folder = visualizerData.libraryPathNode->name.left(lastSeparator + 1);
    QString libraryName = visualizerData.libraryPathNode->name.mid(lastSeparator + 1);
    QString winLibraryName = folder + libraryName + ".dll";
    QString linLibraryName = folder + "lib" + libraryName + ".so";
    if (!tryLoadVisualizerLib(winLibraryName) && !tryLoadVisualizerLib(linLibraryName))
        return;
    int id = QMetaType::type(visualizerData.classNameNode->name.toUtf8().constData());
    if (id != 0) {
        OLSAdaptiveNodeVisualizer *visualizer = static_cast<OLSAdaptiveNodeVisualizer *>
                (QMetaType::create(id));
        if (visualizer == nullptr)
            return;
        visualizer->setData(visualizerData);
        (drawer ? m_drawVisualizers : m_nonDrawVisualizers).append(visualizer);        
        qDebug() << visualizer->displayName() << " via " << visualizer->className() << " @ " << visualizer->libraryPath();
    }
}

void OLSAdaptiveNodeVisualizerMaster::findAndSetVisualizers(OLSOntologyDataController *dataController)
{
    m_drawVisualizers.clear();
    m_nonDrawVisualizers.clear();
    QMap<OLSOntologyRelationData *, OLSOntologyNodeData *> nodeVisualizers = dataController->getRelatedOutgoingNodes("http://knova.ru/ontolis-avis", "#nodeVisualization");
    if (nodeVisualizers.isEmpty())
        return;
    foreach (OLSOntologyRelationData *relationData, nodeVisualizers.keys()) {
        OLSOntologyNodeData *nodeVisualizerData = nodeVisualizers[relationData];
        QList<OLSOntologyRelationData *> visClasses, libPaths, defaults;
        QList<OLSAdaptiveVisualizeParameter> visParams;
        QMap<OLSOntologyRelationData *, OLSOntologyNodeData *> nodeVisualizerOuts = dataController->getRelatedOutgoingNodes(nodeVisualizerData->id);
        foreach (OLSOntologyRelationData *relation, nodeVisualizerOuts.keys()) {
            if (relation->name == "#hasVisClass")
                visClasses.append(relation);
            if (relation->name == "#hasLibPath")
                libPaths.append(relation);
            if (relation->name == "#isDefault")
                defaults.append(relation);
            else if (relation->name == "#hasVisParameter"){
                OLSOntologyNodeData *paramNode = nodeVisualizerOuts[relation];
                QMap<OLSOntologyRelationData *, OLSOntologyNodeData *> paramNodeOuts = dataController->getRelatedOutgoingNodes(paramNode->id);
                QList<QString> defaultValues, visibleNames, types;
                foreach (OLSOntologyRelationData *relation, paramNodeOuts.keys()) {
                    if (relation->name == "#defaultValue")
                        defaultValues.append(paramNodeOuts[relation]->name);
                    else if (relation->name == "#hasVisName")
                        visibleNames.append(paramNodeOuts[relation]->name);
                }
                if (defaultValues.size() != 1 || visibleNames.size() != 1)
                    continue;
                OLSAdaptiveVisualizeParameter param;
                param.name = paramNode->name;
                param.defaultValue = defaultValues.first();
                param.displayName = visibleNames.first();
                visParams.append(param);
            }
        }
        if (visClasses.length() != 1 || libPaths.length() != 1 || defaults.size() > 1)
            continue;
        OLSAdaptiveVisualizerDataAsNodes visualizerData;
        visualizerData.classNameNode = dataController->getNodeById(visClasses[0]->destinationNodeId);
        visualizerData.libraryPathNode = dataController->getNodeById(libPaths[0]->destinationNodeId);
        visualizerData.displayNameNode = nodeVisualizerData;
        visualizerData.parameters = visParams;
        visualizerData.dataController = dataController;
        visualizerData.isDefault = defaults.size() == 1 &&
                dataController->getNodeById(defaults[0]->destinationNodeId)->name == "true";
        addVisualizer(visualizerData, relationData->name == "#hasDrawVisualizer");
    }
    foreach (OLSAdaptiveNodeVisualizer *visualizer, m_nonDrawVisualizers) {
        resolveDependencies(m_drawVisualizers, visualizer);
    }
}

bool OLSAdaptiveNodeVisualizerMaster::tryLoadVisualizerLib(QString path)
{
    QLibrary library(path);
    if (!library.load()) {
     qDebug() << library.errorString();
     return false;
    }
    qDebug() << "library loaded " << path;
    typedef void (*MVoid)();
    MVoid initFunc = (MVoid)library.resolve("initVisualizer");
    if (initFunc == nullptr)
        return false;
    initFunc();
    return true;
}

void OLSAdaptiveNodeVisualizerMaster::fillVisualizersMenu(QMenu *menu, QObject *subscriber, const char *slot)
{
    foreach (OLSAdaptiveNodeVisualizer* visualizer, m_drawVisualizers + m_nonDrawVisualizers) {
        QMenu *visualizerMenu =  menu->addMenu(visualizer->displayName());
        QAction *editAction = visualizerMenu->addAction("Edit");
        QObject::connect(editAction, SIGNAL(triggered()), visualizer->signalForwarder(), SLOT(showForm()));
        QObject::connect(visualizer->signalForwarder(),SIGNAL(workDone()), subscriber, slot);
        QAction *defaultAction = visualizerMenu->addAction("Set default");
        QObject::connect(defaultAction, SIGNAL(triggered()), visualizer->signalForwarder(), SLOT(setAsDefault()));
    }
}

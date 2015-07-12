#ifndef OLSADAPTIVENODEVISUALIZERMASTER_H
#define OLSADAPTIVENODEVISUALIZERMASTER_H

#include <QObject>
#include <QMainWindow>

#include "widgets/OntologyGraph/OLSOntologyGraphNodeItem.h"
#include "widgets/OntologyGraph/OLSOntologyGraphRelationItem.h"
#include "OLSAdaptiveNodeVisualizer.h"
#include "OLSAdaptiveVisualizeParameter.h"

#define ONTOLIS_AVIS_NS "http://knova.ru/ontolis-avis"

class OLSAdaptiveNodeVisualizerMaster
{
private:
    static QList<OLSAdaptiveNodeVisualizer *> m_drawVisualizers, m_nonDrawVisualizers;
    static void resolveDependencies(QList<OLSAdaptiveNodeVisualizer *> visualizers, OLSAdaptiveNodeVisualizer *newVisualizer);
    static void addVisualizer(OLSAdaptiveVisualizerDataAsNodes visualizerData, bool drawer);

public:
    static bool tryPerformAdaptableVisualization(OLSOntologyGraphNodeItem *nodeToVisualize, QPainter *painter);    
    static bool stepOfAdaptableVisualization(QList<OLSAdaptiveNodeVisualizer *> visualizers, OLSOntologyGraphNodeItem *nodeToVisualize, QPainter *painter);
    static void findAndSetVisualizers(OLSOntologyDataController *dataController);
    static void fillVisualizersMenu(QMenu *menu, QObject *subscriber, const char *slot);
    static bool tryLoadVisualizerLib(QString path);
};

#endif // OLSADAPTIVENODEVISUALIZERMASTER_H

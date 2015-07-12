#ifndef OLSADAPTIVERELATIONVISUALIZERMASTER_H
#define OLSADAPTIVERELATIONVISUALIZERMASTER_H

#include "widgets/OntologyGraph/OLSOntologyGraphRelationItem.h"
#include "extensions/OLSAdaptiveRelationVisualizerParameters.h"

#define ONTOLIS_NAME_REGIME "ye!gMnpU!o;5#h[r+a?tP\4e!ED;R.FZeCJiRtMv"
#define ONTOLIS_NAME_REGIME_LOCAL "Local"
#define ONTOLIS_NAME_REGIME_QNAME "QName"
#define ONTOLIS_NAME_REGIME_URI "URI"

class OLSOntologyGraphRelationItem;

class OLSAdaptiveRelationVisualizerMaster
{
    QMap<RelationTypeId, OLSAdaptiveRelationVisualizerParameters *> visualizationParameters;

public:    
    static const RelationTypeId defaultVisualizerId;

    OLSAdaptiveRelationVisualizerMaster();
    bool tryPerformAdaptableVisualization(OLSOntologyGraphRelationItem *relation, QPainter *painter);
    void setNormalColor(RelationTypeId typeId, QColor value);
    void setSelectedColor(RelationTypeId typeId, QColor value);
    void setChangedColor(RelationTypeId typeId, QColor value);
    void setArrowType(RelationTypeId typeId, ArrowType value);
    void setArrowPosition(RelationTypeId typeId, double value);
    void setTextPosition(RelationTypeId typeId, double value);
    void setEdgeWidth(RelationTypeId typeId, double value);
    void resetArrowType(RelationTypeId typeId);
    void resetArrowPosition(RelationTypeId typeId);
    void resetTextPosition(RelationTypeId typeId);
    void resetEdgeWidth(RelationTypeId typeId);
    OLSAdaptiveRelationVisualizerParameters *getVisualizationParameters(RelationTypeId typeId) const;
    OLSAdaptiveRelationVisualizerParameters *createOrGetVisualizationParameters(RelationTypeId typeId);
    static QString siblingPrefix() { return "*"; }
};

#endif // OLSADAPTIVERELATIONVISUALIZERMASTER_H


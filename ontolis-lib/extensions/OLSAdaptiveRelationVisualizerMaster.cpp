#include "OLSAdaptiveRelationVisualizerMaster.h"

const RelationTypeId OLSAdaptiveRelationVisualizerMaster::defaultVisualizerId = "fsgzwBHno9NLH8Ekp75jUznXEyeCJpWYirY2QpvpsAyglcMg7F";

OLSAdaptiveRelationVisualizerMaster::OLSAdaptiveRelationVisualizerMaster()
{
    OLSAdaptiveRelationVisualizerParameters *defaults = createOrGetVisualizationParameters(defaultVisualizerId);
    defaults->setSelectedColor(OLSOntologyGraphRelationItem::defaultSelectedColor);
    defaults->setChangedColor(OLSOntologyGraphRelationItem::defaultChangedColor);
    defaults->setNormalColor(OLSOntologyGraphRelationItem::defaultNormalColor);
    defaults->setArrowType(ArrowType::FilledTriangle);
    defaults->setArrowPosition(0.8);
    defaults->setTextPosition(0.5);
    defaults->setEdgeWidth(2.0);
}

bool OLSAdaptiveRelationVisualizerMaster::tryPerformAdaptableVisualization(OLSOntologyGraphRelationItem *relation, QPainter *painter)
{
    OLSAdaptiveRelationVisualizerParameters *params = createOrGetVisualizationParameters(relation->name());
    OLSAdaptiveRelationVisualizerParameters *defaults = getVisualizationParameters(defaultVisualizerId);

    QPointF sourcePos(relation->line().p1());
    QPointF destinationPos(relation->line().p2());
    QPointF textPos = sourcePos + (destinationPos - sourcePos) * params->textPosition(defaults->textPosition());
    QPointF arrowPos = sourcePos + (destinationPos - sourcePos) * params->arrowPosition(defaults->arrowPosition());

    double angle = acos(relation->line().dx() / relation->line().length());
    if (relation->line().dy() >= 0)
        angle = (M_PI * 2) - angle;
    QPointF arrowP1 = arrowPos - QPointF(sin(angle + M_PI / 3) * 10, cos(angle + M_PI / 3) * 10);
    QPointF arrowP2 = arrowPos - QPointF(sin(angle + M_PI - M_PI / 3) * 10, cos(angle + M_PI - M_PI / 3) * 10);

    QPen pen;
    //pen.setWidthF(30.0);
    pen.setWidthF(params->edgeWidth(defaults->edgeWidth()));

    if (relation->isSelected()) {
        QVector<qreal> dashPattern;
        dashPattern.append(2.0);
        dashPattern.append(2.0);
        pen.setDashPattern(dashPattern);
        pen.setColor(params->selectedColor(defaults->selectedColor()));
    }
    else if (relation->relatedDataController()->isRelationChanged(relation->id())) {
        pen.setColor(params->changedColor(defaults->changedColor()));
    } else {
        pen.setColor(params->normalColor(defaults->normalColor()));
    }
    painter->setPen(pen);

    QPainterPath line;
    QPainterPath arrowPath;
    switch (params->arrowType(defaults->arrowType())) {
    case ArrowType::TwoLines:
        line.moveTo(sourcePos);
        line.lineTo(destinationPos);
        painter->drawPath(line);

        if (!relation->name().startsWith(siblingPrefix())) {
            arrowPath.moveTo(arrowP1);
            arrowPath.lineTo(arrowPos);
            arrowPath.lineTo(arrowP2);
            painter->drawPath(arrowPath);
        }
        break;
    case ArrowType::EmptyTriangle:
        line.moveTo(sourcePos);
        line.lineTo((arrowP1 + arrowP2) / 2.0);
        line.moveTo(arrowPos);
        line.lineTo(destinationPos);
        painter->drawPath(line);

        if (!relation->name().startsWith(siblingPrefix())) {
            arrowPath.moveTo(arrowP1);
            arrowPath.lineTo(arrowPos);
            arrowPath.lineTo(arrowP2);
            arrowPath.lineTo(arrowP1);
            painter->drawPath(arrowPath);
        }
        break;
    case ArrowType::FilledTriangle:
        line.moveTo(sourcePos);
        line.lineTo(destinationPos);
        painter->drawPath(line);

        if (!relation->name().startsWith(siblingPrefix())) {
            arrowPath.moveTo(arrowP1);
            arrowPath.lineTo(arrowPos);
            arrowPath.lineTo(arrowP2);
            arrowPath.lineTo(arrowP1);
            painter->fillPath(arrowPath, QBrush (pen.color()));
        }
        break;
    }

    QString nameRegime = qApp->property(ONTOLIS_NAME_REGIME).toString();
    QString displayName = (nameRegime == ONTOLIS_NAME_REGIME_LOCAL) ? relation->name() : ((nameRegime == ONTOLIS_NAME_REGIME_QNAME) ? relation->qualifiedName() : relation->uri());

    painter->drawText(textPos, displayName);
    return true;
}

OLSAdaptiveRelationVisualizerParameters *OLSAdaptiveRelationVisualizerMaster::createOrGetVisualizationParameters(RelationTypeId typeId)
{
    if (!visualizationParameters.contains(typeId))
        visualizationParameters.insert(typeId, new OLSAdaptiveRelationVisualizerParameters);
    return visualizationParameters.value(typeId);
}

void OLSAdaptiveRelationVisualizerMaster::setNormalColor(RelationTypeId typeId, QColor value)
{    
    createOrGetVisualizationParameters(typeId)->setNormalColor(value);
}

void OLSAdaptiveRelationVisualizerMaster::setSelectedColor(RelationTypeId typeId, QColor value)
{
    createOrGetVisualizationParameters(typeId)->setSelectedColor(value);
}

void OLSAdaptiveRelationVisualizerMaster::setChangedColor(RelationTypeId typeId, QColor value)
{
    createOrGetVisualizationParameters(typeId)->setChangedColor(value);
}

void OLSAdaptiveRelationVisualizerMaster::setArrowType(RelationTypeId typeId, ArrowType value)
{
    createOrGetVisualizationParameters(typeId)->setArrowType(value);
}

void OLSAdaptiveRelationVisualizerMaster::setArrowPosition(RelationTypeId typeId, double value)
{
    createOrGetVisualizationParameters(typeId)->setArrowPosition(value);
}

void OLSAdaptiveRelationVisualizerMaster::setTextPosition(RelationTypeId typeId, double value)
{
    createOrGetVisualizationParameters(typeId)->setTextPosition(value);
}

void OLSAdaptiveRelationVisualizerMaster::setEdgeWidth(RelationTypeId typeId, double value)
{
    createOrGetVisualizationParameters(typeId)->setEdgeWidth(value);
}

void OLSAdaptiveRelationVisualizerMaster::resetArrowType(RelationTypeId typeId)
{
    createOrGetVisualizationParameters(typeId)->resetParameter(RelationAvisParameter::ArrowType);
}

void OLSAdaptiveRelationVisualizerMaster::resetArrowPosition(RelationTypeId typeId)
{
    createOrGetVisualizationParameters(typeId)->resetParameter(RelationAvisParameter::ArrowPosition);
}

void OLSAdaptiveRelationVisualizerMaster::resetTextPosition(RelationTypeId typeId)
{
    createOrGetVisualizationParameters(typeId)->resetParameter(RelationAvisParameter::TextPosition);
}
void OLSAdaptiveRelationVisualizerMaster::resetEdgeWidth(RelationTypeId typeId)
{
    createOrGetVisualizationParameters(typeId)->resetParameter(RelationAvisParameter::EdgeWidth);
}


OLSAdaptiveRelationVisualizerParameters *OLSAdaptiveRelationVisualizerMaster::getVisualizationParameters(RelationTypeId typeId) const
{    
    return visualizationParameters[typeId];
}

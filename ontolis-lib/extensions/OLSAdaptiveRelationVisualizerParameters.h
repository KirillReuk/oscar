#ifndef OLSADAPTIVERELATIONVISUALIZERPARAMETERS_H
#define OLSADAPTIVERELATIONVISUALIZERPARAMETERS_H

#include <set>
#include "widgets/OntologyGraph/OLSOntologyGraphElement.h"

enum ArrowType { TwoLines, EmptyTriangle, FilledTriangle };
Q_DECLARE_METATYPE(ArrowType)
enum class RelationAvisParameter { NormalColor, SelectedColor, ChangedColor, ArrowType, ArrowPosition, TextPosition, EdgeWidth };
typedef QString RelationTypeId;

class OLSAdaptiveRelationVisualizerParameters
{
private:
    std::set<RelationAvisParameter> m_settedParams;
    QColor m_normalColor;
    QColor m_selectedColor;
    QColor m_changedColor;
    ArrowType m_arrowType;
    double m_arrowPosition;
    double m_textPosition;
    double m_edgeWidth;

public:
    OLSAdaptiveRelationVisualizerParameters();
    void setNormalColor(QColor value);
    void setSelectedColor(QColor value);
    void setChangedColor(QColor value);
    void setArrowType(ArrowType value);
    void setArrowPosition(double value);
    void setTextPosition(double value);
    void setEdgeWidth(double value);

    void resetParameter(RelationAvisParameter parameter);
    void resetParameters();
    bool isParameterSet(RelationAvisParameter parameter);

    QColor normalColor(QColor defaultValue = OLSOntologyGraphElement::defaultNormalColor);
    QColor selectedColor(QColor defaultValue = OLSOntologyGraphElement::defaultSelectedColor);
    QColor changedColor(QColor defaultValue = OLSOntologyGraphElement::defaultChangedColor);
    ArrowType arrowType(ArrowType defaultValue = ArrowType::FilledTriangle);
    double arrowPosition(double defaultValue = 0.8);
    double textPosition(double defaultValue = 0.5);
    double edgeWidth(double defaultValue = 2.0);
};

#endif // OLSADAPTIVERELATIONVISUALIZERPARAMETERS_H

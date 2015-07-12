#include "OLSAdaptiveRelationVisualizerParameters.h"

OLSAdaptiveRelationVisualizerParameters::OLSAdaptiveRelationVisualizerParameters()
{
}

void OLSAdaptiveRelationVisualizerParameters::setNormalColor(QColor value)
{
    m_settedParams.insert(RelationAvisParameter::NormalColor);
    m_normalColor = value;
}

void OLSAdaptiveRelationVisualizerParameters::setSelectedColor(QColor value)
{
    m_settedParams.insert(RelationAvisParameter::SelectedColor);
    m_selectedColor = value;
}

void OLSAdaptiveRelationVisualizerParameters::setChangedColor(QColor value)
{
    m_settedParams.insert(RelationAvisParameter::ChangedColor);
    m_changedColor = value;
}

void OLSAdaptiveRelationVisualizerParameters::setArrowType(ArrowType value)
{
    m_settedParams.insert(RelationAvisParameter::ArrowType);
    m_arrowType = value;
}

void OLSAdaptiveRelationVisualizerParameters::setArrowPosition(double value)
{
    if (value >= 0.0 && value <= 1.0) {
        m_settedParams.insert(RelationAvisParameter::ArrowPosition);
        m_arrowPosition = value;
    }
}

void OLSAdaptiveRelationVisualizerParameters::setTextPosition(double value)
{
    if (value >= 0.0 && value <= 1.0) {
        m_settedParams.insert(RelationAvisParameter::TextPosition);
        m_textPosition = value;
    }
}

void OLSAdaptiveRelationVisualizerParameters::setEdgeWidth(double value)
{
    if (value >= 0.0 && value <= 5.0) {
        m_settedParams.insert(RelationAvisParameter::EdgeWidth);
        m_edgeWidth = value;
    }
}

void OLSAdaptiveRelationVisualizerParameters::resetParameter(RelationAvisParameter parameter)
{
    m_settedParams.erase(parameter);
}

void OLSAdaptiveRelationVisualizerParameters::resetParameters()
{
    m_settedParams.clear();
}

bool OLSAdaptiveRelationVisualizerParameters::isParameterSet(RelationAvisParameter parameter)
{
    return m_settedParams.find(parameter) != m_settedParams.end();
}

QColor OLSAdaptiveRelationVisualizerParameters::normalColor(QColor defaultValue)
{
    return m_settedParams.find(RelationAvisParameter::NormalColor) != m_settedParams.end()
            ? m_normalColor : defaultValue;
}

QColor OLSAdaptiveRelationVisualizerParameters::selectedColor(QColor defaultValue)
{
    return m_settedParams.find(RelationAvisParameter::SelectedColor) != m_settedParams.end()
            ? m_selectedColor : defaultValue;
}

QColor OLSAdaptiveRelationVisualizerParameters::changedColor(QColor defaultValue)
{
    return m_settedParams.find(RelationAvisParameter::ChangedColor) != m_settedParams.end()
            ? m_changedColor : defaultValue;
}

ArrowType OLSAdaptiveRelationVisualizerParameters::arrowType(ArrowType defaultValue)
{
    return m_settedParams.find(RelationAvisParameter::ArrowType) != m_settedParams.end()
            ? m_arrowType : defaultValue;
}

double OLSAdaptiveRelationVisualizerParameters::arrowPosition(double defaultValue)
{
    return m_settedParams.find(RelationAvisParameter::ArrowPosition) != m_settedParams.end()
            ? m_arrowPosition : defaultValue;
}

double OLSAdaptiveRelationVisualizerParameters::textPosition(double defaultValue)
{
    return m_settedParams.find(RelationAvisParameter::TextPosition) != m_settedParams.end()
            ? m_textPosition : defaultValue;
}
double OLSAdaptiveRelationVisualizerParameters::edgeWidth(double defaultValue)
{
    //return m_settedParams.find(RelationAvisParameter::EdgeWidth) != m_settedParams.end()? m_edgeWidth : defaultValue;
    //return defaultValue;
    return m_edgeWidth;
}

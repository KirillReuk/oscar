#include "OLSAdaptiveNodeVisualizerSignalForwarder.h"
#include "OLSAdaptiveNodeVisualizer.h"
#include "OLSAdaptiveNodeVisualizerForm.h"
#include "OLSAdaptiveNodeVisualizerMaster.h"

OLSAdaptiveNodeVisualizerSignalForwarder::OLSAdaptiveNodeVisualizerSignalForwarder(QObject *parent) :
    QObject(parent)
{
}

OLSAdaptiveNodeVisualizerSignalForwarder::OLSAdaptiveNodeVisualizerSignalForwarder(
        OLSAdaptiveNodeVisualizer *visualizer) {
    m_visualizer = visualizer;
}

void OLSAdaptiveNodeVisualizerSignalForwarder::showForm() {
    m_visualizer->showForm();
    emit workDone();
}

void OLSAdaptiveNodeVisualizerSignalForwarder::setAsDefault() {
    m_visualizer->setDefault();
    emit workDone();
}

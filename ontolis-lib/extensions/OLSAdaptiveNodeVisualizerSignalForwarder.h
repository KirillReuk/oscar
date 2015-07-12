#ifndef OLSADAPTIVENODEVISUALIZERSIGNALFORWARDER_H
#define OLSADAPTIVENODEVISUALIZERSIGNALFORWARDER_H

#include <QObject>

class OLSAdaptiveNodeVisualizer;

class OLSAdaptiveNodeVisualizerSignalForwarder : public QObject
{
    Q_OBJECT

private:
    OLSAdaptiveNodeVisualizer *m_visualizer;
public:
    explicit OLSAdaptiveNodeVisualizerSignalForwarder(QObject *parent = 0);
    explicit OLSAdaptiveNodeVisualizerSignalForwarder(OLSAdaptiveNodeVisualizer *visualizer);

signals:
    void workDone();

public slots:
    void showForm();
    void setAsDefault();

};

#endif // OLSADAPTIVENODEVISUALIZERSIGNALFORWARDER_H

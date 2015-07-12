#ifndef OLSADAPTIVENODEVISUALIZERFORM_H
#define OLSADAPTIVENODEVISUALIZERFORM_H

#include <QDialog>
#include <QPlainTextEdit>
#include "extensions/OLSAdaptiveVisualizerData.h"

namespace Ui {
class OLSAdaptiveNodeVisualizerForm;
}

class OLSAdaptiveNodeVisualizerForm : public QDialog
{
    Q_OBJECT

protected:
    static void setPlainTextEditText(QPlainTextEdit *ple, QString text);

public:
    explicit OLSAdaptiveNodeVisualizerForm(QWidget *parent = 0);
    ~OLSAdaptiveNodeVisualizerForm();
    void setData(OLSAdaptiveVisualizerData data);
    OLSAdaptiveVisualizerData data();
    static OLSAdaptiveVisualizerData getVisualizerData(OLSAdaptiveVisualizerData inputData);


private:
    Ui::OLSAdaptiveNodeVisualizerForm *ui;

public slots:
    void addRow();
    void deleteRow();
};

#endif // OLSADAPTIVENODEVISUALIZERFORM_H

#include "OLSAdaptiveNodeVisualizerForm.h"
#include "ui_OLSAdaptiveNodeVisualizerForm.h"
#include <QStandardItemModel>

OLSAdaptiveNodeVisualizerForm::OLSAdaptiveNodeVisualizerForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OLSAdaptiveNodeVisualizerForm)
{
    ui->setupUi(this);
    connect(ui->pbAddParam,SIGNAL(clicked()),this,SLOT(addRow()));
    connect(ui->pbDeleteParam,SIGNAL(clicked()),this,SLOT(deleteRow()));

    QStandardItemModel *model = new QStandardItemModel(0,3,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Display name")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Default value")));

    ui->tvParams->setModel(model);
    for (int c = 0; c < ui->tvParams->horizontalHeader()->count(); ++c)
    {
        ui->tvParams->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
}

OLSAdaptiveNodeVisualizerForm::~OLSAdaptiveNodeVisualizerForm()
{
    delete ui;
}

void OLSAdaptiveNodeVisualizerForm::addRow() {
    ui->tvParams->model()->insertRow(ui->tvParams->model()->rowCount());
}

void OLSAdaptiveNodeVisualizerForm::deleteRow() {
    QModelIndexList rows = ui->tvParams->selectionModel()->selectedRows();
    if (rows.count() > 0)
        ui->tvParams->model()->removeRow(rows[0].row());
}

void OLSAdaptiveNodeVisualizerForm::setPlainTextEditText(QPlainTextEdit *ple, QString text) {
    QTextDocument *doc = ple->document();
    QTextCursor curs(doc);
    curs.select(QTextCursor::Document);
    curs.removeSelectedText();
    curs.insertText(text);
}

void OLSAdaptiveNodeVisualizerForm::setData(OLSAdaptiveVisualizerData data) {
    ui->leClassName->setText(data.className);
    ui->leDisplayName->setText(data.displayName);
    setPlainTextEditText(ui->pleLibraryPath, data.libraryPath);
    QAbstractItemModel *model = ui->tvParams->model();
    model->removeRows(0, model->rowCount());
    for (int i = 0; i < data.parameters.count(); ++i) {
        model->insertRow(i);
        model->setData(model->index(i, 0), data.parameters[i].name);
        model->setData(model->index(i, 1), data.parameters[i].displayName);
        model->setData(model->index(i, 2), data.parameters[i].defaultValue);
    }
}

OLSAdaptiveVisualizerData OLSAdaptiveNodeVisualizerForm::data() {
    OLSAdaptiveVisualizerData data;
    data.className = ui->leClassName->text();
    data.displayName = ui->leDisplayName->text();
    data.libraryPath = ui->pleLibraryPath->toPlainText();
    QAbstractItemModel *model = ui->tvParams->model();
    for (int i = 0; i < model->rowCount(); ++i) {
        OLSAdaptiveVisualizeParameter param;
        param.name = model->data(model->index(i, 0)).toString();
        param.displayName = model->data(model->index(i, 1)).toString();
        param.defaultValue = model->data(model->index(i, 2)).toString();
        data.parameters.append(param);
    }
    return data;
}

OLSAdaptiveVisualizerData OLSAdaptiveNodeVisualizerForm::getVisualizerData(OLSAdaptiveVisualizerData inputData) {
    OLSAdaptiveNodeVisualizerForm form;
    form.setData(inputData);
    if (form.exec() == QDialog::Accepted)
        return form.data();
    else
        return inputData;
}

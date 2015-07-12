#include "OntolisWindow.h"
#include <QApplication>

#include <QDebug>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    OntolisWindow w;
    w.show();//Запуск главного окна
    return a.exec();
}

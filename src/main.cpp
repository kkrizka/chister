#include "mainwindow.h"

#include "Definitions.h"

#include <QApplication>

#include <iostream>

int main(int argc, char *argv[])
{    
    qRegisterMetaType<slot_t>("slot_t");
    qRegisterMetaType<QList<QPoint>>("QList<QPoint>");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

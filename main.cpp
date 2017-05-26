#include "mainwindow.h"

#include <QApplication>

#include <iostream>

int main(int argc, char *argv[])
{
    qRegisterMetaType<QList<QPoint> >("QList<QPoint>");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

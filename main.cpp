#include "mainwindow.h"

#include <QApplication>
#include <QSerialPort>

QT_USE_NAMESPACE

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

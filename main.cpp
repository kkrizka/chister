#include "mainwindow.h"

#include <QApplication>

#include <iostream>

int main(int argc, char *argv[])
{    
    qRegisterMetaType<slot_t>("slot_t");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

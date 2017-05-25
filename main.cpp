#include "mainwindow.h"

#include <QApplication>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::cout << "START APP";
    w.show();
    std::cout << "SHOW WINDOW";

    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextStream>

QT_USE_NAMESPACE

#include <iostream>

#include "serialconsole.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    SerialDevice ecs02("COM3");

    serialconsole console;
    console.setDevice(&ecs02);
    console.show();

    ecs02.openConnection();

    return a.exec();
}

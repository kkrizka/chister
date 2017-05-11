#include "mainwindow.h"
#include "serialconsole.h"
#include "ECS02UI.h"

#include "ECS02.h"

#include <QApplication>
#include <QSerialPort>

QT_USE_NAMESPACE

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    ECS02 ecs02;

    serialconsole console;
    console.setDevice(&ecs02);
    console.show();

    ECS02UI ecs02ui;
    ecs02ui.setDevice(&ecs02);
    ecs02ui.show();

    ecs02.openConnection();

    return a.exec();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serialconsole.h"

#include <QtSerialPort/QtSerialPort>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    std::cout << "Hello World!" << std::endl;
    serialconsole console;
    console.show();
}

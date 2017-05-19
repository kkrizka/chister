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

    frameGrabber=new FrameGrabber(this);
    connect(frameGrabber, &FrameGrabber::newImage, this, &MainWindow::updateCamera);
    frameGrabber->startAcquisition();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCamera(const QImage& img)
{
    ui->cameraImage->setPixmap(QPixmap::fromImage(img));
}

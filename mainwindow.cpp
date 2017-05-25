#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serialconsole.h"
#include "ECS02UI.h"

#include <QFileDialog>

#include <QtSerialPort/QtSerialPort>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the frame grabber
    m_frameGrabber=new FrameGrabber(this);
    connect(m_frameGrabber, &FrameGrabber::newImage, this, &MainWindow::updateCamera);
    m_frameGrabber->startAcquisition();

    // Create the probe station
    m_ecs02=new ECS02(this);
    m_ecs02->openConnection();

    // Setup dock widgets
    ECS02UI *ecs02ui=new ECS02UI(ui->ECSControlsDockWidget);
    ecs02ui->setDevice(m_ecs02);
    ui->ECSControlsDockWidget->setWidget(ecs02ui);

    // Setup program
    m_imageScanAnalysis=new ImageScanAnalysis(m_frameGrabber, m_ecs02, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCamera(const QImage& img)
{
    ui->cameraImage->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionControls_triggered()
{
    serialconsole *console=new serialconsole(this);
    console->setDevice(m_ecs02);
    console->show();
}

void MainWindow::on_actionImage_Scan_triggered()
{
    m_imageScanAnalysis->start();
}


void MainWindow::on_actionSavePicture_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Image", "",
        "PNG (*.png);Bitmap (*.bmp);All Files (*)");
    m_frameGrabber->getImage().save(fileName);
}

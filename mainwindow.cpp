#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serialconsole.h"
#include "ECS02UI.h"

#include <QFileDialog>
#include <QSettings>

#include <QtSerialPort/QtSerialPort>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Prepare saved settings
     m_settingsFile = QApplication::applicationDirPath() + "/config.ini";
     qInfo() << "Settings saved in" << m_settingsFile;
     QSettings settings(m_settingsFile, QSettings::IniFormat);

    // Create the frame grabber
    m_frameGrabberThread=new QThread(this);

    m_frameGrabber=new FrameGrabber();
    m_frameGrabber->moveToThread(m_frameGrabberThread);
    connect(m_frameGrabberThread, &QThread::started, m_frameGrabber, &FrameGrabber::startAcquisition);
    connect(m_frameGrabberThread, &QThread::finished,m_frameGrabber, &FrameGrabber::stopAcquisition);
    connect(m_frameGrabber, &FrameGrabber::finished, m_frameGrabberThread, &QThread::quit);
    setupCameraPipe(0);
    m_frameGrabberThread->start();

    // Create the probe station
    m_ecs02=new ECS02(this);
    m_ecs02->openConnection();

    // Setup dock widgets
    ECS02UI *ecs02ui=new ECS02UI(ui->ECSControlsDockWidget);
    ecs02ui->setDevice(m_ecs02);
    ui->ECSControlsDockWidget->setWidget(ecs02ui);

    // Setup program
    m_analysisThread=new QThread(this);

    m_imageScanAnalysis=new ImageScanAnalysis(m_frameGrabber, m_ecs02);
    m_imageScanAnalysis->moveToThread(m_analysisThread);

    m_swissHCCAnalysis=new SwissHCCAnalysis(m_frameGrabber, m_ecs02);
    m_swissHCCAnalysis->settingsLoad(&settings);
    m_swissHCCAnalysis->moveToThread(m_analysisThread);
    m_swissHCCAnalysisGUI=new SwissHCCAnalysisGUI(m_swissHCCAnalysis, this);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_frameGrabber;

    delete m_imageScanAnalysis;
    delete m_swissHCCAnalysis;
}

void MainWindow::setupCameraPipe(const AnalysisProgram *program)
{
    disconnect(m_imagePipe);
    if(program==0)
    {
        m_imagePipe=connect(m_frameGrabber, &FrameGrabber::newImage, this, &MainWindow::updateCamera);
    }
    else
    {
        m_imagePipe=connect(program, &AnalysisProgram::updateImage, this, &MainWindow::updateCamera);
        connect(m_frameGrabber, &FrameGrabber::newImage, program, &AnalysisProgram::analyze, Qt::DirectConnection);
    }
}

void MainWindow::updateCamera(const QImage& img)
{
    ui->cameraImage->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_actionExit_triggered()
{
    cleanUp();
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
    disconnect(m_analysisThread, &QThread::started, 0, 0);
    connect(m_analysisThread, &QThread::started, m_imageScanAnalysis, &ImageScanAnalysis::run);
    connect(m_imageScanAnalysis, &ImageScanAnalysis::finished, m_analysisThread, &QThread::quit);
    setupCameraPipe(m_imageScanAnalysis);

    m_analysisThread->start();
}

void MainWindow::on_actionHCCTest_triggered()
{
    disconnect(m_analysisThread, &QThread::started, 0, 0);
    connect(m_analysisThread, &QThread::started, m_swissHCCAnalysis, &SwissHCCAnalysis::run);
    connect(m_swissHCCAnalysis, &SwissHCCAnalysis::finished, m_analysisThread, &QThread::quit);
    setupCameraPipe(m_swissHCCAnalysis);

    addDockWidget(Qt::LeftDockWidgetArea,m_swissHCCAnalysisGUI->createControlDock(this));
    //m_swissHCCAnalysisGUI->createChipTest();

    m_analysisThread->start();
}

void MainWindow::on_actionSavePicture_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Image", "",
        "Images (*.png *.xpm *.jpg *.bmp);;All Files (*)");
    if(fileName.isEmpty()) return;
    m_frameGrabber->getImage().save(fileName);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    cleanUp();
}

void MainWindow::cleanUp()
{
    // Stop threads
    m_frameGrabberThread->quit();
    m_frameGrabberThread->wait();

    // Save settings
    QSettings settings(m_settingsFile, QSettings::IniFormat);
    m_swissHCCAnalysis->settingsSave(&settings);
}

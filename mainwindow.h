#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "FrameGrabber.h"
#include "ECS02.h"

#include "ImageScanAnalysis.h"
#include "SwissHCCAnalysis.h"
#include "SwissHCCAnalysisGUI.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateCamera(const QImage &img);

private slots:
    void on_actionExit_triggered();

    void on_actionImage_Scan_triggered();
    void on_actionHCCTest_triggered();

    void on_actionControls_triggered();

    void on_actionSavePicture_triggered();

private:
    Ui::MainWindow *ui;

    QString m_settingsFile;

    // Camera and stage control
    QThread *m_frameGrabberThread;
    FrameGrabber *m_frameGrabber;
    ECS02 *m_ecs02;
    QMetaObject::Connection m_imagePipe;

    // Analysis
    QThread *m_analysisThread;

    ImageScanAnalysis *m_imageScanAnalysis;

    SwissHCCAnalysis *m_swissHCCAnalysis;
    SwissHCCAnalysisGUI *m_swissHCCAnalysisGUI;


    void setupCameraPipe(const AnalysisProgram *program);
    void cleanUp();

    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H

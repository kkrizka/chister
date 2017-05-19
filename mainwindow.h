#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "FrameGrabber.h"
#include "ECS02.h"

#include "ImageScanAnalysis.h"

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

    void on_actionControls_triggered();

private:
    Ui::MainWindow *ui;

    FrameGrabber *m_frameGrabber;
    ECS02 *m_ecs02;

    ImageScanAnalysis *m_imageScanAnalysis;
};

#endif // MAINWINDOW_H

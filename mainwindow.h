#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "FrameGrabber.h"

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

private:
    Ui::MainWindow *ui;

    FrameGrabber *frameGrabber;
};

#endif // MAINWINDOW_H

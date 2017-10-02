#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#ifdef _WIN32
#include "NIFrameGrabber.h"
#else
#include "V4L2FrameGrabber.h"
#endif

#include "ECS02.h"

#include "ImageScanAnalysis.h"
#include "ImageScanAnalysisGUI.h"
#include "SwissHCCAnalysis.h"
#include "SwissHCCAnalysisGUI.h"
#include "MicroZedHCC.h"

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
  void showStatus(const QString &msg);

private slots:
  void on_actionExit_triggered();
  void on_actionImage_Scan_triggered();
  void on_actionHCCTest_triggered();
  void on_actionControls_triggered();
  void on_actionSavePicture_triggered();
  void on_actionHCCSerialConsole_triggered();

  void on_actionPreferences_triggered();

private:
  Ui::MainWindow *ui;

  QString m_settingsFile;

  // Camera and stage control
  QThread *m_frameGrabberThread;
  FrameGrabber *m_frameGrabber;
  ECS02 *m_stage;
  QMetaObject::Connection m_imagePipe;

  // Devices
  MicroZedHCC *m_microZedHCC;

  // Analysis
  QThread *m_analysisThread;

  ImageScanAnalysis *m_imageScanAnalysis;
  ImageScanAnalysisGUI *m_imageScanAnalysisGUI;

  SwissHCCAnalysis *m_swissHCCAnalysis;
  SwissHCCAnalysisGUI *m_swissHCCAnalysisGUI;


  void setupCameraPipe(const AnalysisProgram *program);
  void cleanUp();

  void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H

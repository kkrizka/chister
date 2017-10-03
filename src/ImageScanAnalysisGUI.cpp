#include "ImageScanAnalysisGUI.h"

#include <QLabel>
#include <QApplication>
#include <QDebug>

ImageScanAnalysisGUI::ImageScanAnalysisGUI(ImageScanAnalysis *program, QObject *parent)
    : AnalysisProgramGUI(program,parent)
{ 
  connect(program, &ImageScanAnalysis::startCalibrate, this, &ImageScanAnalysisGUI::showCalibrate);
  connect(program, &ImageScanAnalysis::startScan     , this, &ImageScanAnalysisGUI::showScan);
}

QDockWidget* ImageScanAnalysisGUI::createControlDock(QWidget *parent)
{
  QDockWidget *controlDock=AnalysisProgramGUI::createControlDock(parent);
  controlDock->setAttribute( Qt::WA_DeleteOnClose );
  connect(controlDock, &QWidget::destroyed, getProgram(), &AnalysisProgram::done);

  //
  // Create all of the necessary control widgets
  createStart(); // Start process dialog
  createCalibrate(); // Calibrate pixel2mm scale dialog
  createScan(); // Scan a big area to find chips

  return controlDock;
}

void ImageScanAnalysisGUI::createStart()
{
    m_startForm=new ImageScan_StartForm(getControlDock());

    connect(m_startForm,&ImageScan_StartForm::done,dynamic_cast<ImageScanAnalysis*>(getProgram()), &ImageScanAnalysis::runCalibrate);

    m_startForm->hide();
}

void ImageScanAnalysisGUI::createCalibrate()
{
  m_calibrateForm=new ImageScan_CalibrateForm(getControlDock());

  connect(dynamic_cast<ImageScanAnalysis*>(getProgram()), &ImageScanAnalysis::stepCalibrate, m_calibrateForm, &ImageScan_CalibrateForm::updateCalibrate);
  connect(dynamic_cast<ImageScanAnalysis*>(getProgram()), &ImageScanAnalysis::doneCalibrate, m_calibrateForm, &ImageScan_CalibrateForm::doneCalibrate);

  m_calibrateForm->hide();
}

void ImageScanAnalysisGUI::createScan()
{
  m_scanForm=new ImageScan_ScanForm(getControlDock());

  connect(dynamic_cast<ImageScanAnalysis*>(getProgram()), &ImageScanAnalysis::stepScan, m_scanForm, &ImageScan_ScanForm::updateScan);
  connect(dynamic_cast<ImageScanAnalysis*>(getProgram()), &ImageScanAnalysis::doneScan, m_scanForm, &ImageScan_ScanForm::doneScan);

  m_scanForm->hide();
}

void ImageScanAnalysisGUI::showStart()
{
  getControlDock()->setWidget(m_startForm);
}

void ImageScanAnalysisGUI::showCalibrate()
{
  getControlDock()->setWidget(m_calibrateForm);
}

void ImageScanAnalysisGUI::showScan()
{
  getControlDock()->setWidget(m_scanForm);
}

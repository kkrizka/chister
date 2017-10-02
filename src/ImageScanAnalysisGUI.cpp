#include "ImageScanAnalysisGUI.h"

#include <QLabel>
#include <QApplication>
#include <QDebug>

ImageScanAnalysisGUI::ImageScanAnalysisGUI(ImageScanAnalysis *program, QObject *parent)
    : AnalysisProgramGUI(program,parent)
{ 
  connect(program, &ImageScanAnalysis::startCalibrate, this, &ImageScanAnalysisGUI::showCalibrate);
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

void ImageScanAnalysisGUI::showStart()
{
  getControlDock()->setWidget(m_startForm);
}

void ImageScanAnalysisGUI::showCalibrate()
{
  getControlDock()->setWidget(m_calibrateForm);
}

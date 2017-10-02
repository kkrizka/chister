#include "ImageScanAnalysisGUI.h"

#include <QLabel>
#include <QApplication>
#include <QDebug>

ImageScanAnalysisGUI::ImageScanAnalysisGUI(ImageScanAnalysis *program, QObject *parent)
    : AnalysisProgramGUI(program,parent)
{ 
  connect(this, &ImageScanAnalysisGUI::startCalibrate,dynamic_cast<ImageScanAnalysis*>(getProgram()), &ImageScanAnalysis::runCalibrate);
}

QDockWidget* ImageScanAnalysisGUI::createControlDock(QWidget *parent)
{
  QDockWidget *controlDock=AnalysisProgramGUI::createControlDock(parent);
  controlDock->setAttribute( Qt::WA_DeleteOnClose );
  connect(controlDock, &QWidget::destroyed, getProgram(), &AnalysisProgram::done);

  //
  // Create all of the necessary control widgets
  createCalibrate(); // Calibrate pixel2mm scale dialog

  return controlDock;
}

void ImageScanAnalysisGUI::createCalibrate()
{
    m_calibrateForm=new ImageScan_CalibrateForm(getControlDock());

    connect(m_calibrateForm,&ImageScan_CalibrateForm::done,this,&ImageScanAnalysisGUI::calibrate);

    m_calibrateForm->hide();
}

void ImageScanAnalysisGUI::showCalibrate()
{
  getControlDock()->setWidget(m_calibrateForm);
}

void ImageScanAnalysisGUI::calibrate()
{
  emit startCalibrate();
}

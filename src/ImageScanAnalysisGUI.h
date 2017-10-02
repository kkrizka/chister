#ifndef IMAGESCANANALYSISGUI_H
#define IMAGESCANANALYSISGUI_H

#include "AnalysisProgramGUI.h"

#include "ImageScanAnalysis.h"

#include "ImageScan_CalibrateForm.h"

class ImageScanAnalysisGUI : public AnalysisProgramGUI
{
  Q_OBJECT
public:
  ImageScanAnalysisGUI(ImageScanAnalysis *program, QObject *parent=0);

  virtual QDockWidget* createControlDock(QWidget *parent);

public slots:
  void showCalibrate();

  void calibrate();

signals:
  void startCalibrate();

private:
  ImageScan_CalibrateForm *m_calibrateForm;

  void createCalibrate();
};

#endif // IMAGESCANANALYSISGUI_H

#ifndef IMAGESCANANALYSISGUI_H
#define IMAGESCANANALYSISGUI_H

#include "AnalysisProgramGUI.h"

#include "ImageScanAnalysis.h"

#include "ImageScan_StartForm.h"
#include "ImageScan_CalibrateForm.h"

class ImageScanAnalysisGUI : public AnalysisProgramGUI
{
  Q_OBJECT
public:
  ImageScanAnalysisGUI(ImageScanAnalysis *program, QObject *parent=0);

  virtual QDockWidget* createControlDock(QWidget *parent);

public slots:
  void showStart();
  void showCalibrate();

private:
  ImageScan_StartForm *m_startForm;
  ImageScan_CalibrateForm *m_calibrateForm;

  void createStart();
  void createCalibrate();
};

#endif // IMAGESCANANALYSISGUI_H

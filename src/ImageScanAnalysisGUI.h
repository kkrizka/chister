#ifndef IMAGESCANANALYSISGUI_H
#define IMAGESCANANALYSISGUI_H

#include "AnalysisProgramGUI.h"

#include "ImageScanAnalysis.h"

#include "ImageScan_StartForm.h"
#include "ImageScan_CalibrateForm.h"
#include "ImageScan_ScanForm.h"
#include "ImageScan_PictureForm.h"

class ImageScanAnalysisGUI : public AnalysisProgramGUI
{
  Q_OBJECT
public:
  ImageScanAnalysisGUI(ImageScanAnalysis *program, QObject *parent=0);

  virtual QDockWidget* createControlDock(QWidget *parent);

public slots:
  void showStart();
  void showCalibrate();
  void showScan();
  void showPicture(const QList<QPoint>& chips);

private:
  ImageScan_StartForm *m_startForm;
  ImageScan_CalibrateForm *m_calibrateForm;
  ImageScan_ScanForm *m_scanForm;
  ImageScan_PictureForm *m_pictureForm;

  void createStart();
  void createCalibrate();
  void createScan();
  void createPicture();
};

#endif // IMAGESCANANALYSISGUI_H

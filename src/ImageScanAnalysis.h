#ifndef IMAGESCANANALYSIS_H
#define IMAGESCANANALYSIS_H

#include "AnalysisProgram.h"

class ImageScanAnalysis : public AnalysisProgram
{
  Q_OBJECT
public:
  ImageScanAnalysis(FrameGrabber *frameGrabber, ECS02 *stage, QObject *parent = 0);

signals:
  void startCalibrate();
  void stepCalibrate(double scale);
  void doneCalibrate(double scale);
  void startScan();
  void stepScan(const QImage& patch);
  void doneScan(double scale);

public slots:
  void run();
  void runCalibrate();
  void runScan();
  void done();

private:
  double m_scale;
};

#endif // IMAGESCANANALYSIS_H

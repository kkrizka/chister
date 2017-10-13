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
  void doneScan(const QList<QPoint>& chips);
  void startPictures();
  void stepPictures(const QImage& patch);
  void donePictures();

public slots:
  void run();
  void runCalibrate();
  void runScan();
  void runMoveTo(const QPoint& pos);
  void done();

private:
  double m_scale;
  QList<QPoint> m_chipPos;
};

#endif // IMAGESCANANALYSIS_H

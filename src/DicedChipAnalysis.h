#ifndef DICEDCHIPANALYSIS_H
#define DICEDCHIPANALYSIS_H

#include "AnalysisProgram.h"
#include "DicedChipTemplate.h"
#include "DicedChipSlot.h"

#include <opencv2/opencv.hpp>

#include <QTextStream>
#include <QFile>
#include <QMap>

class DicedChipAnalysis : public AnalysisProgram
{
  Q_OBJECT
public:
  DicedChipAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent = 0);

  void setChipTemplate(const DicedChipTemplate& chipTemplate);
  void setLogDirectory(const QString& logDirectory);
  void setValidSlots(const QList<DicedChipSlot*>& validSlots);

  QMap<DicedChipSlot, bool> testResults() const;

public slots:
  void settingsSave(QSettings *settings);
  void settingsLoad(QSettings *settings);

  void run();
  void runLoadChips();
  void runFindProbes();
  void runCalibratePosition();
  void runCrossSave();
  void runCrossTest();
  void runFindChips();
  void runFindChip(DicedChipSlot *slot);
  void runAlignChip();
  void runChipSave();
  void runChipSkip();
  void runTestChips();
  void runChipTest();

  void analyze(const QImage& img);
  void analyzeFindProbes(const QImage& img);
  void analyzeFindGroove(const QImage& img);
  void analyzeFindGrooveCross(const QImage& img);
  void analyzeAlignChip(const QImage& img);

  void done();

signals:
  void status(const QString& text);
  void message(const QString& text);
  void donePrepareLoadChips();
  void doneFindProbes();
  void doneFindCross();

  void foundCross(float angle);
  void testCrossAngle(float angle);

  void startFindChips();
  void startFindChip(DicedChipSlot* slot);
  void chipFound(float score);
  void chipAlignSuccess();
  void chipAlignFailed();
  void chipUpdated(const DicedChipSlot* slot);
  void doneFindChips();
  void startTestChips();
  void doneTestChips();

private:
  QString m_logDirectory;

  // Log
  QFile m_logFH;
  QTextStream  m_log;

  // State machines for image analysis
  enum ImageAnalysisState {None, FindProbes, FindGroove, FindGrooveCross, AlignChip};
  ImageAnalysisState m_imageAnalysisState;

  // Testing state
  QList<DicedChipSlot*> m_validSlots;
  DicedChipSlot* m_activeSlot =nullptr;

  // Templates
  DicedChipTemplate m_chipTemplate;

  // Results of analysis
  bool m_edgeFound;
  double m_edgeRadius, m_edgeAngle;

  bool m_crossFound;
  QPointF m_crossPoint;

  double m_probesOffsetScore;
  double m_probesOffsetX, m_probesOffsetY; // position of the probes template top-left from top-left in mm

  double m_chipOffsetScore;
  double m_chipOffsetX, m_chipOffsetY; // position of the HCC template bottom-right from top-left in mm

  uint m_workingAreaX=0,m_workingAreaY=0; // position of the "probe free area"
  uint m_workingAreaWidth=640,m_workingAreaHeight=480; // size of the "probe free area"

  void logStatus(const QString& message);

  std::vector<cv::Vec2f> findLines(const QImage& img) const;
  std::vector<cv::Vec2f> findGrooves(const QImage& img) const;
  double calculateEmptyInArea(const cv::Mat& img, const cv::Rect& rect) const;
};

#endif // DICEDCHIPANALYSIS_H

#ifndef IMAGESCANANALYSIS_H
#define IMAGESCANANALYSIS_H

#include "AnalysisProgram.h"

class ImageScanAnalysis : public AnalysisProgram
{
    Q_OBJECT
public:
    ImageScanAnalysis(FrameGrabber *frameGrabber, ECS02 *stage, QObject *parent = 0);

    QDockWidget* createControlWidget(QWidget *parent=0);

public slots:
  void run();
  void runCalibrate();
  void done();
};

#endif // IMAGESCANANALYSIS_H

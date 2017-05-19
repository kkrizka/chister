#ifndef IMAGESCANANALYSIS_H
#define IMAGESCANANALYSIS_H

#include "AnalysisProgram.h"

class ImageScanAnalysis : public AnalysisProgram
{
public:
    ImageScanAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent = 0);

public slots:
    virtual void run();
};

#endif // IMAGESCANANALYSIS_H

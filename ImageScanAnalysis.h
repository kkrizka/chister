#ifndef IMAGESCANANALYSIS_H
#define IMAGESCANANALYSIS_H

#include "AnalysisProgram.h"

class ImageScanAnalysis : public AnalysisProgram
{
public:
    ImageScanAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent = 0);

public slots:
    void run() Q_DECL_OVERRIDE;
};

#endif // IMAGESCANANALYSIS_H

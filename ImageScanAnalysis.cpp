#include "ImageScanAnalysis.h"

#include <QDebug>

ImageScanAnalysis::ImageScanAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
    : AnalysisProgram(frameGrabber, ecs02, parent)
{

}

void ImageScanAnalysis::run()
{
    getECS02()->moveHome();
    getFrameGrabber()->getImage(true).save("test_HOME.png");
    getECS02()->moveIncrement(10,0);
    getFrameGrabber()->getImage(true).save("test_move.png");
}

#include "ImageScanAnalysis.h"

#include <QDebug>
#include <QThread>

ImageScanAnalysis::ImageScanAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
    : AnalysisProgram(frameGrabber, ecs02, parent)
{ }

void ImageScanAnalysis::run()
{

    getECS02()->moveHome();
    getECS02()->waitForIdle();
    for(uint x=0; x<30; x++)
    {
        for(uint y=0; y<30;y ++)
        {
            getECS02()->moveAbsolute(-25.4+x*240*0.0075,-25.4+y*240*0.0075);
            getECS02()->waitForIdle();
            getECS02()->updateInfo();
            double px=getECS02()->getX();
            double py=getECS02()->getY();
            getFrameGrabber()->getImage(true).save(QString("data_x%1_y%2_X%3_Y%4.png").arg(x).arg(y).arg(px).arg(py));
        }
    }
    emit finished();
}

void ImageScanAnalysis::done()
{ emit finished(); }

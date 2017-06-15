#ifndef IMAGESCANANALYSIS_H
#define IMAGESCANANALYSIS_H

#include "AnalysisProgram.h"

class ImageScanAnalysis : public AnalysisProgram
{
    Q_OBJECT
public:
    ImageScanAnalysis(FrameGrabber *frameGrabber, Stage *stage, QObject *parent = 0);

    QDockWidget* createControlWidget(QWidget *parent=0);

public slots:
    void run();
    void done();
};

#endif // IMAGESCANANALYSIS_H

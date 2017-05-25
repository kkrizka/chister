#include "SwissHCCAnalysis.h"

#include <QDebug>
#include <QThread>
#include <QLabel>

SwissHCCAnalysis::SwissHCCAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
    : AnalysisProgram(frameGrabber, ecs02, parent)
{ }

void SwissHCCAnalysis::run()
{

    getECS02()->moveLoad();
    getECS02()->waitForIdle();

    emit stepMoveToLoadDone();
}

void SwissHCCAnalysis::runCalibration(const QList<QPoint>& validSlots)
{
    qInfo() << "Run calibration";

    m_validSlots=validSlots;

    emit message(tr("POSITION CALIBRATION"));

    getECS02()->moveAbsolute(0, 20);
    getECS02()->waitForIdle();

    emit message(tr("POSITION ALIGNMENT"));

    emit finished();
}

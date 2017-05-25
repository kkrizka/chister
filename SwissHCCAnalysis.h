#ifndef SWISSHCCANALYSIS_H
#define SWISSHCCANALYSIS_H

#include "AnalysisProgram.h"

Q_DECLARE_METATYPE(QList<QPoint>);

class SwissHCCAnalysis : public AnalysisProgram
{
    Q_OBJECT
public:
    SwissHCCAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent = 0);

public slots:
    void run();
    void runCalibration(const QList<QPoint>& validSlots);

signals:
    void message(const QString& text);
    void stepMoveToLoadDone();

private:
    QList<QPoint> m_validSlots;
};

#endif // SWISSHCCANALYSIS_H

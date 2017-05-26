#ifndef SWISSHCCANALYSIS_H
#define SWISSHCCANALYSIS_H

#include "AnalysisProgram.h"

#include <opencv2/opencv.hpp>

Q_DECLARE_METATYPE(QList<QPoint>);

class SwissHCCAnalysis : public AnalysisProgram
{
    Q_OBJECT
public:
    SwissHCCAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent = 0);

public slots:
    void run();
    void runCalibration(const QList<QPoint>& validSlots);
    void runCrossTest();
    void runFindChip();

    void analyzeCrossAngle(const QImage& img);

signals:
    void message(const QString& text);
    void stepMoveToLoadDone();
    void stepCrossFound();

    void foundCross(float x, float y, float angle);
    void testCrossAngle(float angle);

private:
    QList<QPoint> m_validSlots;

    std::vector<cv::Vec2f> findGrooves(const QImage& img) const;
};

#endif // SWISSHCCANALYSIS_H

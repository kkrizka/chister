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
    void setValidSlots(const QList<QPoint>& validSlots);

    void run();
    void runFindProbes();
    void runCalibration();
    void runCrossTest();
    void runFindChips();
    void runFindChip(const QPoint& slot);
    void runAlignChip();
    void runChipTest();

    void analyze(const QImage& img);
    void analyzeFindProbes(const QImage& img);
    void analyzeFindGroove(const QImage& img);
    void analyzeFindGrooveCross(const QImage& img);
    void analyzeAlignChip(const QImage& img);

signals:
    void message(const QString& text);
    void stepMoveToLoadDone();
    void stopFindProbesDone();
    void stepFindCrossDone();
    void startFindChip();

    void foundCross(float angle);
    void testCrossAngle(float angle);

    void chipAlignFailed();

private:
    QList<QPoint> m_validSlots;

    enum ImageAnalysisState {None, FindProbes, FindGroove, FindGrooveCross, AlignChip};
    ImageAnalysisState m_imageAnalysisState;

    // Testing satate
    QPoint m_activeSlot;

    // Results of analysis
    bool m_edgeFound;

    bool m_crossFound;
    QPointF m_crossPoint;

    double m_probesOffsetScore;
    double m_probesOffsetX, m_probesOffsetY;

    double m_chipOffsetScore;
    double m_chipOffsetX, m_chipOffsetY;

    std::vector<cv::Vec2f> findLines(const QImage& img) const;
    std::vector<cv::Vec2f> findGrooves(const QImage& img) const;
};

#endif // SWISSHCCANALYSIS_H

#ifndef SWISSHCCANALYSIS_H
#define SWISSHCCANALYSIS_H

#include "AnalysisProgram.h"

#include <opencv2/opencv.hpp>

#include <QTextStream>
#include <QFile>

class SwissHCCAnalysis : public AnalysisProgram
{
    Q_OBJECT
public:
    SwissHCCAnalysis(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent = 0);

    void setLogDirectory(const QString& logDirectory);
    void setValidSlots(const QList<QPoint>& validSlots);

public slots:
    void settingsSave(QSettings *settings);
    void settingsLoad(QSettings *settings);

    void run();
    void runLoadChips();
    void runFindProbes();
    void runCalibratePosition();
    void runCrossSave();
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
    void status(const QString& text);
    void message(const QString& text);
    void donePrepareLoadChips();
    void doneFindProbes();
    void doneFindCross();
    void startFindChip();

    void foundCross(float angle);
    void testCrossAngle(float angle);

    void findingChip();
    void chipFound(float score);
    void chipAlignSuccess();
    void chipAlignFailed();

private:
    QString m_logDirectory;
    QList<QPoint> m_validSlots;

    // Log
    QFile m_logFH;
    QTextStream  m_log;

    // State machines for image analysis
    enum ImageAnalysisState {None, FindProbes, FindGroove, FindGrooveCross, AlignChip};
    ImageAnalysisState m_imageAnalysisState;

    // Testing satate
    QPoint m_activeSlot;

    // Templates
    cv::Mat m_templateHCC;
    cv::Mat m_templateProbes;

    // Results of analysis
    bool m_edgeFound;
    double m_edgeAngle;

    bool m_crossFound;
    QPointF m_crossPoint;

    double m_probesOffsetScore;
    double m_probesOffsetX, m_probesOffsetY; // position of the probes template top-left from top-left in mm

    double m_chipOffsetScore;
    double m_chipOffsetX, m_chipOffsetY; // position of the HCC template bottom-right from top-left in mm

    void logStatus(const QString& message);

    std::vector<cv::Vec2f> findLines(const QImage& img) const;
    std::vector<cv::Vec2f> findGrooves(const QImage& img) const;
};

#endif // SWISSHCCANALYSIS_H

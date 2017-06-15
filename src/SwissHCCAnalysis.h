#ifndef SWISSHCCANALYSIS_H
#define SWISSHCCANALYSIS_H

#include "AnalysisProgram.h"
#include "MicroZedHCC.h"

#include <opencv2/opencv.hpp>

#include <QTextStream>
#include <QFile>
#include <QMap>

typedef QPair<uint,uint> slot_t;

Q_DECLARE_METATYPE(slot_t);

class SwissHCCAnalysis : public AnalysisProgram
{
    Q_OBJECT
public:
    SwissHCCAnalysis(FrameGrabber *frameGrabber, Stage *ecs02, MicroZedHCC *microZed, QObject *parent = 0);

    void setLogDirectory(const QString& logDirectory);
    void setValidSlots(const QList<slot_t>& validSlots);

    QMap<slot_t, bool> testResults() const;

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
    void runFindChip(const slot_t& slot);
    void runAlignChip();
    void runChipTest();
    void runChipTestDone(bool result, const QString& testLog);

    void analyze(const QImage& img);
    void analyzeFindProbes(const QImage& img);
    void analyzeFindGroove(const QImage& img);
    void analyzeFindGrooveCross(const QImage& img);
    void analyzeAlignChip(const QImage& img);

    void done();

signals:
    void status(const QString& text);
    void message(const QString& text);
    void donePrepareLoadChips();
    void doneFindProbes();
    void doneFindCross();

    void foundCross(float angle);
    void testCrossAngle(float angle);

    void startFindChips();
    void startFindChip(const slot_t& slot);
    void chipFound(float score);
    void chipAlignSuccess();
    void chipAlignFailed();
    void doneChipTest(bool result, const QString& testLog);
    void doneFindChips();

private:
    QString m_logDirectory;
    QList<slot_t> m_validSlots;
    QMap<slot_t, bool> m_testedSlots;

    // Log
    QFile m_logFH;
    QTextStream  m_log;

    //
    // HCC communication
    MicroZedHCC *m_microZed;

    // State machines for image analysis
    enum ImageAnalysisState {None, FindProbes, FindGroove, FindGrooveCross, AlignChip};
    ImageAnalysisState m_imageAnalysisState;

    // Testing state
    bool m_validSlotList;
    slot_t m_activeSlot;

    // Templates
    cv::Mat m_templateHCC;
    cv::Mat m_templateProbes;

    // Results of analysis
    bool m_edgeFound;
    double m_edgeRadius, m_edgeAngle;

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

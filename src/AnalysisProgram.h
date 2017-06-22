#ifndef ANALYSISPROGRAM_H
#define ANALYSISPROGRAM_H

#include <QThread>
#include <QDockWidget>
#include <QSettings>

#include "FrameGrabber.h"
#include "ECS02.h"

class AnalysisProgram : public QObject
{
    Q_OBJECT
public:
    explicit AnalysisProgram(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent = 0);

    FrameGrabber* getFrameGrabber() const;
    ECS02* getStage() const;

    virtual void settingsSave(QSettings *settings);
    virtual void settingsLoad(QSettings *settings);

signals:
    void finished();
    void updateImage(const QImage &img);

public slots:
    virtual void run() =0;
    virtual void analyze(const QImage &img);
    virtual void done() =0;

private:
    FrameGrabber *m_frameGrabber;
    ECS02 *m_ecs02;
};

#endif // ANALYSISPROGRAM_H

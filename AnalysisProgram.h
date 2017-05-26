#ifndef ANALYSISPROGRAM_H
#define ANALYSISPROGRAM_H

#include <QThread>
#include <QDockWidget>

#include "FrameGrabber.h"
#include "ECS02.h"

class AnalysisProgram : public QObject
{
    Q_OBJECT
public:
    explicit AnalysisProgram(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent = 0);

    FrameGrabber* getFrameGrabber() const;
    ECS02* getECS02() const;

signals:
    void finished();

public slots:
    virtual void run() =0;

private:
    FrameGrabber *m_frameGrabber;
    ECS02 *m_ecs02;
};

#endif // ANALYSISPROGRAM_H

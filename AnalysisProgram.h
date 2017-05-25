#ifndef ANALYSISPROGRAM_H
#define ANALYSISPROGRAM_H

#include <QThread>

#include "FrameGrabber.h"
#include "ECS02.h"

class AnalysisProgram : public QThread
{
    Q_OBJECT
public:
    explicit AnalysisProgram(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent = 0);

public:
    void run() Q_DECL_OVERRIDE;

protected:
    FrameGrabber* getFrameGrabber() const;
    ECS02* getECS02() const;

private:
    FrameGrabber *m_frameGrabber;
    ECS02 *m_ecs02;
};

#endif // ANALYSISPROGRAM_H

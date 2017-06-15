#include "AnalysisProgram.h"

AnalysisProgram::AnalysisProgram(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
    : QObject(parent),
      m_frameGrabber(frameGrabber), m_ecs02(ecs02)
{

}

FrameGrabber* AnalysisProgram::getFrameGrabber() const
{ return m_frameGrabber; }

ECS02* AnalysisProgram::getStage() const
{ return m_ecs02; }

void AnalysisProgram::settingsSave(QSettings *)
{ }

void AnalysisProgram::settingsLoad(QSettings *)
{ }

void AnalysisProgram::analyze(const QImage &img)
{ emit updateImage(img); }

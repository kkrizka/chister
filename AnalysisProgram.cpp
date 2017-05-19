#include "AnalysisProgram.h"

AnalysisProgram::AnalysisProgram(FrameGrabber *frameGrabber, ECS02 *ecs02, QObject *parent)
    : QObject(parent),
      m_frameGrabber(frameGrabber), m_ecs02(ecs02)
{

}

FrameGrabber* AnalysisProgram::getFrameGrabber() const
{ return m_frameGrabber; }

ECS02* AnalysisProgram::getECS02() const
{ return m_ecs02; }

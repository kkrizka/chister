#include "AnalysisProgramGUI.h"

AnalysisProgramGUI::AnalysisProgramGUI(AnalysisProgram *program, QObject *parent)
  : QObject(parent), m_program(program)
{

}

AnalysisProgram* AnalysisProgramGUI::getProgram() const
{ return m_program; }

QDockWidget *AnalysisProgramGUI::getControlDock() const
{ return m_controlDock; }

QDockWidget* AnalysisProgramGUI::createControlDock(QWidget* parent)
{
  m_controlDock = new QDockWidget(parent);
  m_controlDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  //m_controlDock->setWidget(new QLabel(tr("PLEASE WAIT"),m_controlWidget));

  return m_controlDock;
}

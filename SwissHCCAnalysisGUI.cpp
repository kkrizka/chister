#include "SwissHCCAnalysisGUI.h"

#include <QLabel>

#include "SwissHCC_LoadChipsForm.h"
#include "SwissHCC_CrossAlignForm.h"

SwissHCCAnalysisGUI::SwissHCCAnalysisGUI(SwissHCCAnalysis *program, QObject *parent)
    : AnalysisProgramGUI(program,parent)
{
    connect(program, &SwissHCCAnalysis::message, this, &SwissHCCAnalysisGUI::displayMessage);

    connect(program, &SwissHCCAnalysis::stepMoveToLoadDone, this, &SwissHCCAnalysisGUI::createSlotSelection);
    connect(program, &SwissHCCAnalysis::stepCrossFound, this, &SwissHCCAnalysisGUI::createCrossAlign);
}

QDockWidget* SwissHCCAnalysisGUI::createControlDock(QWidget *parent)
{
    QDockWidget *controlDock=AnalysisProgramGUI::createControlDock(parent);

    m_infoWidget=new SwissHCC_InfoForm(controlDock);

    displayMessage(tr("PLEASE WAIT"));

    return controlDock;
}

void SwissHCCAnalysisGUI::displayMessage(const QString &text)
{
    m_infoWidget->setText(text);
    getControlDock()->setWidget(m_infoWidget);
}

void SwissHCCAnalysisGUI::createSlotSelection()
{
    SwissHCC_LoadChipsForm *loadChipsForm=new SwissHCC_LoadChipsForm(getControlDock());
    loadChipsForm->setupSlots(5,3);
    connect(loadChipsForm,&SwissHCC_LoadChipsForm::done,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runCalibration);

    getControlDock()->setWidget(loadChipsForm);
}

void SwissHCCAnalysisGUI::createCrossAlign()
{
    SwissHCC_CrossAlignForm *crossAlignForm=new SwissHCC_CrossAlignForm(getControlDock());
    connect(crossAlignForm,&SwissHCC_CrossAlignForm::done,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runFindChip);
    connect(crossAlignForm,&SwissHCC_CrossAlignForm::test,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runCrossTest);
    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::foundCross,crossAlignForm,&SwissHCC_CrossAlignForm::updateInfo);
    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::testCrossAngle,crossAlignForm,&SwissHCC_CrossAlignForm::updateTestCrossAngle);

    connect(getProgram()->getFrameGrabber(),&FrameGrabber::newImage,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::analyzeCrossAngle);

    getControlDock()->setWidget(crossAlignForm);
}

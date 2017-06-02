#include "SwissHCCAnalysisGUI.h"

#include <QLabel>

#include "SwissHCC_LoadChipsForm.h"
#include "SwissHCC_CrossAlignForm.h"
#include "SwissHCC_ChipTestForm.h"

SwissHCCAnalysisGUI::SwissHCCAnalysisGUI(SwissHCCAnalysis *program, QObject *parent)
    : AnalysisProgramGUI(program,parent)
{
    connect(program, &SwissHCCAnalysis::message, this, &SwissHCCAnalysisGUI::displayMessage);

    connect(program, &SwissHCCAnalysis::stepMoveToLoadDone, this, &SwissHCCAnalysisGUI::createSlotSelection);    
    connect(program, &SwissHCCAnalysis::stepFindCrossDone, this, &SwissHCCAnalysisGUI::createCrossAlign);
    connect(program, &SwissHCCAnalysis::startFindChip, this, &SwissHCCAnalysisGUI::createChipTest);
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
    connect(loadChipsForm,&SwissHCC_LoadChipsForm::slotsSelected,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::setValidSlots);
    connect(loadChipsForm,&SwissHCC_LoadChipsForm::skip,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runFindChips);
    connect(loadChipsForm,&SwissHCC_LoadChipsForm::done,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runFindProbes);

    getControlDock()->setWidget(loadChipsForm);
}

void SwissHCCAnalysisGUI::createCrossAlign()
{
    SwissHCC_CrossAlignForm *crossAlignForm=new SwissHCC_CrossAlignForm(getControlDock());
    connect(crossAlignForm,&SwissHCC_CrossAlignForm::done,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runCrossSave);
    connect(crossAlignForm,&SwissHCC_CrossAlignForm::test,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runCrossTest);
    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::foundCross,crossAlignForm,&SwissHCC_CrossAlignForm::updateInfo);
    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::testCrossAngle,crossAlignForm,&SwissHCC_CrossAlignForm::updateTestCrossAngle);

    getControlDock()->setWidget(crossAlignForm);
}

void SwissHCCAnalysisGUI::createChipTest()
{
    SwissHCC_ChipTestForm *chipTestForm=new SwissHCC_ChipTestForm(getControlDock());
    chipTestForm->setupSlots(5,3);
    connect(chipTestForm,&SwissHCC_ChipTestForm::findChip ,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runFindChip);
    connect(chipTestForm,&SwissHCC_ChipTestForm::alignChip,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runAlignChip);
    connect(chipTestForm,&SwissHCC_ChipTestForm::testChip ,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runChipTest);

    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::findingChip,chipTestForm,&SwissHCC_ChipTestForm::disableChipAlignScore);
    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::chipFound  ,chipTestForm,&SwissHCC_ChipTestForm::updateChipAlignScore);

    getControlDock()->setWidget(chipTestForm);
}

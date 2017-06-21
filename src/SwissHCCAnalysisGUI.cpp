#include "SwissHCCAnalysisGUI.h"

#include <QLabel>
#include <QApplication>

#include "SwissHCCPreferencesForm.h"

SwissHCCAnalysisGUI::SwissHCCAnalysisGUI(SwissHCCAnalysis *program, QObject *parent)
    : AnalysisProgramGUI(program,parent),
      m_templates(QApplication::applicationDirPath()+"/SwissHCCTemplates/")
{
  m_templates.load();

  connect(program, &SwissHCCAnalysis::message, this, &SwissHCCAnalysisGUI::displayMessage);

  connect(this, &SwissHCCAnalysisGUI::startLoadChips,         dynamic_cast<SwissHCCAnalysis*>(getProgram()), &SwissHCCAnalysis::runLoadChips);
  connect(this, &SwissHCCAnalysisGUI::startFindProbes,        dynamic_cast<SwissHCCAnalysis*>(getProgram()), &SwissHCCAnalysis::runFindProbes);
  connect(this, &SwissHCCAnalysisGUI::startCalibratePosition, dynamic_cast<SwissHCCAnalysis*>(getProgram()), &SwissHCCAnalysis::runCalibratePosition);
  connect(this, &SwissHCCAnalysisGUI::startFindChips,         dynamic_cast<SwissHCCAnalysis*>(getProgram()), &SwissHCCAnalysis::runFindChips);

  connect(program, &SwissHCCAnalysis::donePrepareLoadChips, this, &SwissHCCAnalysisGUI::showSlotSelection);
  connect(program, &SwissHCCAnalysis::doneFindProbes,       this, &SwissHCCAnalysisGUI::showProbeCheck);
  connect(program, &SwissHCCAnalysis::doneFindCross,        this, &SwissHCCAnalysisGUI::showCrossAlign);
  connect(program, &SwissHCCAnalysis::startFindChip,        this, &SwissHCCAnalysisGUI::showChipTest);
  connect(program, &SwissHCCAnalysis::doneFindChips,        this, &SwissHCCAnalysisGUI::showSummary);
}

void SwissHCCAnalysisGUI::createPreferencesForm(PreferencesDialog *prefDialog)
{
  prefDialog->addForm(tr("Swiss HCC"), new SwissHCCPreferencesForm(&m_templates));
}

QDockWidget* SwissHCCAnalysisGUI::createControlDock(QWidget *parent)
{
    QDockWidget *controlDock=AnalysisProgramGUI::createControlDock(parent);
    controlDock->setAttribute( Qt::WA_DeleteOnClose );
    connect(controlDock, &QWidget::destroyed, getProgram(), &AnalysisProgram::done);

    //
    // Create all of the necessary control widgets

    // Generic information
    m_infoWidget=new SwissHCC_InfoForm(controlDock);
    displayMessage(tr("PLEASE WAIT"));

    createConfigure(); // Test procedure configuration dialog
    createSlotSelection(); // Slot selection dialog
    createCrossAlign(); // Cross alignment dialog
    createChipTest(); // Chip testing dialog
    createSummary(); // Testing summary

    return controlDock;
}

void SwissHCCAnalysisGUI::displayMessage(const QString &text)
{
    m_infoWidget->setText(text);
    getControlDock()->setWidget(m_infoWidget);
}

void SwissHCCAnalysisGUI::createConfigure()
{
    m_configureForm=new SwissHCC_ConfigureForm(getControlDock());

    connect(m_configureForm,&SwissHCC_ConfigureForm::done,this,&SwissHCCAnalysisGUI::configure);

    m_configureForm->hide();
}

void SwissHCCAnalysisGUI::createSlotSelection()
{
    m_loadChipsForm=new SwissHCC_LoadChipsForm(getControlDock());
    m_loadChipsForm->setupSlots(5,3);

    connect(m_loadChipsForm,&SwissHCC_LoadChipsForm::done,this,&SwissHCCAnalysisGUI::slotSelection);

    m_loadChipsForm->hide();
}

void SwissHCCAnalysisGUI::createCrossAlign()
{
    m_crossAlignForm=new SwissHCC_CrossAlignForm(getControlDock());

    connect(m_crossAlignForm,&SwissHCC_CrossAlignForm::done,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runCrossSave);
    connect(m_crossAlignForm,&SwissHCC_CrossAlignForm::test,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runCrossTest);

    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::foundCross,    m_crossAlignForm,&SwissHCC_CrossAlignForm::updateInfo);
    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::testCrossAngle,m_crossAlignForm,&SwissHCC_CrossAlignForm::updateTestCrossAngle);

    m_crossAlignForm->hide();
}

void SwissHCCAnalysisGUI::createChipTest()
{
    m_chipTestForm=new SwissHCC_ChipTestForm(getControlDock());
    m_chipTestForm->setupSlots(5,3);

    connect(m_chipTestForm,&SwissHCC_ChipTestForm::findChip ,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runFindChip);
    connect(m_chipTestForm,&SwissHCC_ChipTestForm::alignChip,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runAlignChip);
    connect(m_chipTestForm,&SwissHCC_ChipTestForm::testChip ,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runChipTest);
    connect(m_chipTestForm,&SwissHCC_ChipTestForm::nextChip ,dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::runFindChips);

    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::startFindChip,m_chipTestForm,&SwissHCC_ChipTestForm::updateChipSlot);
    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::chipFound    ,m_chipTestForm,&SwissHCC_ChipTestForm::updateChipAlignScore);
    connect(dynamic_cast<SwissHCCAnalysis*>(getProgram()),&SwissHCCAnalysis::doneChipTest ,m_chipTestForm,&SwissHCC_ChipTestForm::updateChipStatus);

    m_chipTestForm->hide();
}

void SwissHCCAnalysisGUI::createSummary()
{
    m_summaryForm=new SwissHCC_SummaryForm(getControlDock());
    m_summaryForm->setupSlots(5,3);

    connect(m_summaryForm, &SwissHCC_SummaryForm::done, this, &SwissHCCAnalysisGUI::done);

    m_summaryForm->hide();
}

void SwissHCCAnalysisGUI::showConfigure()
{
  m_configureForm->setTemplates(m_templates.list());
    getControlDock()->setWidget(m_configureForm);
}

void SwissHCCAnalysisGUI::showSlotSelection()
{
    getControlDock()->setWidget(m_loadChipsForm);
}

void SwissHCCAnalysisGUI::showCrossAlign()
{
    getControlDock()->setWidget(m_crossAlignForm);
}

void SwissHCCAnalysisGUI::showProbeCheck()
{
    if(m_calibratePosition)
        emit startCalibratePosition();
    else
        emit startFindChips();
}

void SwissHCCAnalysisGUI::showChipTest()
{
    getControlDock()->setWidget(m_chipTestForm);
}

void SwissHCCAnalysisGUI::showSummary()
{
    getControlDock()->setWidget(m_summaryForm);
}

void SwissHCCAnalysisGUI::configure(const QString& chipTemplate, bool loadChips, bool findProbes, bool calibratePosition, const QString& logDirectory)
{
    m_loadChips=loadChips;
    m_findProbes=findProbes;
    m_calibratePosition=calibratePosition;

    SwissHCCAnalysis *analysis=dynamic_cast<SwissHCCAnalysis*>(getProgram());
    analysis->setChipTemplate(m_templates.get(chipTemplate));
    analysis->setLogDirectory(logDirectory);

    if(m_loadChips)
        emit startLoadChips();
    else if(m_findProbes)
        emit startFindProbes();
    else if(m_calibratePosition)
        emit startCalibratePosition();
    else
        emit startFindChips();
}

void SwissHCCAnalysisGUI::slotSelection(const QList<slot_t> &validSlots)
{
    dynamic_cast<SwissHCCAnalysis*>(getProgram())->setValidSlots(validSlots);

    if(m_findProbes)
        emit startFindProbes();
    else if(m_calibratePosition)
        emit startCalibratePosition();
    else
        emit startFindChips();
}

void SwissHCCAnalysisGUI::done()
{
    getControlDock()->close();
}


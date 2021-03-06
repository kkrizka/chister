#include "DicedChipAnalysisGUI.h"

#include <QLabel>
#include <QApplication>
#include <QDebug>

#include "DicedChipPreferencesForm.h"

DicedChipAnalysisGUI::DicedChipAnalysisGUI(DicedChipAnalysis *program, QObject *parent)
    : AnalysisProgramGUI(program,parent),
      m_templates(QApplication::applicationDirPath()+"/DicedChipTemplates/")
{
  m_templates.load();

  connect(program, &DicedChipAnalysis::message, this, &DicedChipAnalysisGUI::displayMessage);

  connect(this, &DicedChipAnalysisGUI::startLoadChips,         dynamic_cast<DicedChipAnalysis*>(getProgram()), &DicedChipAnalysis::runLoadChips);
  connect(this, &DicedChipAnalysisGUI::startFindProbes,        dynamic_cast<DicedChipAnalysis*>(getProgram()), &DicedChipAnalysis::runFindProbes);
  connect(this, &DicedChipAnalysisGUI::startCalibratePosition, dynamic_cast<DicedChipAnalysis*>(getProgram()), &DicedChipAnalysis::runCalibratePosition);
  connect(this, &DicedChipAnalysisGUI::startFindChips,         dynamic_cast<DicedChipAnalysis*>(getProgram()), &DicedChipAnalysis::runFindChips);
  connect(this, &DicedChipAnalysisGUI::startTestChips,         dynamic_cast<DicedChipAnalysis*>(getProgram()), &DicedChipAnalysis::runTestChips);

  connect(program, &DicedChipAnalysis::donePrepareLoadChips, this, &DicedChipAnalysisGUI::showSlotSelection);\
  connect(program, &DicedChipAnalysis::doneFindProbes,       this, &DicedChipAnalysisGUI::showProbeCheck);
  connect(program, &DicedChipAnalysis::doneFindCross,        this, &DicedChipAnalysisGUI::showCrossAlign);
  connect(program, &DicedChipAnalysis::startFindChip,        this, &DicedChipAnalysisGUI::showChipTest);
}

void DicedChipAnalysisGUI::createPreferencesForm(PreferencesDialog *prefDialog)
{
  prefDialog->addForm(tr("Diced Chips"), new DicedChipPreferencesForm(&m_templates));
}

QDockWidget* DicedChipAnalysisGUI::createControlDock(QWidget *parent)
{
  QDockWidget *controlDock=AnalysisProgramGUI::createControlDock(parent);
  controlDock->setAttribute( Qt::WA_DeleteOnClose );
  connect(controlDock, &QWidget::destroyed, getProgram(), &AnalysisProgram::done);

  //
  // Create all of the necessary control widgets

  // Generic information
  m_infoWidget=new DicedChip_InfoForm(controlDock);
  displayMessage(tr("PLEASE WAIT"));

  createConfigure(); // Test procedure configuration dialog
  createSlotSelection(); // Slot selection dialog
  createCrossAlign(); // Cross alignment dialog
  createChipTest(); // Chip testing dialog
  createSummary(); // Testing summary

  return controlDock;
}

void DicedChipAnalysisGUI::displayMessage(const QString &text)
{
  m_infoWidget->setText(text);
  getControlDock()->setWidget(m_infoWidget);
}

void DicedChipAnalysisGUI::createConfigure()
{
  m_configureForm=new DicedChip_ConfigureForm(getControlDock());

  connect(m_configureForm,&DicedChip_ConfigureForm::done,this,&DicedChipAnalysisGUI::configure);

  m_configureForm->hide();
}

void DicedChipAnalysisGUI::createSlotSelection()
{
  m_loadChipsForm=new DicedChip_LoadChipsForm(getControlDock());
  m_loadChipsForm->setupSlots(6,5);

  connect(m_loadChipsForm,&DicedChip_LoadChipsForm::done,this,&DicedChipAnalysisGUI::slotSelection);

  m_loadChipsForm->hide();
}

void DicedChipAnalysisGUI::createCrossAlign()
{
    m_crossAlignForm=new DicedChip_CrossAlignForm(getControlDock());

    connect(m_crossAlignForm,&DicedChip_CrossAlignForm::done,dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::runCrossSave);
    connect(m_crossAlignForm,&DicedChip_CrossAlignForm::test,dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::runCrossTest);

    connect(dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::foundCross,    m_crossAlignForm,&DicedChip_CrossAlignForm::updateInfo);
    connect(dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::testCrossAngle,m_crossAlignForm,&DicedChip_CrossAlignForm::updateTestCrossAngle);

    m_crossAlignForm->hide();
}

void DicedChipAnalysisGUI::createChipTest()
{
  m_chipTestForm=new DicedChip_ChipTestForm(getControlDock());
  m_chipTestForm->setupSlots(6,5);

  connect(m_chipTestForm,&DicedChip_ChipTestForm::findChip      ,dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::runFindChip);
  connect(m_chipTestForm,&DicedChip_ChipTestForm::alignChip     ,dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::runAlignChip);
  connect(m_chipTestForm,&DicedChip_ChipTestForm::confirmChip   ,dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::runChipSave);
  connect(m_chipTestForm,&DicedChip_ChipTestForm::skipChip      ,dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::runChipSkip);
  connect(m_chipTestForm,&DicedChip_ChipTestForm::nextChip      ,dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::runTestChips);
  connect(m_chipTestForm,&DicedChip_ChipTestForm::testChip      ,dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::runChipTest);

  connect(dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::startFindChip,m_chipTestForm,&DicedChip_ChipTestForm::updateChipSlot);
  connect(dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::chipFound    ,m_chipTestForm,&DicedChip_ChipTestForm::updateChipTestScore);
  connect(dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::chipUpdated  ,m_chipTestForm,&DicedChip_ChipTestForm::updateChipSlotStyle);
  connect(dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::doneFindChips,m_chipTestForm,&DicedChip_ChipTestForm::prepareTestChips);
  connect(dynamic_cast<DicedChipAnalysis*>(getProgram()),&DicedChipAnalysis::doneTestChips,m_chipTestForm,&DicedChip_ChipTestForm::prepareAlignChips);

  m_chipTestForm->hide();
}

void DicedChipAnalysisGUI::createSummary()
{
  m_summaryForm=new DicedChip_SummaryForm(getControlDock());
  m_summaryForm->setupSlots(6,5);

  connect(m_summaryForm, &DicedChip_SummaryForm::done, this, &DicedChipAnalysisGUI::done);

  m_summaryForm->hide();
}

void DicedChipAnalysisGUI::showConfigure()
{
  m_configureForm->setTemplates(m_templates.list());
  getControlDock()->setWidget(m_configureForm);
}

void DicedChipAnalysisGUI::showSlotSelection()
{
  getControlDock()->setWidget(m_loadChipsForm);
}

void DicedChipAnalysisGUI::showCrossAlign()
{
  getControlDock()->setWidget(m_crossAlignForm);
}

void DicedChipAnalysisGUI::showProbeCheck()
{
  if(m_calibratePosition)
    emit startCalibratePosition();
  else
    emit startFindChips();
}

void DicedChipAnalysisGUI::showChipTest()
{
  getControlDock()->setWidget(m_chipTestForm);
}

void DicedChipAnalysisGUI::showSummary()
{
  getControlDock()->setWidget(m_summaryForm);
}

void DicedChipAnalysisGUI::configure(const QString& chipTemplate, bool loadChips, bool findProbes, bool calibratePosition, const QString& logDirectory)
{
    m_loadChips=loadChips;
    m_findProbes=findProbes;
    m_calibratePosition=calibratePosition;

    DicedChipAnalysis *analysis=dynamic_cast<DicedChipAnalysis*>(getProgram());
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

void DicedChipAnalysisGUI::slotSelection(const QList<DicedChipSlot*> &validSlots)
{
  dynamic_cast<DicedChipAnalysis*>(getProgram())->setValidSlots(validSlots);
  m_chipTestForm->setValidSlots(validSlots);

  if(m_findProbes)
    emit startFindProbes();
  else if(m_calibratePosition)
    emit startCalibratePosition();
  else
    emit startFindChips();
}

void DicedChipAnalysisGUI::done()
{
  getControlDock()->close();
}


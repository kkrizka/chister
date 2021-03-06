#ifndef SWISSHCCANALYSISGUI_H
#define SWISSHCCANALYSISGUI_H

#include "AnalysisProgramGUI.h"
#include "PreferencesDialog.h"

#include "SwissHCCAnalysis.h"
#include "SwissHCCTemplateManager.h"

#include "SwissHCC_InfoForm.h"
#include "SwissHCC_ConfigureForm.h"
#include "SwissHCC_LoadChipsForm.h"
#include "SwissHCC_CrossAlignForm.h"
#include "SwissHCC_ChipTestForm.h"
#include "SwissHCC_SummaryForm.h"

class SwissHCCAnalysisGUI : public AnalysisProgramGUI
{
  Q_OBJECT
public:
  SwissHCCAnalysisGUI(SwissHCCAnalysis *program, QObject *parent=0);

  virtual void createPreferencesForm(PreferencesDialog *prefDialog);
  virtual QDockWidget* createControlDock(QWidget *parent);

public slots:
  void displayMessage(const QString &text);

  void showConfigure();
  void showSlotSelection();
  void showProbeCheck();
  void showCrossAlign();
  void showChipTest();
  void showSummary();

  void configure(const QString& chipTemplate, bool loadChips, bool findProbes, bool calibratePosition, const QString& logDirectory);
  void slotSelection(const QList<slot_t>& validSlots);
  void done();

signals:
  void startLoadChips();
  void startFindProbes();
  void startCalibratePosition();
  void startFindChips();

private:
  SwissHCCTemplateManager m_templates;

  SwissHCC_InfoForm *m_infoWidget;
  SwissHCC_ConfigureForm *m_configureForm;
  SwissHCC_LoadChipsForm *m_loadChipsForm;
  SwissHCC_CrossAlignForm *m_crossAlignForm;
  SwissHCC_ChipTestForm *m_chipTestForm;
  SwissHCC_SummaryForm *m_summaryForm;

  bool m_loadChips;
  bool m_findProbes;
  bool m_calibratePosition;

  void createConfigure();
  void createSlotSelection();
  void createCrossAlign();
  void createChipTest();
  void createSummary();
};

#endif // SWISSHCCANALYSISGUI_H

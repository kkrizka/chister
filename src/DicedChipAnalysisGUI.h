#ifndef DICEDCHIPANALYSISGUI_H
#define DICEDCHIPANALYSISGUI_H

#include "AnalysisProgramGUI.h"
#include "PreferencesDialog.h"

#include "DicedChipAnalysis.h"
#include "DicedChipTemplateManager.h"

#include "DicedChip_InfoForm.h"
#include "DicedChip_ConfigureForm.h"
#include "DicedChip_LoadChipsForm.h"
#include "DicedChip_CrossAlignForm.h"
#include "DicedChip_ChipTestForm.h"
#include "DicedChip_SummaryForm.h"

class DicedChipAnalysisGUI : public AnalysisProgramGUI
{
  Q_OBJECT
public:
  DicedChipAnalysisGUI(DicedChipAnalysis *program, QObject *parent=0);

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
  DicedChipTemplateManager m_templates;

  DicedChip_InfoForm *m_infoWidget;
  DicedChip_ConfigureForm *m_configureForm;
  DicedChip_LoadChipsForm *m_loadChipsForm;
  DicedChip_CrossAlignForm *m_crossAlignForm;
  DicedChip_ChipTestForm *m_chipTestForm;
  DicedChip_SummaryForm *m_summaryForm;

  bool m_loadChips;
  bool m_findProbes;
  bool m_calibratePosition;

  void createConfigure();
  void createSlotSelection();
  void createCrossAlign();
  void createChipTest();
  void createSummary();
};

#endif // DICEDCHIPANALYSISGUI_H

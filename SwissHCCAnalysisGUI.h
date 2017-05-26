#ifndef SIWSSHCCANALYSISGUI_H
#define SIWSSHCCANALYSISGUI_H

#include "AnalysisProgramGUI.h"
#include "SwissHCCAnalysis.h"
#include "SwissHCC_InfoForm.h"

class SwissHCCAnalysisGUI : public AnalysisProgramGUI
{
    Q_OBJECT
public:
    SwissHCCAnalysisGUI(SwissHCCAnalysis *program, QObject *parent=0);

    virtual QDockWidget* createControlDock(QWidget *parent);

public slots:
    void displayMessage(const QString &text);
    void createSlotSelection();
    void createCrossAlign();

private:
    SwissHCC_InfoForm *m_infoWidget;
};

#endif // SIWSSHCCANALYSISGUI_H

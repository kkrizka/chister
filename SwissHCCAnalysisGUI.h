#ifndef SIWSSHCCANALYSISGUI_H
#define SIWSSHCCANALYSISGUI_H

#include "AnalysisProgramGUI.h"
#include "SwissHCCAnalysis.h"
#include "SwissHCC_InfoForm.h"

Q_DECLARE_METATYPE(QList<QPoint>);

class SwissHCCAnalysisGUI : public AnalysisProgramGUI
{
    Q_OBJECT
public:
    SwissHCCAnalysisGUI(SwissHCCAnalysis *program, QObject *parent=0);

    virtual QDockWidget* createControlDock(QWidget *parent);

public slots:
    void displayMessage(const QString &text);
    void createConfigure();
    void createSlotSelection();
    void createProbeCheck();
    void createCrossAlign();
    void createChipTest();

    void configure(bool loadChips, bool findProbes, bool calibratePosition, const QString& logDirectory);
    void slotSelection(const QList<QPoint>& validSlots);

signals:
    void startLoadChips();
    void startFindProbes();
    void startCalibratePosition();
    void startFindChips();

private:
    SwissHCC_InfoForm *m_infoWidget;

    bool m_loadChips;
    bool m_findProbes;
    bool m_calibratePosition;
};

#endif // SIWSSHCCANALYSISGUI_H

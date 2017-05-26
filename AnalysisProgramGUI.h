#ifndef ANALYSISPROGRAMGUI_H
#define ANALYSISPROGRAMGUI_H

#include <QObject>
#include <QDockWidget>

#include "AnalysisProgram.h"

class AnalysisProgramGUI : public QObject
{
    Q_OBJECT
public:
    explicit AnalysisProgramGUI(AnalysisProgram *program, QObject *parent = 0);

    AnalysisProgram* getProgram() const;
    QDockWidget *getControlDock() const;

    virtual QDockWidget* createControlDock(QWidget *parent=0);

signals:

private:
    AnalysisProgram *m_program;
    QDockWidget *m_controlDock;
};

#endif // ANALYSISPROGRAMGUI_H

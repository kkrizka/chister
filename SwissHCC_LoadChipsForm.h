#ifndef SWISSHCC_LOADCHIPSFORM_H
#define SWISSHCC_LOADCHIPSFORM_H

#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QList>
#include <QPoint>

#include "SwissHCCAnalysis.h"

namespace Ui {
class SwissHCC_LoadChipsForm;
}

class SwissHCC_LoadChipsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SwissHCC_LoadChipsForm(QWidget *parent = 0);
    ~SwissHCC_LoadChipsForm();

    void setupSlots(uint nX, uint nY);

signals:
    void done(const QList<slot_t>& slots);

private slots:
    void on_donePushButton_clicked();
    void on_selectPushButton_clicked();
    void on_deselectPushButton_clicked();

private:
    Ui::SwissHCC_LoadChipsForm *ui;

    QMap<QPushButton*, slot_t> m_slotChecks;
};

#endif // SWISSHCC_LOADCHIPSFORM_H

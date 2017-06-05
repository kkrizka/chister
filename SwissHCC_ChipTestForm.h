#ifndef SWISSHCC_CHIPTESTFORM_H
#define SWISSHCC_CHIPTESTFORM_H

#include <QWidget>
#include <QPushButton>
#include <QMap>

#include "SwissHCCAnalysis.h"

namespace Ui {
class SwissHCC_ChipTestForm;
}

class SwissHCC_ChipTestForm : public QWidget
{
    Q_OBJECT

public:
    explicit SwissHCC_ChipTestForm(QWidget *parent = 0);
    ~SwissHCC_ChipTestForm();

    void setupSlots(uint nX, uint nY);

public slots:
    void updateChipSlot(const slot_t& slot);
    void updateChipAlignScore(float chipOffsetScore);
    void updateChipStatus(bool result);

signals:
    void findChip(const slot_t& slot);
    void alignChip();
    void testChip();
    void nextChip();

private slots:
    void on_slotPushButton_clicked();
    void on_redoPushButton_clicked();
    void on_confirmPushButton_clicked();
    void on_skipPushButton_clicked();

private:
    Ui::SwissHCC_ChipTestForm *ui;

    QMap<QPushButton*, slot_t> m_slotChecks;
};

#endif // SWISSHCC_CHIPTESTFORM_H

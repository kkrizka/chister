#ifndef DICEDCHIP_CHIPTESTFORM_H
#define DICEDCHIP_CHIPTESTFORM_H

#include <QWidget>
#include <QPushButton>
#include <QMap>

#include "DicedChipAnalysis.h"

namespace Ui {
class DicedChip_ChipTestForm;
}

class DicedChip_ChipTestForm : public QWidget
{
    Q_OBJECT

public:
    explicit DicedChip_ChipTestForm(QWidget *parent = 0);
    ~DicedChip_ChipTestForm();

    void setupSlots(uint nX, uint nY);

public slots:
    void updateChipSlot(const slot_t& slot);
    void updateChipAlignScore(float chipOffsetScore);
    void updateChipStatus(bool result, const QString& testLog);

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
    Ui::DicedChip_ChipTestForm *ui;

    QMap<QPushButton*, slot_t> m_slotChecks;
};

#endif // DICEDCHIP_CHIPTESTFORM_H

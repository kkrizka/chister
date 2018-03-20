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

  //! Configure the slots with valid slot objects
  void setValidSlots(const QList<DicedChipSlot*>& validSlots);

public slots:
  void updateChipSlot(DicedChipSlot* slot);
  void updateChipTestScore(float chipOffsetScore);
  void updateChipSlotStyle(const DicedChipSlot* slot);
  void prepareAlignChips();
  void prepareTestChips();

signals:
  void findChip(DicedChipSlot* slot);
  void alignChip();
  void confirmChip();
  void skipChip();
  void nextChip();
  void testChip();

private slots:
  void on_slotPushButton_clicked();
  void on_redoPushButton_clicked();
  void on_confirmPushButton_clicked();
  void on_skipPushButton_clicked();
  void on_nextPushButton_clicked();
  void on_testPushButton_clicked();

private:
  Ui::DicedChip_ChipTestForm *ui;

  QMap<QPushButton*, DicedChipSlot*> m_slotChecks;
};

#endif // DICEDCHIP_CHIPTESTFORM_H

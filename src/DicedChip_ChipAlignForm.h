#ifndef DICEDCHIP_CHIPALIGNFORM_H
#define DICEDCHIP_CHIPALIGNFORM_H

#include <QWidget>
#include <QPushButton>
#include <QMap>

#include "DicedChipAnalysis.h"

namespace Ui {
class DicedChip_ChipAlignForm;
}

class DicedChip_ChipAlignForm : public QWidget
{
  Q_OBJECT

public:
  explicit DicedChip_ChipAlignForm(QWidget *parent = 0);
  ~DicedChip_ChipAlignForm();

  void setupSlots(uint nX, uint nY);

  //! Configure the slots with valid slot objects
  void setValidSlots(const QList<DicedChipSlot*>& validSlots);

public slots:
  void updateChipSlot(DicedChipSlot* slot);
  void updateChipAlignScore(float chipOffsetScore);
  void updateChipSlotStyle(const DicedChipSlot* slot);

signals:
  void findChip(DicedChipSlot* slot);
  void alignChip();
  void confirmChip();
  void nextChip();

private slots:
  void on_slotPushButton_clicked();
  void on_redoPushButton_clicked();
  void on_confirmPushButton_clicked();
  void on_skipPushButton_clicked();

private:
  Ui::DicedChip_ChipAlignForm *ui;

  QMap<QPushButton*, DicedChipSlot*> m_slotChecks;
  DicedChipSlot *m_activeSlot =nullptr;
};

#endif // DICEDCHIP_CHIPTESTFORM_H

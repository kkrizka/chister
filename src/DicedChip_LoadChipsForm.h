#ifndef DICEDCHIP_LOADCHIPSFORM_H
#define DICEDCHIP_LOADCHIPSFORM_H

#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QList>
#include <QPoint>

#include "DicedChipSlot.h"

namespace Ui {
  class DicedChip_LoadChipsForm;
}

class DicedChip_LoadChipsForm : public QWidget
{
  Q_OBJECT

public:
  explicit DicedChip_LoadChipsForm(QWidget *parent = 0);
  ~DicedChip_LoadChipsForm();

  void setupSlots(uint nX, uint nY);

signals:
  void done(const QList<DicedChipSlot*>& slots);

private slots:
  void on_slotPushButton_clicked(bool checked);
  void on_donePushButton_clicked();
  void on_selectPushButton_clicked();
  void on_deselectPushButton_clicked();

private:
  Ui::DicedChip_LoadChipsForm *ui;

  QMap<QPushButton*, DicedChipSlot*> m_slotChecks;

  QString m_lastChipName;
};

#endif // DICEDCHIP_LOADCHIPSFORM_H

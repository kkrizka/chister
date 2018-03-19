#ifndef DICEDCHIP_SUMMARYFORM_H
#define DICEDCHIP_SUMMARYFORM_H

#include <QWidget>
#include <QLabel>
#include <QMap>

#include "Definitions.h"
#include "DicedChipSlot.h"

namespace Ui {
  class DicedChip_SummaryForm;
}

class DicedChip_SummaryForm : public QWidget
{
  Q_OBJECT

public:
  explicit DicedChip_SummaryForm(QWidget *parent = 0);
  ~DicedChip_SummaryForm();

  void setupSlots(uint nX, uint nY);
  void setResults(const QList<DicedChipSlot*>& results);

signals:
  void done();

private slots:
  void on_closePushButton_clicked();

private:
  Ui::DicedChip_SummaryForm *ui;

  QMap<slot_t, QLabel*> m_slotLabels;
};

#endif // DICEDCHIP_SUMMARYFORM_H

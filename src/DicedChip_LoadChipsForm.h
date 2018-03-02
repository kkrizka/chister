#ifndef DICEDCHIP_LOADCHIPSFORM_H
#define DICEDCHIP_LOADCHIPSFORM_H

#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QList>
#include <QPoint>

#include "DicedChipAnalysis.h"

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
    void done(const QList<slot_t>& slots);

private slots:
    void on_donePushButton_clicked();
    void on_selectPushButton_clicked();
    void on_deselectPushButton_clicked();

private:
    Ui::DicedChip_LoadChipsForm *ui;

    QMap<QPushButton*, slot_t> m_slotChecks;
};

#endif // DICEDCHIP_LOADCHIPSFORM_H

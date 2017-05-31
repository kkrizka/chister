#ifndef SWISSHCC_CHIPTESTFORM_H
#define SWISSHCC_CHIPTESTFORM_H

#include <QWidget>
#include <QPushButton>
#include <QMap>

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

signals:
    void findChip(const QPoint& slot);
    void alignChip();
    void testChip();

private slots:
    void on_slotPushButton_clicked();
    void on_redoPushButton_clicked();
    void on_confirmPushButton_clicked();

private:
    Ui::SwissHCC_ChipTestForm *ui;

    QMap<QPushButton*, QPoint> m_slotChecks;
};

#endif // SWISSHCC_CHIPTESTFORM_H

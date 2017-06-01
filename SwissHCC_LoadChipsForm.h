#ifndef SWISSHCC_LOADCHIPSFORM_H
#define SWISSHCC_LOADCHIPSFORM_H

#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QList>
#include <QPoint>

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
    void slotsSelected(const QList<QPoint>& slots);
    void done();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SwissHCC_LoadChipsForm *ui;

    QMap<QPushButton*, QPoint> m_slotChecks;
};

#endif // SWISSHCC_LOADCHIPSFORM_H

#ifndef SWISSHCC_SUMMARYFORM_H
#define SWISSHCC_SUMMARYFORM_H

#include <QWidget>
#include <QLabel>
#include <QMap>

#include "SwissHCCAnalysis.h"

namespace Ui {
class SwissHCC_SummaryForm;
}

class SwissHCC_SummaryForm : public QWidget
{
    Q_OBJECT

public:
    explicit SwissHCC_SummaryForm(QWidget *parent = 0);
    ~SwissHCC_SummaryForm();

    void setupSlots(uint nX, uint nY);
    void setResults(const QMap<slot_t, bool>& results);

signals:
    void done();

private slots:
    void on_closePushButton_clicked();

private:
    Ui::SwissHCC_SummaryForm *ui;

    QMap<slot_t, QLabel*> m_slotLabels;
};

#endif // SWISSHCC_SUMMARYFORM_H

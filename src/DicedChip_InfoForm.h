#ifndef DICEDCHIP_INFOFORM_H
#define DICEDCHIP_INFOFORM_H

#include <QWidget>

namespace Ui {
class DicedChip_InfoForm;
}

class DicedChip_InfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit DicedChip_InfoForm(QWidget *parent = 0);
    ~DicedChip_InfoForm();

    void setText(const QString& text);

private:
    Ui::DicedChip_InfoForm *ui;
};

#endif // DICEDCHIP_INFOFORM_H

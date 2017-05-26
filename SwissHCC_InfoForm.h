#ifndef SWISSHCC_INFOFORM_H
#define SWISSHCC_INFOFORM_H

#include <QWidget>

namespace Ui {
class SwissHCC_InfoForm;
}

class SwissHCC_InfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit SwissHCC_InfoForm(QWidget *parent = 0);
    ~SwissHCC_InfoForm();

    void setText(const QString& text);

private:
    Ui::SwissHCC_InfoForm *ui;
};

#endif // SWISSHCC_INFOFORM_H

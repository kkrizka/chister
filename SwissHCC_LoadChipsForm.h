#ifndef SWISSHCC_LOADCHIPSFORM_H
#define SWISSHCC_LOADCHIPSFORM_H

#include <QWidget>

namespace Ui {
class SwissHCC_LoadChipsForm;
}

class SwissHCC_LoadChipsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SwissHCC_LoadChipsForm(QWidget *parent = 0);
    ~SwissHCC_LoadChipsForm();

private:
    Ui::SwissHCC_LoadChipsForm *ui;
};

#endif // SWISSHCC_LOADCHIPSFORM_H

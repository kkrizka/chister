#ifndef DICEDCHIP_CROSSALIGNFORM_H
#define DICEDCHIP_CROSSALIGNFORM_H

#include <QWidget>

namespace Ui {
class DicedChip_CrossAlignForm;
}

class DicedChip_CrossAlignForm : public QWidget
{
    Q_OBJECT

public:
    explicit DicedChip_CrossAlignForm(QWidget *parent = 0);
    ~DicedChip_CrossAlignForm();

public slots:
    void updateInfo(float angle);
    void updateTestCrossAngle(float angle);

signals:
    void test();
    void done();

private slots:
    void on_continuePushButton_clicked();
    void on_testPushButton_clicked();

private:
    Ui::DicedChip_CrossAlignForm *ui;
};

#endif // DICEDCHIP_CROSSALIGNFORM_H

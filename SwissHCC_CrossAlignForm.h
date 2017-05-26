#ifndef SWISSHCC_CROSSALIGNFORM_H
#define SWISSHCC_CROSSALIGNFORM_H

#include <QWidget>

namespace Ui {
class SwissHCC_CrossAlignForm;
}

class SwissHCC_CrossAlignForm : public QWidget
{
    Q_OBJECT

public:
    explicit SwissHCC_CrossAlignForm(QWidget *parent = 0);
    ~SwissHCC_CrossAlignForm();

public slots:
    void updateInfo(float x, float y, float angle);
    void updateTestCrossAngle(float angle);

signals:
    void test();
    void done();

private slots:
    void on_continuePushButton_clicked();
    void on_testPushButton_clicked();

private:
    Ui::SwissHCC_CrossAlignForm *ui;
};

#endif // SWISSHCC_CROSSALIGNFORM_H

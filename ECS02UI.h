#ifndef ECS02UI_H
#define ECS02UI_H

#include <QWidget>

#include "ECS02.h"

namespace Ui {
class ECS02UI;
}

class ECS02UI : public QWidget
{
    Q_OBJECT

public:
    explicit ECS02UI(QWidget *parent = 0);
    ~ECS02UI();

    void setDevice(ECS02 *device);

public slots:
    void updateInfo();

private slots:
    void on_separateButton_clicked();

    void on_moveChuckUpButton_clicked();
    void on_moveChuckDownButton_clicked();
    void on_moveChuckLeftButton_clicked();
    void on_moveChuckRightButton_clicked();

    void on_homeButton_clicked();
    void on_loadButton_clicked();

private:
    Ui::ECS02UI *ui;

    ECS02 *m_device;
};

#endif // ECS02UI_H

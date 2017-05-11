#include "ECS02UI.h"
#include "ui_ECS02UI.h"

#include <QDebug>

ECS02UI::ECS02UI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ECS02UI),
    m_device(0),
    m_xSpeed(0), m_ySpeed(0)
{
    ui->setupUi(this);
}

ECS02UI::~ECS02UI()
{
    delete ui;
}

void ECS02UI::setDevice(ECS02 *device)
{
    m_device=device;

    connect(m_device, &ECS02::infoUpdated, this, &ECS02UI::updateInfo);
}

void ECS02UI::updateInfo()
{
    bool separated=m_device->isSeparated();

    ui->chuckStateLabel->setText(separated?"Separated":"Contact");
    ui->separateButton->setText(separated?"CONTACT":"SEPARATE");
    ui->moveChuckUpButton   ->setEnabled(separated);
    ui->moveChuckDownButton ->setEnabled(separated);
    ui->moveChuckLeftButton ->setEnabled(separated);
    ui->moveChuckRightButton->setEnabled(separated);
}

void ECS02UI::on_separateButton_clicked()
{
    m_device->separate(!m_device->isSeparated());
}

void ECS02UI::on_moveChuckUpButton_pressed()
{
    m_xSpeed++;
    m_device->moveIncrement(m_xSpeed,0);
}

void ECS02UI::on_moveChuckUpButton_released()
{
    if(ui->moveChuckUpButton->isDown()) return;
    m_xSpeed=0;
}

void ECS02UI::on_moveChuckDownButton_pressed()
{
    m_xSpeed++;
    m_device->moveIncrement(-m_xSpeed,0);
}

void ECS02UI::on_moveChuckDownButton_released()
{
    if(ui->moveChuckDownButton->isDown()) return;
    m_xSpeed=0;
}

void ECS02UI::on_moveChuckLeftButton_pressed()
{
    m_ySpeed++;
    m_device->moveIncrement(0,m_ySpeed);
}

void ECS02UI::on_moveChuckLeftButton_released()
{
    if(ui->moveChuckLeftButton->isDown()) return;
    m_ySpeed=0;
}

void ECS02UI::on_moveChuckRightButton_pressed()
{
    m_ySpeed++;
    m_device->moveIncrement(0,-m_ySpeed);
}

void ECS02UI::on_moveChuckRightButton_released()
{
    if(ui->moveChuckRightButton->isDown()) return;
    m_ySpeed=0;
}

void ECS02UI::on_homeButton_clicked()
{
    m_device->moveHome();
}

void ECS02UI::on_loadButton_clicked()
{
    m_device->moveLoad();
}

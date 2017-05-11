#include "ECS02UI.h"
#include "ui_ECS02UI.h"

#include <QDebug>

ECS02UI::ECS02UI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ECS02UI)
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

void ECS02UI::on_moveChuckUpButton_clicked()
{
    m_device->moveUp();
}

void ECS02UI::on_moveChuckDownButton_clicked()
{
    m_device->moveDown();
}
void ECS02UI::on_moveChuckLeftButton_clicked()
{
    m_device->moveLeft();
}
void ECS02UI::on_moveChuckRightButton_clicked()
{
    m_device->moveRight();
}

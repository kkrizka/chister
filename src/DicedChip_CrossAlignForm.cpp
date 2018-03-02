#define _USE_MATH_DEFINES

#include "DicedChip_CrossAlignForm.h"
#include "ui_DicedChip_CrossAlignForm.h"

#include <cmath>

#include <QDebug>

DicedChip_CrossAlignForm::DicedChip_CrossAlignForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicedChip_CrossAlignForm)
{
    ui->setupUi(this);
}

DicedChip_CrossAlignForm::~DicedChip_CrossAlignForm()
{
    delete ui;
}

void DicedChip_CrossAlignForm::updateInfo(float angle)
{
    ui->rotationLabel->setText(QString("Rotation: %1 deg").arg(angle*180/M_PI,0,'f',2));
}

void DicedChip_CrossAlignForm::updateTestCrossAngle(float angle)
{
    ui->testRotationLabel->setText(QString("Alt Rotation: %1 deg").arg(angle*180/M_PI,0,'f',2));
}

void DicedChip_CrossAlignForm::on_continuePushButton_clicked()
{
    emit done();
}


void DicedChip_CrossAlignForm::on_testPushButton_clicked()
{
    emit test();
}

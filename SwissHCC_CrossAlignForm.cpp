#define _USE_MATH_DEFINES

#include "SwissHCC_CrossAlignForm.h"
#include "ui_SwissHCC_CrossAlignForm.h"

#include <cmath>

#include <QDebug>

SwissHCC_CrossAlignForm::SwissHCC_CrossAlignForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwissHCC_CrossAlignForm)
{
    ui->setupUi(this);
}

SwissHCC_CrossAlignForm::~SwissHCC_CrossAlignForm()
{
    delete ui;
}

void SwissHCC_CrossAlignForm::updateInfo(float x, float y, float angle)
{
    ui->rotationLabel->setText(QString("Rotation: %1 deg").arg(angle*180/M_PI,0,'f',2));
}

void SwissHCC_CrossAlignForm::updateTestCrossAngle(float angle)
{
    ui->testRotationLabel->setText(QString("Alt Rotation: %1 deg").arg(angle*180/M_PI,0,'f',2));
}

void SwissHCC_CrossAlignForm::on_continuePushButton_clicked()
{
    emit done();
}


void SwissHCC_CrossAlignForm::on_testPushButton_clicked()
{
    emit test();
}

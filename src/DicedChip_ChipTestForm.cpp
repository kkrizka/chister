#include "DicedChip_ChipTestForm.h"
#include "ui_DicedChip_ChipTestForm.h"

#include <QMessageBox>
#include <QDebug>

DicedChip_ChipTestForm::DicedChip_ChipTestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicedChip_ChipTestForm)
{
    ui->setupUi(this);
}

DicedChip_ChipTestForm::~DicedChip_ChipTestForm()
{
    delete ui;
}

void DicedChip_ChipTestForm::setupSlots(uint nX, uint nY)
{
    uint cnt=0;
    for(uint iY=0;iY<nY;iY++)
    {
        for(uint iX=0;iX<nX;iX++)
        {
            QPushButton *button=new QPushButton(this);
            button->setText(QString::number(cnt));
            button->resize(20,20);
            button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            connect(button,&QPushButton::clicked,this,&DicedChip_ChipTestForm::on_slotPushButton_clicked);
            ui->slotsGridLayout->addWidget(button,iY,iX);
            m_slotChecks[button]=slot_t(iX,iY);
            cnt++;
        }
    }
}

void DicedChip_ChipTestForm::on_slotPushButton_clicked()
{
    QPushButton *sourceSlot=dynamic_cast<QPushButton*>(sender());
    emit findChip(m_slotChecks[sourceSlot]);
}

void DicedChip_ChipTestForm::on_redoPushButton_clicked()
{
    emit alignChip();
}

void DicedChip_ChipTestForm::on_confirmPushButton_clicked()
{
    emit testChip();
}

void DicedChip_ChipTestForm::updateChipSlot(const slot_t& slot)
{
    ui->chipFoundLabel->setText("Finding chip...");
    ui->chipSlotLabel->setText(QString("Active Slot: %1 (%2,%3)").arg(slot.first*2+slot.second).arg(slot.first).arg(slot.second));
}

void DicedChip_ChipTestForm::updateChipAlignScore(float chipOffsetScore)
{
    ui->chipFoundLabel->setText(QString("Chip alignment score: %1").arg(chipOffsetScore));
}

void DicedChip_ChipTestForm::on_skipPushButton_clicked()
{
    emit nextChip();
}

void DicedChip_ChipTestForm::updateChipStatus(bool result, const QString& testLog)
{
    if(result)
    {
        QMessageBox msgBox;
        msgBox.setText("Chip test succeeded.");
        msgBox.exec();
        emit nextChip();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Chip test failed.");
        msgBox.setInformativeText("Do you want to procceed?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setDetailedText(testLog);
        int ret=msgBox.exec();
        switch(ret)
        {
        case QMessageBox::Yes:
            emit nextChip();
            break;
        case QMessageBox::No:
        default:
            break;
        }
    }
}


#include "SwissHCC_ChipTestForm.h"
#include "ui_SwissHCC_ChipTestForm.h"

SwissHCC_ChipTestForm::SwissHCC_ChipTestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwissHCC_ChipTestForm)
{
    ui->setupUi(this);
}

SwissHCC_ChipTestForm::~SwissHCC_ChipTestForm()
{
    delete ui;
}

void SwissHCC_ChipTestForm::setupSlots(uint nX, uint nY)
{
    uint cnt=0;
    for(uint iY=0;iY<nY;iY++)
    {
        for(uint iX=0;iX<nX;iX++)
        {
            QPushButton *button=new QPushButton(this);
            button->setText(QString::number(cnt));
            //button->setCheckable(true);
            button->resize(20,20);
            button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            connect(button,&QPushButton::clicked,this,&SwissHCC_ChipTestForm::on_slotPushButton_clicked);
            ui->slotsGridLayout->addWidget(button,iY,iX);
            m_slotChecks[button]=QPoint(iX,iY);
            cnt++;
        }
    }
}

void SwissHCC_ChipTestForm::on_slotPushButton_clicked()
{
    QPushButton *sourceSlot=dynamic_cast<QPushButton*>(sender());
    emit findChip(m_slotChecks[sourceSlot]);
}


void SwissHCC_ChipTestForm::on_confirmPushButton_clicked()
{
    emit testChip();
}

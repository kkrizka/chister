#include "SwissHCC_LoadChipsForm.h"
#include "ui_SwissHCC_LoadChipsForm.h"

SwissHCC_LoadChipsForm::SwissHCC_LoadChipsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwissHCC_LoadChipsForm)
{
    ui->setupUi(this);
}

SwissHCC_LoadChipsForm::~SwissHCC_LoadChipsForm()
{
    delete ui;
}

void SwissHCC_LoadChipsForm::setupSlots(uint nX, uint nY)
{
    uint cnt=0;
    for(uint iY=0;iY<nY;iY++)
    {
        for(uint iX=0;iX<nX;iX++)
        {
            QPushButton *button=new QPushButton(this);
            button->setText(QString::number(cnt));
            button->setCheckable(true);
            button->resize(20,20);
            button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            ui->slotsGridLayout->addWidget(button,iY,iX);
            m_slotChecks[button]=slot_t(iX,iY);
            cnt++;
        }
    }
}

void SwissHCC_LoadChipsForm::on_donePushButton_clicked()
{
    QList<slot_t> validslots;
    for(QPushButton *check : m_slotChecks.keys())
       if(check->isChecked()) validslots.append(m_slotChecks[check]);

    emit done(validslots);
}

void SwissHCC_LoadChipsForm::on_selectPushButton_clicked()
{
    for(QPushButton *check : m_slotChecks.keys())
        check->setChecked(true);
}

void SwissHCC_LoadChipsForm::on_deselectPushButton_clicked()
{
    for(QPushButton *check : m_slotChecks.keys())
        check->setChecked(false);
}

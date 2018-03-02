#include "DicedChip_SummaryForm.h"
#include "ui_DicedChip_SummaryForm.h"

DicedChip_SummaryForm::DicedChip_SummaryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicedChip_SummaryForm)
{
    ui->setupUi(this);
}

DicedChip_SummaryForm::~DicedChip_SummaryForm()
{
    delete ui;
}

void DicedChip_SummaryForm::setupSlots(uint nX, uint nY)
{
    uint cnt=0;
    for(uint iY=0;iY<nY;iY++)
    {
        for(uint iX=0;iX<nX;iX++)
        {
            QLabel *label=new QLabel(this);
            label->setText(QString::number(cnt));
            label->resize(20,20);
            label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            ui->slotsGridLayout->addWidget(label,iY,iX);
            m_slotLabels[slot_t(iX,iY)]=label;
            cnt++;
        }
    }
}

void DicedChip_SummaryForm::setResults(const QMap<slot_t, bool>& results)
{
    for(const auto& slot : results.keys())
    {
        QString colcode=(results[slot]?"#00FF00":"#FF0000");
        m_slotLabels[slot]->setStyleSheet("QLabel { background-color:"+colcode+"; }");
    }
}

void DicedChip_SummaryForm::on_closePushButton_clicked()
{
    emit done();
}

#include "SwissHCC_SummaryForm.h"
#include "ui_SwissHCC_SummaryForm.h"

SwissHCC_SummaryForm::SwissHCC_SummaryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwissHCC_SummaryForm)
{
    ui->setupUi(this);
}

SwissHCC_SummaryForm::~SwissHCC_SummaryForm()
{
    delete ui;
}

void SwissHCC_SummaryForm::setupSlots(uint nX, uint nY)
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

void SwissHCC_SummaryForm::setResults(const QMap<slot_t, bool>& results)
{
    for(const auto& slot : results.keys())
    {
        QString colcode=(results[slot]?"#00FF00":"#FF0000");
        m_slotLabels[slot]->setStyleSheet("QLabel { background-color :"+colcode+"; }");
    }
}

void SwissHCC_SummaryForm::on_closePushButton_clicked()
{
    emit done();
}

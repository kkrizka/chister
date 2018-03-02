#include "DicedChip_InfoForm.h"
#include "ui_DicedChip_InfoForm.h"

DicedChip_InfoForm::DicedChip_InfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicedChip_InfoForm)
{
    ui->setupUi(this);
}

DicedChip_InfoForm::~DicedChip_InfoForm()
{
    delete ui;
}

void DicedChip_InfoForm::setText(const QString& text)
{
    ui->infoLabel->setText(text);
}

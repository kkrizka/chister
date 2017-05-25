#include "SwissHCC_InfoForm.h"
#include "ui_SwissHCC_InfoForm.h"

SwissHCC_InfoForm::SwissHCC_InfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwissHCC_InfoForm)
{
    ui->setupUi(this);
}

SwissHCC_InfoForm::~SwissHCC_InfoForm()
{
    delete ui;
}

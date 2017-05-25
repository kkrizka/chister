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

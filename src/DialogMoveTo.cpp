#include "DialogMoveTo.h"
#include "ui_DialogMoveTo.h"

#include <QDoubleValidator>

DialogMoveTo::DialogMoveTo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMoveTo)
{
    ui->setupUi(this);

    ui->lineEditX->setValidator( new QDoubleValidator(0, 100, 2, this) );
    ui->lineEditY->setValidator( new QDoubleValidator(0, 100, 2, this) );
}

DialogMoveTo::~DialogMoveTo()
{
  delete ui;
}

void DialogMoveTo::setX(double x)
{
  ui->lineEditX->setText(QString::number(x,'g',2));
}

void DialogMoveTo::setY(double y)
{
  ui->lineEditY->setText(QString::number(y,'g',2));
}

double DialogMoveTo::x() const
{
  return ui->lineEditX->text().toDouble();
}

double DialogMoveTo::y() const
{
  return ui->lineEditY->text().toDouble();
}

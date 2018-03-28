#include "DialogSetIncrement.h"
#include "ui_DialogSetIncrement.h"

#include <QDoubleValidator>

DialogSetIncrement::DialogSetIncrement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetIncrement)
{
    ui->setupUi(this);

    ui->lineEditX->setValidator( new QDoubleValidator(0, 100, 3, this) );
    ui->lineEditY->setValidator( new QDoubleValidator(0, 100, 3, this) );
}

DialogSetIncrement::~DialogSetIncrement()
{
  delete ui;
}

void DialogSetIncrement::setX(double x)
{
  ui->lineEditX->setText(QString::number(x,'g',3));
}

void DialogSetIncrement::setY(double y)
{
  ui->lineEditY->setText(QString::number(y,'g',3));
}

double DialogSetIncrement::x() const
{
  return ui->lineEditX->text().toDouble();
}

double DialogSetIncrement::y() const
{
  return ui->lineEditY->text().toDouble();
}

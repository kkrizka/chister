#include "ImageScan_CalibrateForm.h"
#include "ui_ImageScan_CalibrateForm.h"

#include <QFileDialog>
#include <QDebug>

ImageScan_CalibrateForm::ImageScan_CalibrateForm(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::ImageScan_CalibrateForm)
{
  ui->setupUi(this);
}

ImageScan_CalibrateForm::~ImageScan_CalibrateForm()
{
  delete ui;
}

void ImageScan_CalibrateForm::updateCalibrate(double scale)
{
  ui->stepResultLabel->setText(ui->stepResultLabel->text()+QString::number(scale)+"\n");
}

void ImageScan_CalibrateForm::doneCalibrate(double scale)
{
  ui->stepResultLabel->setText(ui->stepResultLabel->text()+"Final: "+QString::number(scale)+" mm/pix");
}

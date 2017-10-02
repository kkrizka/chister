#include "ImageScan_CalibrateForm.h"
#include "ui_ImageScan_CalibrateForm.h"

#include <QFileDialog>

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

void ImageScan_CalibrateForm::on_donePushButton_clicked()
{
  emit done();
}

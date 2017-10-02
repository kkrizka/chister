#include "ImageScan_StartForm.h"
#include "ui_ImageScan_StartForm.h"

#include <QFileDialog>

ImageScan_StartForm::ImageScan_StartForm(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::ImageScan_StartForm)
{
  ui->setupUi(this);
}

ImageScan_StartForm::~ImageScan_StartForm()
{
  delete ui;
}

void ImageScan_StartForm::on_donePushButton_clicked()
{
  emit done();
}

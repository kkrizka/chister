#include "ImageScan_ScanForm.h"
#include "ui_ImageScan_ScanForm.h"

#include <QFileDialog>
#include <QDebug>

ImageScan_ScanForm::ImageScan_ScanForm(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::ImageScan_ScanForm)
{
  ui->setupUi(this);
}

ImageScan_ScanForm::~ImageScan_ScanForm()
{
  delete ui;
}

void ImageScan_ScanForm::updateScan(const QImage& img)
{
  ui->scanImageLabel->setPixmap(QPixmap::fromImage(img.scaled(200,200)));
}

void ImageScan_ScanForm::doneScan(uint chips)
{
  ui->messageLabel->setText("Found "+QString::number(chips)+" chips");
}

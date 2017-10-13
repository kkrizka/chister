#include "ImageScan_PictureForm.h"
#include "ui_ImageScan_PictureForm.h"

#include <QDebug>

ImageScan_PictureForm::ImageScan_PictureForm(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::ImageScan_PictureForm)
{
  ui->setupUi(this);
}

ImageScan_PictureForm::~ImageScan_PictureForm()
{
  delete ui;
}

void ImageScan_PictureForm::setChips(const QList<QPoint>& chips)
{
  ui->chipComboBox->clear();
  for(uint i=0;i<chips.size();i++)
    ui->chipComboBox->addItem("Chip "+QString::number(i),chips[i]);
}

void ImageScan_PictureForm::on_chipComboBox_currentIndexChanged(int index)
{
  if(index==-1) return;

  QVariant data=ui->chipComboBox->itemData(index);
  emit moveTo(data.toPoint());
}

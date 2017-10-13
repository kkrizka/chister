#ifndef IMAGESCAN_PICTUREFORM_H
#define IMAGESCAN_PICTUREFORM_H

#include <QWidget>

namespace Ui {
  class ImageScan_PictureForm;
}

class ImageScan_PictureForm : public QWidget
{
  Q_OBJECT

public:
  explicit ImageScan_PictureForm(QWidget *parent = 0);
  ~ImageScan_PictureForm();

  void setChips(const QList<QPoint>& chips);

signals:
  void done();
  void moveTo(const QPoint& pos);

public slots:
  void on_chipComboBox_currentIndexChanged(int index);

private:
  Ui::ImageScan_PictureForm *ui;
};

#endif // IMAGESCAN_PICTUREFORM_H

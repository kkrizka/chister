#ifndef IMAGESCAN_STARTFORM_H
#define IMAGESCAN_STARTFORM_H

#include <QWidget>

namespace Ui {
  class ImageScan_StartForm;
}

class ImageScan_StartForm : public QWidget
{
  Q_OBJECT

public:
  explicit ImageScan_StartForm(QWidget *parent = 0);
  ~ImageScan_StartForm();

private slots:
  void on_donePushButton_clicked();

signals:
  void done();

private:
  Ui::ImageScan_StartForm *ui;
};

#endif // IMAGESCAN_STARTFORM_H

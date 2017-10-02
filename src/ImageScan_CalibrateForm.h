#ifndef IMAGESCAN_CALIBRATEFORM_H
#define IMAGESCAN_CALIBRATEFORM_H

#include <QWidget>

namespace Ui {
  class ImageScan_CalibrateForm;
}

class ImageScan_CalibrateForm : public QWidget
{
  Q_OBJECT

public:
  explicit ImageScan_CalibrateForm(QWidget *parent = 0);
  ~ImageScan_CalibrateForm();

signals:
  void done();

public slots:
  void updateCalibrate(double scale);
  void doneCalibrate(double scale);

private:
  Ui::ImageScan_CalibrateForm *ui;
};

#endif // IMAGESCAN_CALIBRATEFORM_H

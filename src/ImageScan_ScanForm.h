#ifndef IMAGESCAN_SCANFORM_H
#define IMAGESCAN_SCANFORM_H

#include <QWidget>

namespace Ui {
  class ImageScan_ScanForm;
}

class ImageScan_ScanForm : public QWidget
{
  Q_OBJECT

public:
  explicit ImageScan_ScanForm(QWidget *parent = 0);
  ~ImageScan_ScanForm();

signals:
  void done();

public slots:
  void updateScan(const QImage& img);
  void doneScan(const QList<QPoint>& chips);

private:
  Ui::ImageScan_ScanForm *ui;
};

#endif // IMAGESCAN_SCANFORM_H

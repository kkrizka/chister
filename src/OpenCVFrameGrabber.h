#ifndef FRAMEGRABBER_H
#define FRAMEGRABBER_H

#include <QImage>
#include <QObject>
#include <QTimer>
#include <QWaitCondition>

#include <opencv2/opencv.hpp>


class FrameGrabber : public QObject
{
  Q_OBJECT
public:
  explicit FrameGrabber(QObject *parent = 0);

  const QImage& getImage(bool update=false);

signals:
  void finished();
  void newImage(const QImage& img);

public slots:
  void startAcquisition();
  void stopAcquisition();

private slots:
  void updateCamera();

private:
  cv::VideoCapture *m_capture;
  QImage m_img;

  // GUI
  QTimer *m_cameraTimer;
  QWaitCondition m_waitForNew;
};

#endif // FRAMEGRABBER_H

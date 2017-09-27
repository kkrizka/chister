#include "OpenCVFrameGrabber.h"

#include <QMutex>
#include <QDebug>
#include <QPainter>

FrameGrabber::FrameGrabber(QObject *parent) : QObject(parent)
{
  //Initialize interface and session
  m_capture=new cv::VideoCapture(0);
  if(!m_capture->isOpened())
    qInfo() << "Error opening capture card";

  // Prepare acquisition timer
  m_cameraTimer=new QTimer(this);
  connect(m_cameraTimer, &QTimer::timeout, this, &FrameGrabber::updateCamera);
}

const QImage& FrameGrabber::getImage(bool update)
{
  if(update)
    {
      QMutex waitMutex;
      waitMutex.lock();
      m_waitForNew.wait(&waitMutex);
      waitMutex.unlock();
    }
  return m_img;
}

void FrameGrabber::startAcquisition()
{
  // Start acquitision timer
  m_cameraTimer->start(100);
}

void FrameGrabber::stopAcquisition()
{
  // Start acquitision timer
  m_cameraTimer->stop();
  emit finished();
}

void FrameGrabber::updateCamera()
{
  cv::Mat frame;

  (*m_capture) >> frame;
  QImage::Format format;
  switch(frame.type())
    {
    case CV_8UC3:
      format=QImage::Format_RGB888;
      break;
    default:
      qInfo() << "Unknown camera format" << frame.type();
      format=QImage::Format_Grayscale8;
    }
  m_img=QImage(frame.data, frame.cols, frame.rows, format);

  m_waitForNew.wakeAll();
  emit newImage(m_img);
}

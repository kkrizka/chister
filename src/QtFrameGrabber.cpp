#include "QtFrameGrabber.h"

#include <QMutex>
#include <QDebug>
#include <QPainter>
#include <QCameraInfo>
#include <QCameraViewfinder>

QtFrameGrabber::QtFrameGrabber(QObject *parent)
  : FrameGrabber(parent)
{
  qInfo() << "QtFrameGrabber";
  for(const QCameraInfo &cameraInfo : QCameraInfo::availableCameras())
    qInfo() << cameraInfo.deviceName() << cameraInfo.description();

  /*m_camera=new QCamera(QCameraInfo::defaultCamera());
  qInfo() << m_camera->isCaptureModeSupported(QCamera::CaptureStillImage);
  m_imageCapture=new QCameraImageCapture(m_camera);
  m_camera->setCaptureMode(QCamera::CaptureStillImage);
  QCameraViewfinder *viewfinder=new QCameraViewfinder();
  m_camera->setViewfinder(viewfinder);
  viewfinder->show();*/
}

QtFrameGrabber::~QtFrameGrabber()
{
  //delete m_imageCapture;
  //delete m_camera;
}

void QtFrameGrabber::startAcquisition()
{
    qInfo() << "start";
    //m_camera->start();
    FrameGrabber::startAcquisition();
}

void QtFrameGrabber::stopAcquisition()
{
    FrameGrabber::stopAcquisition();
    //m_camera->stop();
}

void QtFrameGrabber::grabImage()
{
    //m_imageCapture->capture("test.png");
}

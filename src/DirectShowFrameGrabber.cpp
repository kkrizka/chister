#include "DirectShowFrameGrabber.h"

#include <QMutex>
#include <QDebug>
#include <QPainter>
#include <QCameraInfo>
#include <QCameraViewfinder>

DirectShowFrameGrabber::DirectShowFrameGrabber(QObject *parent)
  : FrameGrabber(parent)
{
  qInfo() << "DirectShowFrameGrabber";
  for(const QCameraInfo &cameraInfo : QCameraInfo::availableCameras())
    qInfo() << cameraInfo.deviceName() << cameraInfo.description();

  m_camera=new QCamera(QCameraInfo::defaultCamera());
  qInfo() << m_camera->isCaptureModeSupported(QCamera::CaptureStillImage);
  m_imageCapture=new QCameraImageCapture(m_camera);
  m_camera->setCaptureMode(QCamera::CaptureStillImage);
  //m_camera->searchAndLock();

  //on shutter button pressed

  //on shutter button released
  //m_camera->unlock();
}

DirectShowFrameGrabber::~DirectShowFrameGrabber()
{
  delete m_imageCapture;
  delete m_camera;
}

void DirectShowFrameGrabber::startAcquisition()
{
    qInfo() << "start";
    m_camera->start();
    FrameGrabber::startAcquisition();
}

void DirectShowFrameGrabber::stopAcquisition()
{
    FrameGrabber::stopAcquisition();
    m_camera->stop();
}

void DirectShowFrameGrabber::grabImage()
{
    m_imageCapture->capture("test.png");
}

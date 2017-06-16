#include "FrameGrabber.h"

#include <QMutex>
#include <QDebug>
#include <QPainter>

FrameGrabber::FrameGrabber(QObject *parent) : QObject(parent)
{
  m_img=QImage(640, 480, QImage::Format_Grayscale8);
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
  grabImage();

    m_waitForNew.wakeAll();
    emit newImage(m_img);
}

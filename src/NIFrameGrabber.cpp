#include "NIFrameGrabber.h"

#include <QMutex>
#include <QDebug>
#include <QPainter>

FrameGrabber::FrameGrabber(QObject *parent) : QObject(parent)
{
    //Initialize interface and session
    imgInterfaceOpen ("img0", &m_iid);
    imgSessionOpen (m_iid, &m_sid);

    imgGetAttribute (m_sid, IMG_ATTR_ROI_WIDTH, &m_acqWinWidth);
    imgGetAttribute (m_sid, IMG_ATTR_ROI_HEIGHT, &m_acqWinHeight);
    imgGetAttribute (m_sid, IMG_ATTR_BYTESPERPIXEL, &m_bytesPerPixel);

    m_img=QImage(m_acqWinWidth, m_acqWinHeight, QImage::Format_Grayscale8);

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
    imgSnap (m_sid, (void **)&m_ImaqBuffer);
    m_img=QImage(m_ImaqBuffer, m_acqWinWidth, m_acqWinHeight, QImage::Format_Grayscale8);

    m_waitForNew.wakeAll();
    emit newImage(m_img);
}

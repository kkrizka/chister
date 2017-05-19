#include "FrameGrabber.h"

#include <QImage>

FrameGrabber::FrameGrabber(QObject *parent) : QObject(parent)
{
    //Initialize interface and session
    imgInterfaceOpen ("img0", &m_iid);
    imgSessionOpen (m_iid, &m_sid);

    imgGetAttribute (m_sid, IMG_ATTR_ROI_WIDTH, &m_acqWinWidth);
    imgGetAttribute (m_sid, IMG_ATTR_ROI_HEIGHT, &m_acqWinHeight);
    imgGetAttribute (m_sid, IMG_ATTR_BYTESPERPIXEL, &m_bytesPerPixel);

    // Prepare acquisition timer
    m_cameraTimer=new QTimer(this);
    connect(m_cameraTimer, &QTimer::timeout, this, &FrameGrabber::updateCamera);
}

void FrameGrabber::startAcquisition()
{
    // Start acquitision timer
    m_cameraTimer->start(100);
}

void FrameGrabber::updateCamera()
{
    imgSnap (m_sid, (void **)&m_ImaqBuffer);
    QImage img(m_ImaqBuffer, m_acqWinWidth, m_acqWinHeight, QImage::Format_Grayscale8);
    emit newImage(img);
}

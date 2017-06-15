#include "DummyFrameGrabber.h"

#include <QMutex>
#include <QDebug>
#include <QPainter>

DummyFrameGrabber::DummyFrameGrabber(QObject *parent)
  : FrameGrabber(parent),
    m_x(0),m_y(0)
{
  m_imgsrc=QImage("/home/kkrizka/Projects/chister/data/fullimg.png");
  
  m_img=QImage(640, 480, QImage::Format_Grayscale8);
}

void DummyFrameGrabber::grabImage()
{
  m_img=m_imgsrc.copy(0,0,m_img.width(),m_img.height());
}

#ifndef DUMMYFRAMEGRABBER_H
#define DUMMYFRAMEGRABBER_H

#include <QImage>
#include <QObject>
#include <QTimer>
#include <QWaitCondition>

#include "FrameGrabber.h"

class DummyFrameGrabber : public FrameGrabber
{
    Q_OBJECT
public:
    explicit DummyFrameGrabber(QObject *parent = 0);

protected:
  void grabImage();

private:
  QImage m_imgsrc;
  float m_x, m_y;
};

#endif // DUMMYFRAMEGRABBER_H

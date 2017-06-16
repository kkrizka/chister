#ifndef QTFRAMEGRABBER_H
#define QTFRAMEGRABBER_H

#include <QImage>
#include <QObject>
#include <QTimer>
#include <QWaitCondition>
#include <QCamera>
#include <QCameraImageCapture>

#include "FrameGrabber.h"

class QtFrameGrabber : public FrameGrabber
{
    Q_OBJECT
public:
    explicit QtFrameGrabber(QObject *parent = 0);
    virtual ~QtFrameGrabber();

public slots:
  void startAcquisition();
  void stopAcquisition();

protected:
  void grabImage();

private:
  QCamera *m_camera;
  QCameraImageCapture *m_imageCapture;
};

#endif // QTFRAMEGRABBER_H

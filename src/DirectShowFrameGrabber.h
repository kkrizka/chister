#ifndef DIRECTSHOWFRAMEGRABBER_H
#define DIRECTSHOWFRAMEGRABBER_H

#include <QImage>
#include <QObject>
#include <QTimer>
#include <QWaitCondition>
#include <QCamera>
#include <QCameraImageCapture>

#include "FrameGrabber.h"

class DirectShowFrameGrabber : public FrameGrabber
{
    Q_OBJECT
public:
    explicit DirectShowFrameGrabber(QObject *parent = 0);
    virtual ~DirectShowFrameGrabber();

public slots:
  void startAcquisition();
  void stopAcquisition();

protected:
  void grabImage();

private:
  QCamera *m_camera;
  QCameraImageCapture *m_imageCapture;
};

#endif // DIRECTSHOWFRAMEGRABBER_H

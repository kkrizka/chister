#ifndef FRAMEGRABBER_H
#define FRAMEGRABBER_H

#include <QImage>
#include <QObject>
#include <QTimer>
#include <QWaitCondition>

/**
 * Class representing a camera designed to run image acquisition inside a separate thread.
 *
 * All implementations need to implement the grabImage function. It needs to acquire a new
 * image  and store it as a QImage inside the protected m_img variable.
 *
 * Consumers of the camera can obtain the image either through the newImage signal, emitted 
 * everytime a new image is obtained, or via the getImage function. The getImage function 
 * returns the last (or next via option) image acquired.
 *
 * Image acquistion is started by calling startAcquisition and stopped via stopAcquisition.
 */
class FrameGrabber : public QObject
{
    Q_OBJECT
public:
    explicit FrameGrabber(QObject *parent = 0);

    const QImage& getImage(bool update=false);

signals:
    void finished();
    void newImage(const QImage& img);

public slots:
    virtual void startAcquisition();
    virtual void stopAcquisition();

protected:
  QImage m_img;
  virtual void grabImage() =0;
			  
private slots:
    void updateCamera();

private:

  // GUI
  QTimer *m_cameraTimer;
  QWaitCondition m_waitForNew;
};

#endif // FRAMEGRABBER_H

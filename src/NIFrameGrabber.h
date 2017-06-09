#ifndef FRAMEGRABBER_H
#define FRAMEGRABBER_H

#include <QImage>
#include <QObject>
#include <QTimer>
#include <QWaitCondition>

#include <niimaq.h>


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
    void startAcquisition();
    void stopAcquisition();

private slots:
    void updateCamera();

private:
    // NI-IMAQ
    INTERFACE_ID m_iid;
    SESSION_ID m_sid;
    uchar* m_ImaqBuffer=NULL; //NI-IMAQ image

    Int32 m_acqWinWidth, m_acqWinHeight, m_bytesPerPixel;
    QImage m_img;

    // GUI
    QTimer *m_cameraTimer;
    QWaitCondition m_waitForNew;

};

#endif // FRAMEGRABBER_H

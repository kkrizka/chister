#ifndef FRAMEGRABBER_H
#define FRAMEGRABBER_H

#include <QImage>
#include <QObject>
#include <QTimer>

#include <niimaq.h>


class FrameGrabber : public QObject
{
    Q_OBJECT
public:
    explicit FrameGrabber(QObject *parent = 0);

    const QImage& getImage(bool update=false);

    void startAcquisition();

signals:
    void newImage(const QImage& img);

public slots:

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
};

#endif // FRAMEGRABBER_H
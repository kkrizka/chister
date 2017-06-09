#include "FrameGrabber.h"

#include <QMutex>
#include <QDebug>
#include <QPainter>

FrameGrabber::FrameGrabber(QObject *parent) : QObject(parent)
{ }

const QImage& FrameGrabber::getImage(bool update)
{ }

void FrameGrabber::startAcquisition()
{ }

void FrameGrabber::stopAcquisition()
{ }

void FrameGrabber::updateCamera()
{ }

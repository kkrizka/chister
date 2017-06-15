#include "DummyStage.h"

#include <QMutex>
#include <QDebug>
#include <QPainter>

DummyStage::DummyStage(DummyFrameGrabber *fg, QObject *parent)
  : Stage(parent),
    m_fg(fg),
    m_x(0),m_y(0)
{ }

void DummyStage::updateInfo()
{ emit infoUpdated(); }

void DummyStage::separate(bool separate)
{ emit separated(separate); }

void DummyStage::moveHome()
{ }

void DummyStage::moveLoad()
{ }

void DummyStage::moveAbsolute(double x, double y)
{ }

void DummyStage::moveIncrement(int x, int y)
{ }

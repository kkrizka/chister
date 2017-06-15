#ifndef DUMMYSTAGE_H
#define DUMMYSTAGE_H

#include <QImage>
#include <QObject>
#include <QTimer>
#include <QWaitCondition>

#include "DummyFrameGrabber.h"
#include "Stage.h"

class DummyStage : public Stage
{
    Q_OBJECT
public:
    explicit DummyStage(DummyFrameGrabber *fg, QObject *parent = 0);

public slots:
  void updateInfo();
  void separate(bool separate);
  void moveHome();
  void moveLoad();
  void moveAbsolute(double x, double y);
  void moveIncrement(int x, int y);

private:
  DummyFrameGrabber *m_fg;
  float m_x, m_y;
};

#endif // DUMMYSTAGE_H

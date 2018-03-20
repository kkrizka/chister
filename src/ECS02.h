#ifndef ECS02_H
#define ECS02_H

#include "SerialDevice.h"

class ECS02 : public SerialDevice
{
  Q_OBJECT
public:
  explicit ECS02(QObject *parent=0);

  void openConnection();

  //
  // State information
  bool isSeparated() const;
  bool isMetric() const;
  double getX() const;
  double getY() const;
  double getIncrementX() const;
  double getIncrementY() const;

  void waitForReady();

public slots:
  //
  // Query commands
  //! Quary all of the probe station information
  void updateInfo();
  //! Query the probe station state
  void updateState();

  //
  // Move commands
  void separate(bool separate);

  void moveHome();
  void moveLoad();

  void moveUp();
  void moveDown();
  void moveLeft();
  void moveRight();

  void moveAbsolute(double x, double y);
  void moveIncrement(int x, int y);

signals:
  void infoUpdated();
  void separated(bool separated);

protected:
  virtual void interpretData(const QByteArray& data);

private:
  bool m_isReady;
  bool m_isSeparated;
  bool m_isMetric;
  double m_X, m_Y;
  double m_incX, m_incY;
};

#endif // ECS02_H

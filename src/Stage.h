#ifndef STAGE_H
#define STAGE_H

#include "SerialDevice.h"

/**
 * An abstract XYZ stage that can be controller programitically. 
 *
 * Several abstract functions need to be implemented to translate the movement
 * requests into the protocol of the stage. All functions are expected to be
 * asynchronous. Please see their description for more details.
 *
 * The Stage class does not contain any protections for movement while contact
 * with the chip is made.
 */
class Stage : public QObject
{
    Q_OBJECT
public:
  explicit Stage(QObject *parent=0);

  /**
   * \brief Called when the connection with the stage controller should be created. 
   *
   * The default implementation requests an information update via updateInfo();
   */
  virtual void openConnection();

  //
  // State information
  bool isSeparated() const;
  bool isMetric() const;
  double getX() const;
  double getY() const;
  double getIncrementX() const;
  double getIncrementY() const;

public slots:
  //
  // Query commands

  /**
   * \brief Query the stage for its status. 
   *
   * The values should be set to the following protected variables
   * * m_X, m_Y - absolute XY coordinates
   * * m_incX, m_incY - size of XY increments for relative motion
   * * m_isMetric - whether the position-related units are mm (true) or imperial (false)
   * * m_isSeparated - whether the stage is currently separated (true) or contact with chip is made (false)
   *
   * After information is updated, the infoUpdated signal should be emitted.
   */
  virtual void updateInfo() =0;

  //
  // Move commands
  /** 
   * \brief Separate the stage from contact
   *
   * This is the z-axis motion. The only two states allowed are making contact with the
   * chip or being separated.
   *
   * Implementation must emit separated signal when state is updated.
   *
   * \param separate separate the stage on true, make contact on false
   */
  virtual void separate(bool separate) =0;

  /**
   * \brief Move the stage into "home" position.
   *
   * The home position is with the stage under the microscope. The position is arbitrary, but 
   * ideally the microscope would be looking at a loaded chip.
   */
  virtual void moveHome() =0;

  /**
   * \brief Move the stage into "load" position
   *
   * The load position is assumed when new chips are loaded onto the chuck.
   */
  virtual void moveLoad() =0;

  /**
   * \brief Move the stage to an absolute position.
   *
   * The reference and units are expected to be arbitrary, but self-consistent.
   *
   * \param x the x-coordinatex
   * \param y the y-coordinate
   */
  virtual void moveAbsolute(double x, double y) =0;

  /**
   * \brief Move the stage by quanta of a predefined increment.
   *
   * The size of the increment is predefined elsewhere.
   *
   * \param x the number of x movements to make
   * \param y the number of y movements to make
   */
  void moveIncrement(int x, int y);

  /**
   * \brief Move the stage one unit up (moveIncrement(1,0))
   */
  void moveUp();

  /**
   * \brief Move the stage one unit down (moveIncrement(-1,0))
   */
  void moveDown();

  /**
   * \brief Move the stage one unit left (moveIncrement(0,1))
   */
  void moveLeft();

  /**
   * \brief Move the stage one unit right (moveIncrement(0,-1))
   */
  void moveRight();

signals:
  void infoUpdated();
  void separated(bool separated);

protected:
  bool m_isSeparated;
  bool m_isMetric;
  double m_X, m_Y;
  double m_incX, m_incY;
};

#endif // STAGE_H

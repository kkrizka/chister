#ifndef DICEDCHIPSLOT_H
#define DICEDCHIPSLOT_H

#include <QPair>
#include <QPointF>
#include <QObject>

/**
 * Represents a chip slot
 */
class DicedChipSlot : public QObject
{
public:
  enum Status {Empty, Untested, Pass, Fail};

  //! Initialize the diced chip with an invalid slot
  DicedChipSlot(QObject *parent=0);

  //! Initialize the diced chip with a valid slot
  DicedChipSlot(QPair<uint,uint> slot, QObject *parent=0);
  DicedChipSlot(uint x, uint y, QObject *parent=0);

  //! Determine if the slot is valid
  bool isValid() const;

  // getter/setters
  QPair<uint,uint> slot() const;

  QPointF position() const;
  void setPosition(const QPointF& position);

  //! Name of the chip
  QString m_name;

  //! Test status
  enum Status m_status = Empty;

private:
  //! Whether the slot is valid
  bool m_valid;
  //! Position in the slot grid
  QPair<uint,uint> m_slot;
  //! Absolute stage position that aligns the chip with the needles
  QPointF m_position;
};

#endif // DICEDCHIPSLOT_H

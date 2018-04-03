#include "DicedChipSlot.h"

DicedChipSlot::DicedChipSlot(QObject *parent)
  : QObject(parent),
    m_valid(true)
{ }

DicedChipSlot::DicedChipSlot(QPair<uint,uint> slot, QObject *parent)
  : QObject(parent),
    m_valid(true),
    m_slot(slot)
{ }

DicedChipSlot::DicedChipSlot(uint x, uint y, QObject *parent)
  : QObject(parent),
    m_valid(true),
    m_slot(x,y)
{ }

bool DicedChipSlot::isValid() const
{
  return m_valid;
}

QPair<uint,uint> DicedChipSlot::slot() const
{
  return m_slot;
}

void DicedChipSlot::setPosition(const QPointF& position)
{
  m_position=position;
}

QPointF DicedChipSlot::position() const
{
  return m_position;
}

bool DicedChipSlot::operator<(const DicedChipSlot& S2) const
{
  if(m_slot.second<S2.m_slot.second) return true;
  if(m_slot.second>S2.m_slot.second) return false;
  if(m_slot.first <S2.m_slot.first ) return true;
  return false;
}

#include "Stage.h"

#include <QDebug>

Stage::Stage(QObject *parent)
  : QObject(parent)
{ }

void Stage::openConnection()
{
  updateInfo();
}

void Stage::moveUp()
{
    if(!m_isSeparated) return;
    moveIncrement(1,0);
}

void Stage::moveDown()
{
    if(!m_isSeparated) return;
    moveIncrement(-1,0);
}

void Stage::moveLeft()
{
    if(!m_isSeparated) return;
    moveIncrement(0,1);
}

void Stage::moveRight()
{
    if(!m_isSeparated) return;
    moveIncrement(0,-1);
}

bool Stage::isSeparated() const
{ return m_isSeparated; }

bool Stage::isMetric() const
{ return m_isMetric; }

double Stage::getIncrementX() const
{ return m_incX; }

double Stage::getIncrementY() const
{ return m_incY; }

double Stage::getX() const
{ return m_X; }

double Stage::getY() const
{ return m_Y; }

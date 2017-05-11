#include "ECS02.h"

#include <QDebug.h>

ECS02::ECS02(QObject *parent)
    : SerialDevice("COM3", parent)
{ }

void ECS02::openConnection()
{
    SerialDevice::openConnection();

    updateInfo();
}

void ECS02::updateInfo()
{
    QByteArray data=sendCommand("QD", true);
    m_isSeparated=data.right(2).startsWith("S");
    emit infoUpdated();
}

void ECS02::separate(bool separate)
{
    sendCommand(separate?"MD S":"MD C");
    m_isSeparated=separate;
    emit infoUpdated();
}

void ECS02::moveUp()
{ moveIncrement(1,0); }

void ECS02::moveDown()
{ moveIncrement(-1,0); }

void ECS02::moveLeft()
{ moveIncrement(0,1); }

void ECS02::moveRight()
{ moveIncrement(0,-1); }

void ECS02::moveIncrement(int x, int y)
{
    if(!m_isSeparated) return;
    QString command=QString("MN 1 X %1 Y %2").arg(x).arg(y);
    sendCommand(command);
}

bool ECS02::isSeparated()
{ return m_isSeparated; }

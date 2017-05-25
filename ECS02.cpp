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

void ECS02::interpretData(const QByteArray& data)
{
    if(data.startsWith("QD"))
    {
        m_isSeparated=data.right(2).startsWith("S");
        emit infoUpdated();
    }
    else if(data.startsWith("QU"))
    {
        m_isMetric=data.right(2).startsWith("M");
        emit infoUpdated();
    }
    else if(data.startsWith("QN"))
    {
        QStringList info=QString::fromLocal8Bit(data).split(" ");
        m_incX=info[2].toDouble();
        m_incY=info[4].toDouble();
        emit infoUpdated();
    }
    else if(data.startsWith("QC"))
    {
        QStringList info=QString::fromLocal8Bit(data).split(" ");
        m_X=info[9].toDouble();
        m_Y=info[14].toDouble();
        emit infoUpdated();
    }
}

void ECS02::updateInfo()
{
    sendCommand("QD");
    sendCommand("QU");
    sendCommand("QN");
    sendCommand("QC 1");
}

void ECS02::separate(bool separate)
{
    sendCommand(separate?"MD S":"MD C");
    m_isSeparated=separate;
    emit infoUpdated();
}

void ECS02::moveHome()
{
    if(!m_isSeparated) return;
    sendCommand("MH 1");
}

void ECS02::moveLoad()
{
    if(!m_isSeparated) return;
    moveAbsolute(0,70);
}

void ECS02::moveUp()
{ moveIncrement(1,0); }

void ECS02::moveDown()
{ moveIncrement(-1,0); }

void ECS02::moveLeft()
{ moveIncrement(0,1); }

void ECS02::moveRight()
{ moveIncrement(0,-1); }

void ECS02::moveAbsolute(double x, double y)
{
    if(!m_isSeparated) return;
    QString command=QString("MA 1 X %1 Y %2").arg(x).arg(y);
    sendCommand(command);
}

void ECS02::moveIncrement(int x, int y)
{
    if(!m_isSeparated) return;
    QString command=QString("MN 1 X %1 Y %2").arg(x).arg(y);
    sendCommand(command);
}

bool ECS02::isSeparated() const
{ return m_isSeparated; }

bool ECS02::isMetric() const
{ return m_isMetric; }

double ECS02::getIncrementX() const
{ return m_incX; }

double ECS02::getIncrementY() const
{ return m_incY; }

double ECS02::getX() const
{ return m_X; }

double ECS02::getY() const
{ return m_Y; }

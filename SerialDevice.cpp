#include "SerialDevice.h"

#include <QDebug>

SerialDevice::SerialDevice(const QString& port, QObject *parent)
    : QObject(parent), m_port(port), m_blocking(false)
{
    m_serialPort=new QSerialPort(this);

    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialDevice::readData);
}

QString SerialDevice::getPort() const
{ return m_port; }

void SerialDevice::openConnection()
{
    m_serialPort->setPortName(m_port);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    bool result=m_serialPort->open(QIODevice::ReadWrite);
    emit connectionOpened(result);
}

void SerialDevice::closeConnection()
{
    if(m_serialPort->isOpen()) m_serialPort->close();
    emit connectionClosed();
}

void SerialDevice::readData()
{
    if(m_blocking) return;
    QByteArray data=m_serialPort->readAll();
    emit recievedData(data);
}

QByteArray SerialDevice::sendCommand(const QString& command, bool block)
{
    QByteArray commandData=(command+"\r").toLocal8Bit();
    m_serialPort->write(commandData);
    emit sentData(commandData);

    QByteArray data;
    if(block)
    {
        m_blocking=true;
        while(!data.endsWith('\n'))
            if(m_serialPort->waitForReadyRead()) data+=m_serialPort->readAll();
        m_blocking=false;
        emit recievedData(data);
    }
    return data;
}

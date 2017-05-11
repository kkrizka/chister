#include "SerialDevice.h"

SerialDevice::SerialDevice(const QString& port, QObject *parent)
    : QObject(parent), m_port(port)
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
    QByteArray data=m_serialPort->readAll();
    emit recievedData(data);
}

void SerialDevice::sendCommand(const QString& command)
{
    QByteArray commandData=(command+"\r").toLocal8Bit();
    m_serialPort->write(commandData);
    emit sentData(commandData);
}

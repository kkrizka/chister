#include "SerialDevice.h"

#include <QMutex>
#include <QDebug>

SerialDevice::SerialDevice(const QString& port, QObject *parent)
    : QObject(parent), m_port(port), m_ready(true)
{
    m_serialPort=new QSerialPort(this);

    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialDevice::readData);
}

QString SerialDevice::getPort() const
{ return m_port; }

bool SerialDevice::isReady() const
{ return m_ready; }

QByteArray SerialDevice::getLastResponse() const
{ return m_lastResponse; }

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
    interpretData(data);
    emit recievedData(data);
    if(!m_commandQueue.isEmpty())
    {
        QByteArray commandData=m_commandQueue.dequeue();
        m_serialPort->write(commandData);
        emit sentData(commandData);
    }
    else
    {
        m_ready=true;
        m_waitForIdle.wakeAll();
    }
}

void SerialDevice::interpretData(const QByteArray& /*data*/)
{ }

void SerialDevice::sendCommand(const QString& command)
{
    QByteArray commandData=(command+"\r").toLocal8Bit();
    if(!m_ready)
    {
        m_commandQueue.enqueue(commandData);
        return;
    }
    m_serialPort->write(commandData);
    emit sentData(commandData);
    m_ready=false;
}

void SerialDevice::waitForIdle()
{
    //QMutex mutex;
    //mutex.lock();
   // m_waitForIdle.wait(&mutex);
}

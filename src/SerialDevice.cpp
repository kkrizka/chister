#include "SerialDevice.h"

#include <QMutex>
#include <QDebug>

SerialDevice::SerialDevice(const QString& port, QObject *parent)
    : QObject(parent), m_port(port), m_ready(true), m_lineEnd("\n")
{
  m_serialPort=new QSerialPort(this);

  connect(m_serialPort, &QSerialPort::readyRead, this, &SerialDevice::readData);
}

QSerialPort* SerialDevice::getSerialPort() const
{ return m_serialPort; }

QString SerialDevice::getPort() const
{ return m_port; }

bool SerialDevice::isReady() const
{ return m_ready; }

QByteArray SerialDevice::getLastResponse() const
{ return m_lastResponse; }

void SerialDevice::setLineEnd(const QByteArray& lineEnd)
{ m_lineEnd=lineEnd; }

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
  if(m_lastResponse.endsWith(m_lineEnd)) m_lastResponse.clear();
  m_lastResponse+=m_serialPort->readAll();
  if(!m_lastResponse.endsWith(m_lineEnd)) return; // Haven't recieved the complete response

  // Interpret data
  int s=0;
  int e=0;
  while ((e = m_lastResponse.indexOf(m_lineEnd, s)) != -1)
    {
      QByteArray line=m_lastResponse.mid(s,e-s)+"\n";
      interpretData(line);
      emit recievedData(line);
      s=e+m_lineEnd.size();
    }

  // Move on to the next command
  m_ready=true;
  sendCommandFromQueue();
  if(m_ready) m_waitForIdle.wakeAll();
}

void SerialDevice::interpretData(const QByteArray& /*data*/)
{ }

void SerialDevice::sendCommand(const QString& command)
{
  QByteArray commandData=(command+"\r").toLocal8Bit();
  m_commandQueueMutex.lock();
  m_commandQueue.enqueue(commandData);
  m_commandQueueMutex.unlock();
  QMetaObject::invokeMethod(this,"sendCommandFromQueue",Qt::AutoConnection);
}

void SerialDevice::sendCommandFromQueue()
{
  if(m_ready)
    {
      m_commandQueueMutex.lock();
      if(!m_commandQueue.isEmpty())
        {
	  QByteArray commandData=m_commandQueue.dequeue();
	  m_serialPort->write(commandData);
	  m_ready=false;
	  emit sentData(commandData);
        }
      m_commandQueueMutex.unlock();
    }

}

void SerialDevice::waitForIdle()
{
  QMutex mutex;
  mutex.lock();
  m_waitForIdle.wait(&mutex);
  mutex.unlock();
}

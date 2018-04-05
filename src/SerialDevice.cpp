#include "SerialDevice.h"

#include <QThread>
#include <QMutex>
#include <QDebug>

SerialDevice::SerialDevice(const QString& port, QObject *parent)
    : QObject(parent), m_port(port), m_ready(true), m_lineEnd("\n")
{
  qInfo() << "start";
  m_serialPort=new QSerialPort(this);
  qInfo() << "end";
}

QSerialPort* SerialDevice::getSerialPort() const
{ return m_serialPort; }

QString SerialDevice::getPort() const
{ return m_port; }

bool SerialDevice::isReady() const
{ return m_ready; }

int SerialDevice::commandQueueSize() 
{
    m_commandQueueMutex.lock();
    int size=m_commandQueue.size();
    m_commandQueueMutex.unlock();
    return size;
}

QByteArray SerialDevice::getLastResponse() const
{ return m_lastResponse; }

void SerialDevice::setLineEnd(const QByteArray& lineEnd)
{ m_lineEnd=lineEnd; }

void SerialDevice::openConnection()
{
  qInfo() << "Open connection";
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

void SerialDevice::interpretData(const QByteArray& /*data*/)
{ }

void SerialDevice::sendCommand(const QString& command)
{
  QByteArray commandData=(command+"\n").toLocal8Bit();
  m_commandQueueMutex.lock();
  m_commandQueue.enqueue(commandData);
  m_commandQueueMutex.unlock();
  QMetaObject::invokeMethod(this,"sendCommandFromQueue",Qt::AutoConnection);
}

void SerialDevice::sendCommandFromQueue()
{
  m_commandQueueMutex.lock();
  while(!m_commandQueue.isEmpty())
    {
      QByteArray commandData=m_commandQueue.dequeue();
      m_serialPort->write(commandData);

      emit sentData(commandData);

      m_serialPort->waitForBytesWritten(3000);

      // Read the results
      QByteArray response;
      while(!response.endsWith(m_lineEnd))
	{
	  m_serialPort->waitForReadyRead(100);
	  QByteArray data=m_serialPort->readAll();
      	  response+=data;
      	}

      // Interpret the results
      int s=0;
      int e=0;
      while ((e = response.indexOf(m_lineEnd, s)) != -1)
	{
	  QByteArray line=response.mid(s,e-s)+"\n";
	  interpretData(line);
	  emit recievedData(line);
	  s=e+m_lineEnd.size();
	}
      QThread::msleep(100);
    }
  m_commandQueueMutex.unlock();
  m_waitForIdle.wakeAll();
}

void SerialDevice::waitForIdle()
{
  QMutex mutex;
  mutex.lock();
  m_waitForIdle.wait(&mutex);
  mutex.unlock();
}

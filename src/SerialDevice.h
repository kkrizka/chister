#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>
#include <QSerialPort>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

class SerialDevice : public QObject
{
  Q_OBJECT
public:
  explicit SerialDevice(const QString& port, QObject *parent = 0);

  QSerialPort* getSerialPort() const;
  QString getPort() const;
  bool isOpen() const;
  bool isReady() const;
  QByteArray getLastResponse() const;

  void setLineEnd(const QByteArray& lineEnd);

  bool openConnection();
  void closeConnection();

  void sendCommand(const QString &command);

  void waitForIdle();

signals:
  void connectionOpened(bool success);
  void connectionClosed();
  void recievedData(const QByteArray& data);
  void sentData(const QByteArray& data);

protected slots:
  void readData();

protected:
  virtual void interpretData(const QByteArray& data);

private slots:
  void sendCommandFromQueue();

private:
  QString m_port;
  QSerialPort *m_serialPort;

  bool m_ready;
  QByteArray m_lineEnd;
  QMutex m_commandQueueMutex;
  QQueue<QByteArray> m_commandQueue;
  QByteArray m_lastResponse;

  QWaitCondition m_waitForIdle;
};

#endif // SERIALDEVICE_H

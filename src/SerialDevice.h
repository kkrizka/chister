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
  bool isReady() const;
  int commandQueueSize();
  QByteArray getLastResponse() const;

  void setLineEnd(const QByteArray& lineEnd);

  void sendCommand(const QString &command);

  void waitForIdle();

public slots:
  void openConnection();
  void closeConnection();

signals:
  void connectionOpened(bool success);
  void connectionClosed();
  void recievedData(const QByteArray& data);
  void sentData(const QByteArray& data);

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

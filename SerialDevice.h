#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>
#include <QSerialPort>

class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(const QString& port, QObject *parent = 0);

    QString getPort() const;

    void openConnection();
    void closeConnection();

    void sendCommand(const QString &command);

signals:
    void connectionOpened(bool success);
    void connectionClosed();
    void recievedData(const QByteArray& data);
    void sentData(const QByteArray& data);

protected slots:
    void readData();

private:
    QString m_port;
    QSerialPort *m_serialPort;
};

#endif // SERIALDEVICE_H

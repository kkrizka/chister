#ifndef SERIALCONSOLE_H
#define SERIALCONSOLE_H

#include <QMainWindow>
#include <QSerialPort>

#include "SerialDevice.h"

namespace Ui {
class serialconsole;
}

class serialconsole : public QMainWindow
{
    Q_OBJECT

public:
    explicit serialconsole(QWidget *parent = 0);
    ~serialconsole();

    void setDevice(SerialDevice *device);

public slots:
    void connectionOpened(bool success);
    void connectionClosed();

    void appendData(const QByteArray& data);

private slots:
    void on_sendPushButton_clicked();

    void on_commandLineEdit_returnPressed();

private:
    Ui::serialconsole *ui;

    SerialDevice *m_device;
};

#endif // SERIALCONSOLE_H

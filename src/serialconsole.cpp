#include "serialconsole.h"
#include "ui_serialconsole.h"

#include <QDebug>
#include <QMessageBox>

#include <iostream>

serialconsole::serialconsole(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::serialconsole)
{
    ui->setupUi(this);
}

serialconsole::~serialconsole()
{
    delete ui;
}

void serialconsole::setDevice(SerialDevice *device)
{
    m_device=device;
    ui->serialPortLabel->setText(m_device->getPort());

    // Prepare connections
    connect(device, &SerialDevice::connectionOpened, this, &serialconsole::connectionOpened);
    connect(device, &SerialDevice::connectionClosed, this, &serialconsole::connectionClosed);
    connect(device, &SerialDevice::recievedData, this, &serialconsole::appendData);
    connect(device, &SerialDevice::sentData, this, &serialconsole::appendData);

    // Initialize status
    connectionOpened(device->getSerialPort()->isOpen());
}

void serialconsole::connectionOpened(bool success)
{
    if(success)
        ui->connectedLabel->setText("Connected");
    else
        ui->connectedLabel->setText("Unconnected");
}

void serialconsole::connectionClosed()
{
    ui->connectedLabel->setText("Unconnected");
}

void serialconsole::appendData(const QByteArray& data)
{
    ui->console->insertPlainText(QString(data));
    ui->console->moveCursor(QTextCursor::End);
}

void serialconsole::on_sendPushButton_clicked()
{
    m_device->sendCommand(ui->commandLineEdit->text());
    ui->commandLineEdit->setText("");
}


void serialconsole::on_commandLineEdit_returnPressed()
{
    m_device->sendCommand(ui->commandLineEdit->text());
    ui->commandLineEdit->setText("");
}

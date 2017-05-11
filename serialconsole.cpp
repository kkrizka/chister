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

    m_serialPort=new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::readyRead, this, &serialconsole::readData);

    openConnection();
}

serialconsole::~serialconsole()
{
    delete ui;
}

void serialconsole::openConnection()
{
    m_serialPort->setPortName("COM3");
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if(m_serialPort->open(QIODevice::ReadWrite))
    {
        ui->commandLineEdit->setEnabled(true);
        ui->connectedLabel->setText("Connected");
    }
    else
    {
        QMessageBox::critical(this, "Error", m_serialPort->errorString());
        ui->commandLineEdit->setDisabled(true);
        ui->connectedLabel->setText("Unconnected");
    }
}

void serialconsole::readData()
{
    QByteArray data = m_serialPort->readAll();
    ui->console->insertPlainText(QString(data));
}

void serialconsole::on_sendPushButton_clicked()
{
    m_serialPort->write(ui->commandLineEdit->text().toLocal8Bit());
    m_serialPort->write("\r");
}

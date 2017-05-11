#include "mainwindow.h"
#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextStream>

QT_USE_NAMESPACE

#include <iostream>

#include "serialconsole.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    QTextStream out(stdout);

    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    out << "Total number of ports available: " << serialPortInfos.count() << endl;

    const QString blankString = QObject::tr("N/A");
    QString description;
    QString manufacturer;
    QString serialNumber;

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        out << endl
                << "Port: " << serialPortInfo.portName() << endl
                << "Location: " << serialPortInfo.systemLocation() << endl
                << "Description: " << (!description.isEmpty() ? description : blankString) << endl
                << "Manufacturer: " << (!manufacturer.isEmpty() ? manufacturer : blankString) << endl
                << "Serial number: " << (!serialNumber.isEmpty() ? serialNumber : blankString) << endl
                << "Vendor Identifier: " << (serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16) : blankString) << endl
                << "Product Identifier: " << (serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : blankString) << endl
                << "Busy: " << (serialPortInfo.isBusy() ? "Yes" : "No") << endl;
    }


    serialconsole console;
    console.show();

    return a.exec();
}

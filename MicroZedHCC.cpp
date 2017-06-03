#include "MicroZedHCC.h"

#include <QDebug.h>

MicroZedHCC::MicroZedHCC(QObject *parent)
    : SerialDevice("COM4", parent),
      m_testRunning(true)
{
    setLineEnd("\n\r");
}

void MicroZedHCC::interpretData(const QByteArray& data)
{
    if(data.startsWith("ZYNQ_REPORT: Start HCC Test"))
    {
        qInfo() << "Test started";
        m_testRunning=true;
    }
    else if(data.startsWith("STOP"))
    {
        qInfo() << "Test completed";
        m_testRunning=false;
    }
    else if(data.startsWith("ZYNQ_REPORT: TEST_CHIP_COMPLETED"))
    {
        qInfo() << data;
        emit testDone(data.split(' ').last()=="PASSED\n");
    }

    if(m_testRunning)
        emit testMessage(QString(data));
}


void MicroZedHCC::runBasicTest()
{
    sendCommand("bt");
    //waitForIdle();
}

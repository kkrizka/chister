#include "MicroZedHCC.h"

#include <QDebug>

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
        m_testRunning=true;
        m_lastTestLog.clear();
    }
    else if(data.startsWith("STOP"))
    {
        m_testRunning=false;
    }
    else if(data.startsWith("ZYNQ_REPORT: TEST_CHIP_COMPLETED"))
    {
        emit testDone(data.split(' ').last()=="PASSED\n", m_lastTestLog);
    }

    // Append to log
    if(data.startsWith("ZYNQ_REPORT"))
        m_lastTestLog+=data;

    if(m_testRunning)
        emit testMessage(QString(data));
}


void MicroZedHCC::runBasicTest()
{
    sendCommand("bt");
}

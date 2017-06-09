#ifndef MICROZEDHCC_H
#define MICROZEDHCC_H

#include "SerialDevice.h"

class MicroZedHCC : public SerialDevice
{
    Q_OBJECT
public:
    explicit MicroZedHCC(QObject *parent=0);

    void interpretData(const QByteArray& data);

    void runBasicTest();

signals:
    void testMessage(const QString& msg);
    void testDone(bool status, const QString& testLog);

private:
    bool m_testRunning;
    QString m_lastTestLog;
};

#endif // MICROZEDHCC_H

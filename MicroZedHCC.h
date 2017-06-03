#ifndef MICROZEDHCC_H
#define MICROZEDHCC_H

#include "SerialDevice.h"

class MicroZedHCC : public SerialDevice
{
    Q_OBJECT
public:
    explicit MicroZedHCC(QObject *parent=0);
};

#endif // MICROZEDHCC_H

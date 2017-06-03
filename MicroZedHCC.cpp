#include "MicroZedHCC.h"

#include <QDebug.h>

MicroZedHCC::MicroZedHCC(QObject *parent)
    : SerialDevice("COM4", parent)
{
    setLineEnd("\n\r");
}

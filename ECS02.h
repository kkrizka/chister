#ifndef ECS02_H
#define ECS02_H

#include "SerialDevice.h"

class ECS02 : public SerialDevice
{
    Q_OBJECT
public:
    explicit ECS02(QObject *parent=0);

    void openConnection();

    //
    // State information
    bool isSeparated();

    //
    // Query commands
    void updateInfo();

    //
    // Move commands
    void separate(bool separate);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void moveIncrement(int x, int y);

signals:
    void infoUpdated();
    void separated(bool separated);

private:
    bool m_isSeparated;
};

#endif // ECS02_H

#ifndef SERIALCONSOLE_H
#define SERIALCONSOLE_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class serialconsole;
}

class serialconsole : public QMainWindow
{
    Q_OBJECT

public:
    explicit serialconsole(QWidget *parent = 0);
    ~serialconsole();

protected slots:
    void readData();

private slots:
    void on_sendPushButton_clicked();

private:
    Ui::serialconsole *ui;

    QSerialPort *m_serialPort;

    void openConnection();
    void closeConnection();
};

#endif // SERIALCONSOLE_H

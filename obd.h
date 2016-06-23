#ifndef OBD_H
#define OBD_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QTimer>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothSocket>
#include <vector>

namespace Ui {
class Obd;
}

class Obd : public QMainWindow
{
    Q_OBJECT

public:
    explicit Obd(QWidget *parent = 0);
    ~Obd();
    void read();
    void write();
    void connect();
    void disconnect();
    void serialError(QSerialPort::SerialPortError error);

private:
    Ui::Obd *ui;
    QBluetoothSocket *socket;
    QTimer *timer;
    QString porta;
    bool isConnected;
    Vector<QBluetoothDeviceInfo> devices;
    QBluetoothLocalDevice localdevice;
};

#endif // OBD_H

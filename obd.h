#ifndef OBD_H
#define OBD_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QTimer>

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
    void serialError();

private:
    Ui::Obd *ui;
    QSerialPort *serial;
    QTimer *timer;
    QString porta;
};

#endif // OBD_H

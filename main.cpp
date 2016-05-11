#include "obd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Obd w;
    w.show();

    return a.exec();
}

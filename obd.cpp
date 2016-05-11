#include "obd.h"
#include "ui_obd.h"

Obd::Obd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Obd)
{
    ui->setupUi(this);
}

Obd::~Obd()
{
    delete ui;
}

Obd::connect(){
    serial = new QSerialPort(this);
    timer = new QTimer(this);

    porta = ui->comboBox->currentText();

    qDebug() << "Porta: "<<ui->comboBox->currentText();

    if(!conectado){
        serial->setPortName(ui->comboBox->currentText());
        serial->setBaudRate(QSerialPort::Baud38400);
        serial->setFlowControl(QSerialPort::SoftwareControl);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setParity(QSerialPort::NoParity);
        serial->setDataBits(QSerialPort::Data8);

        if (serial->open(QIODevice::ReadWrite)) {

            serial->write( QByteArray("AT E0\r") );
            serial->write( QByteArray("AT L0\r") );
            serial->write( QByteArray("AT ST 00\r") );
            serial->write( QByteArray("AT SP 0\r") );

            conectado = true;
            qDebug() << "Conectado";

            timer->start(100);

        }
        else {
            QMessageBox::critical(this, tr("Erro de conexão"), serial->errorString());
        }
    }
}

Obd::disconnect(){
    if (serial->isOpen())
        serial->close();
    conectado = false;

    preencherPortasSeriais();

    timer->stop();
}

Obd::serialError(){
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Erro crítico"), serial->errorString());
        fecharSerial();
    }
}

Obd::write(){
    serial->write(QByteArray("0105\r"));
    serial->write(QByteArray("010D\r"));
    serial->write(QByteArray("010C\r"));
    serial->write(QByteArray("012F\r"));
}

Obd::read(){
    QByteArray data;
    data = serial->readAll();

    QString str(data);

    mensagem = str.split(QRegExp("\\W+"), QString::SkipEmptyParts);

    if (mensagem.size() > 0){
        if (mensagem[0] == "41" && mensagem[1] == "05"){
            temperaturaValor = mensagem[2].toInt(&ok,16) - 40;
            ui->lcdNumber->display(temperaturaValor);

            if(temperaturaO)
                temperaturaO->setProperty("value", (double)temperaturaValor/215);

            qDebug() << "Temperatura: "<<temperaturaValor;
        }
    }

    if (mensagem.size() > 0){
        if (mensagem[0] == "41" && mensagem[1] == "0D"){
            velocidadeValor = mensagem[2].toInt(&ok,16);

            if(velocidadeO)
                velocidadeO->setProperty("value", (double)velocidadeValor);

            qDebug() << "Velocidade: "<<velocidadeValor;
        }
    }

    if (mensagem.size() > 0){
        if (mensagem[0] == "41" && mensagem[1] == "0C"){
            rpmValor = (mensagem[2].toInt(&ok,16) * 256 + mensagem[3].toInt(&ok,16))/4;

            if(rpmO)
                rpmO->setProperty("value", (double)rpmValor/1000);

            qDebug() << "RPM: "<<rpmValor;
        }
    }

    if (mensagem.size() > 0){
        if (mensagem[0] == "41" && mensagem[1] == "2F"){
            fuelValor = mensagem[2].toInt(&ok,16) * 100 / 255;

            if(fuelO)
                fuelO->setProperty("value", (double)fuelValor/100);

            qDebug() << "Fuel: "<<fuelValor;
        }
    }

    comando = (comando + 1)%4;
}

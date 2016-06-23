#include "obd.h"
#include "ui_obd.h"

Obd::Obd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Obd)
{
    ui->setupUi(this);

    connected = false;

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    socket->connectToService(remoteService);

    timer = new QTimer(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(read());
            connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

Obd::~Obd()
{
    delete ui;
}

Obd::connect(){
    QString uString = "00001101-0000-1000-8000-00805F9B34FB";
    QBluetoothUuid *uuid = new QBluetoothUuid(&uString);

    QBluetoothDeviceInfo aux, remoteService;

    for(int i = 0; i<devices[i]; i++){
        aux = devices[i];

        if(aux.deviceUuid() == uuid){
            remoteService = aux;
        }
    }

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

    socket->connectToService(remoteService);

    timer = new QTimer(this);

    qDebug() << "Porta: "<<ui->comboBox->currentText();

    if(!isConnected){

        if (socket->connect()) {

            socket->write(QByteArray("AT E0\r"));
            socket->write(QByteArray("AT L0\r"));
            socket->write(QByteArray("AT ST 00\r"));
            socket->write(QByteArray("AT SP 0\r"));

            connected = true;
            qDebug() << "Conectado";

            timer->start(100);

        }
        else {
            QMessageBox::critical(this, tr("Erro de conexão"), serial->errorString());
        }
    }
}

Obd::disconnect(){
    if (socket->isOpen())
        socket->disconnect();
    connected = false;

    devices.clear();
    timer->stop();
}

Obd::serialError(QSerialPort::SerialPortError error){
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Erro crítico"), serial->errorString());
        fecharSerial();
    }
}

Obd::write(){
    socket->write(QByteArray("0105\r"));
}

Obd::read(){
    QByteArray data;
    data = socket->readAll();

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
}

Obd::bluetooth(){
    QString localDeviceName;

    if(localDevice.isValid()){
        localDevice.powerOn();

        localDeviceName = localDevice.name();

        localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

        QList<QBluetoothAddress> remotes;
        remotes = localDevice.connectedDevices();
    }
}


Obd::findBluetooth(){
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    device.clear();

    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(dispositivoEncontrado(QBluetoothDeviceInfo));

            discoveryAgent->start();
}

Obd::dispositivoEncontrado(const QBluetoothDeviceInfo &device){
    device.push_back(device);
}

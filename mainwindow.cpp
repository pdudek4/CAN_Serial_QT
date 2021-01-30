#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CANopenui = new CANOpen(this);
    this->device = new QSerialPort(this);
    setUi();
}

MainWindow::~MainWindow()
{
    delete device;
    delete CANopenui;
    delete ui;
}

void MainWindow::on_pushButtonSearch_clicked()
{
    this->addToLogs("Szukam urządzeń...");
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();

    for(int i = 0; i < devices.count(); i++) {
      this->addToLogs("Znalazłem urządzenie: " + devices.at(i).portName() + " " + devices.at(i).description());
      ui->comboBoxDevices->addItem(devices.at(i).portName() + " " + devices.at(i).description());
    }
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QTime::currentTime().toString("hh:mm:ss");
    ui->textEditLogi->append(currentDateTime + "   " + message);
}

void MainWindow::sendMessageToDevice(QString message)
{
    if(this->device->isOpen() && this->device->isWritable()) {
        this->addToLogs("Wysyłam do STM: " + message);
        this->device->write(message.toStdString().c_str());
    }
    else {
        this->addToLogs("Nie mogę wysłać wiadomości. Port nie jest otwarty!");
    }
}

void MainWindow::setUi()
{
    ui->spinBox_ID->setMaximum(0x7FF);
    ui->spinBox_ID->setPrefix("0x");
    ui->spinBox_ID->setDisplayIntegerBase(16);
    ui->spinBox_ID->clear();

    ui->spinBox_B0->setMaximum(255);
    ui->spinBox_B1->setMaximum(255);
    ui->spinBox_B2->setMaximum(255);
    ui->spinBox_B3->setMaximum(255);
    ui->spinBox_B4->setMaximum(255);
    ui->spinBox_B5->setMaximum(255);
    ui->spinBox_B6->setMaximum(255);
    ui->spinBox_B7->setMaximum(255);
    ui->spinBox_B0->clear();
    ui->spinBox_B1->clear();
    ui->spinBox_B2->clear();
    ui->spinBox_B3->clear();
    ui->spinBox_B4->clear();
    ui->spinBox_B5->clear();
    ui->spinBox_B6->clear();
    ui->spinBox_B7->clear();

    ui->spinBox_F0->setMaximum(0x7FF);
    ui->spinBox_F1->setMaximum(0x7FF);
    ui->spinBox_F2->setMaximum(0x7FF);
    ui->spinBox_F3->setMaximum(0x7FF);
    ui->spinBox_F0->setPrefix("0x");
    ui->spinBox_F1->setPrefix("0x");
    ui->spinBox_F2->setPrefix("0x");
    ui->spinBox_F3->setPrefix("0x");
    ui->spinBox_F0->setDisplayIntegerBase(16);
    ui->spinBox_F1->setDisplayIntegerBase(16);
    ui->spinBox_F2->setDisplayIntegerBase(16);
    ui->spinBox_F3->setDisplayIntegerBase(16);
    ui->spinBox_F0->clear();
    ui->spinBox_F1->clear();
    ui->spinBox_F2->clear();
    ui->spinBox_F3->clear();

    ui->comboBoxPasmo->addItem("125");
    ui->comboBoxPasmo->addItem("250");
    ui->comboBoxPasmo->addItem("500");
    ui->comboBoxPasmo->addItem("1000");

    ui->spinBoxIndex->setMaximum(0xFFFF);
    ui->spinBoxSubIndex->setMaximum(0x20);
    ui->spinBoxODValue->setMaximum(0xFFFFFFFF);
    ui->spinBoxIndex->setPrefix("0x");
    ui->spinBoxSubIndex->setPrefix("0x");
    ui->spinBoxODValue->setPrefix("0x");
    ui->spinBoxIndex->setDisplayIntegerBase(16);
    ui->spinBoxSubIndex->setDisplayIntegerBase(16);
    ui->spinBoxODValue->setDisplayIntegerBase(16);
    ui->spinBoxIndex->clear();
    ui->spinBoxSubIndex->clear();
    ui->spinBoxODValue->clear();
}

void MainWindow::on_pushButtonLink_clicked()
{
    if(ui->comboBoxDevices->count() == 0)
    {
        this->addToLogs("Nie wykryto żadnych urządzeń!");
        return;
    }

    QString comboBoxQString = ui->comboBoxDevices->currentText();
    QStringList portList = comboBoxQString.split(0x20);
    QString portName = portList.first();

    this->device->setPortName(portName);

    // OTWÓRZ I SKONFIGURUJ PORT:
    if(!device->isOpen())
    {

        if(device->open(QSerialPort::ReadWrite))
        {
            this->device->setBaudRate(QSerialPort::Baud115200);
            this->device->setDataBits(QSerialPort::Data8);
            this->device->setParity(QSerialPort::NoParity);
            this->device->setStopBits(QSerialPort::OneStop);
            this->device->setFlowControl(QSerialPort::NoFlowControl);
            // CONNECT:
            connect(this->device, SIGNAL(readyRead()),this, SLOT(readFromPort()));
            this->addToLogs("Otwarto port szeregowy.");
        }
        else
        {
            this->addToLogs("Otwarcie portu szeregowego się nie powiodło!");
        }
    }
    else
    {
        this->addToLogs("Port już jest otwarty!");
        return;
    }
}

void MainWindow::on_pushButtonClose_clicked()
{
    if(this->device->isOpen()) {
      this->device->close();
      this->addToLogs("Zamknięto połączenie.");
    }
    else {
      this->addToLogs("Port nie jest otwarty!");
      return;
    }
}

void MainWindow::readFromPort()
{
    while(this->device->canReadLine()){
      QString line = this->device->readLine();

      QString log;
      ID_rcv = line[1];
      ID_rcv += line[2];
      ID_rcv += line[3];
      log.append("ID: ");
      log.append(ID_rcv);
      log.append(" Dane: ");

//      bool ok;
      QString tmp;
      for(int i=0; i<8; i++){
//        tmp = line[5+i*2];
//        tmp += line[6+i*2];
//        data_rcv[i] = tmp.toInt(&ok, 16);
          log.append(line[5+2*i]);
          log.append(line[6+2*i]);
          log.append(" ");
      }
      this->addToLogs(log);
    }
}

void MainWindow::on_pushButtonFilterSet_clicked()
{
    //wyslij ramke z wartosciami 4 filtrow
    char msg[24];
    sprintf(msg, "FCHG,%03x%03x%03x%03x;;;;;", ui->spinBox_F0->value(),
            ui->spinBox_F1->value(), ui->spinBox_F2->value(), ui->spinBox_F3->value());

    this->sendMessageToDevice(msg);
    qDebug() << "Wyslano: " << msg;
}

void MainWindow::on_pushButtonFilterOff_clicked()
{
    //wyslij ramke z informacja zeby wylaczyc filtrowanie
    this->sendMessageToDevice("FDEA,123456789123;;;;;");
    qDebug() << "Wyslano: " << "FDEA,123456789123;;;;;";
}

void MainWindow::on_pushButtonSendMsg_clicked()
{
    char msg[24];
    sprintf(msg, "M%03x,%02x%02x%02x%02x%02x%02x%02x%02x;", ui->spinBox_ID->value(),
            ui->spinBox_B0->value(), ui->spinBox_B1->value(), ui->spinBox_B2->value(), ui->spinBox_B3->value(),
            ui->spinBox_B4->value(), ui->spinBox_B5->value(), ui->spinBox_B6->value(), ui->spinBox_B7->value());

    this->sendMessageToDevice(msg);
    qDebug() << "Wyslano: " << msg;
}

void MainWindow::on_pushButtonPasmo_clicked()
{
    QString msg = "PASM,";
    QString pasmo_cb = ui->comboBoxPasmo->currentText();
    pasmo_cb.resize(3);
    msg.append(pasmo_cb);
    msg.append("1234567890123;");

    this->sendMessageToDevice(msg);
    qDebug() << "Wyslano: " << msg;
}

void MainWindow::on_pushButton_clicked()
{
   //CANopenui->show();
}

void MainWindow::on_pushButtonSDORead_clicked()
{
    //wyslij komende na can
    // ID=0x601 B0=0x40 B1=indexMSB B2=indexLSB B3=subindex B4-B7=0
    char msg[24];
    uint16_t index;
    uint8_t indexM, indexL;
    index = ui->spinBoxIndex->value();
    indexM = index >> 8;
    indexL = index & 0x0f;
    sprintf(msg, "M601,%02x%02x%02x%02x00000000;",
            0x40, indexM, indexL, ui->spinBoxSubIndex->value());

    //this->sendMessageToDevice(msg);
    //komunikat potwierdzający do logów
    qDebug() << "Wyslano: " << msg;
    addToLogs("Wysłano komendę SDO Read");

}

void MainWindow::on_pushButtonSDOWrite_clicked()
{

}

void MainWindow::on_pushButtoOp_clicked()
{
    //ID=0x000 B0=0x01 (cmd), B1=nodeID
    char msg[24];
    uint8_t nodeID = 0x01;
    sprintf(msg, "M000,%02x%02x000000000000;", 0x01, nodeID);

    this->sendMessageToDevice(msg);
    qDebug() << "Wyslano: " << msg;
}

void MainWindow::on_pushButtonPreOp_clicked()
{
    //ID=0x000 B0=0x80 (cmd), B1=nodeID
    char msg[24];
    uint8_t nodeID = 0x01;
    sprintf(msg, "M000,%02x%02x000000000000;", 0x80, nodeID);

    this->sendMessageToDevice(msg);
    qDebug() << "Wyslano: " << msg;
}

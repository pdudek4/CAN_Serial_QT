#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>

#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>
#include <stdio.h>

//#define _msglog
//#define _time

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("LEM CANopen Magic software");
    secondwindow = new CANOpen(this);
    //this->device = new QSerialPort(this);

    secondwindow->serial_second = &nowyserial;
    //nowyserial.canOpenWin = secondwindow;

    setUi();
    Sniffer = false;

    SDO_Tx_ID = 0x601;
    SDO_Rx_ID = 0x581;


}

MainWindow::~MainWindow()
{
    //delete device;
    delete secondwindow;
    delete ui;
}

void MainWindow::on_pushButtonSearch_clicked()
{
//    this->addToLogs("Szukam urządzeń...");
//    QList<QSerialPortInfo> devices;
//    devices = QSerialPortInfo::availablePorts();

//    for(int i = 0; i < devices.count(); i++) {
//      this->addToLogs("Znalazłem urządzenie: " + devices.at(i).portName() + " " + devices.at(i).description());
//      ui->comboBoxDevices->addItem(devices.at(i).portName() + " " + devices.at(i).description());
//    }
    ui->comboBoxDevices->clear();
    this->addToLogs("Szukam urządzeń...");
    QList<QSerialPortInfo> devices;
    devices = nowyserial.Search();

    for(int i = 0; i < devices.count(); i++) {
      this->addToLogs("Znalazłem urządzenie: " + devices.at(i).portName() + " " + devices.at(i).description());
      ui->comboBoxDevices->addItem(devices.at(i).portName() + " " + devices.at(i).description());
    }
}

void MainWindow::on_pushButtonLink_clicked()
{
//    if(ui->comboBoxDevices->count() == 0)
//    {
//        this->addToLogs("Nie wykryto żadnych urządzeń!");
//        return;
//    }

//    QString comboBoxQString = ui->comboBoxDevices->currentText();
//    QStringList portList = comboBoxQString.split(0x20);
//    QString portName = portList.first();

//    this->device->setPortName(portName);

//    // OTWÓRZ I SKONFIGURUJ PORT:
//    if(!device->isOpen())
//    {

//        if(device->open(QSerialPort::ReadWrite))
//        {
//            this->device->setBaudRate(QSerialPort::Baud115200);
//            this->device->setDataBits(QSerialPort::Data8);
//            this->device->setParity(QSerialPort::NoParity);
//            this->device->setStopBits(QSerialPort::OneStop);
//            this->device->setFlowControl(QSerialPort::NoFlowControl);
//            // CONNECT:
//            connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
//            this->addToLogs("Otwarto port szeregowy.");
//        }
//        else
//        {
//            this->addToLogs("Otwarcie portu szeregowego się nie powiodło!");
//        }
//    }
//    else
//    {
//        this->addToLogs("Port już jest otwarty!");
//        return;
//    }

    if(ui->comboBoxDevices->count() == 0)
    {
        this->addToLogs("Nie wykryto żadnych urządzeń!");
        return;
    }

    QString comboBoxQString = ui->comboBoxDevices->currentText();
    QStringList portList = comboBoxQString.split(0x20);
    QString portName = portList.first();

    //this->device->setPortName(portName);
    nowyserial.device.setPortName(portName);
    // OTWÓRZ I SKONFIGURUJ PORT:
    int nr = nowyserial.Link();
    switch(nr){
    case 1:
        // CONNECT:
        //connect(this->device, SIGNAL(readyRead()),this, SLOT(readFromPort()));
        connect(&nowyserial.device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
        this->addToLogs("Otwarto port szeregowy.");
        break;
    case 2:
        this->addToLogs("Otwarcie portu szeregowego się nie powiodło!");
        break;
    case 3:
        this->addToLogs("Port już jest otwarty!");
        break;
    }
}

void MainWindow::on_pushButtonClose_clicked()
{
    if(nowyserial.device.isOpen()) {
      nowyserial.device.close();
      nowyserial.disconnect(&nowyserial.device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
      this->addToLogs("Zamknięto połączenie.");
    }
    else {
      this->addToLogs("Port nie jest otwarty!");
        emit nowyserial.ready1();
      return;
    }
}

void MainWindow::sendMessageToDevice(QString message)
{
   QString canFrame;

   int nr = nowyserial.sendMessageToDevice(message);
   if(nr == 1){
        canFrame = "->ID: " + message[1] + message[2] + message[3] +
                " Dane: " + message[5] + message[6] + " " +
                message[7] + message[8] + " " + message[9] + message[10] + " " +
                message[11] + message[12] + " " + message[13] + message[14] + " " +
                message[15] + message[16] + " " + message[17] + message[18] + " " +
                message[19] + message[20];
        addToLogs(canFrame);
   }
   else this->addToLogs("Nie mogę wysłać wiadomości. Port nie jest otwarty!");

#ifdef _msglog
    this->addToLogs("Wysyłano do STM: " + message);
#endif
}

void MainWindow::addToLogs(QString message)
{
#ifdef _time
    QString currentDateTime = QTime::currentTime().toString("hh:mm:ss");
    ui->textEditLogi->append(currentDateTime + "   " + message);
#else
    ui->textEditLogi->append(message);
#endif
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
    ui->spinBox_B0->setPrefix("0x");
    ui->spinBox_B1->setPrefix("0x");
    ui->spinBox_B2->setPrefix("0x");
    ui->spinBox_B3->setPrefix("0x");
    ui->spinBox_B4->setPrefix("0x");
    ui->spinBox_B5->setPrefix("0x");
    ui->spinBox_B6->setPrefix("0x");
    ui->spinBox_B7->setPrefix("0x");
    ui->spinBox_B0->setDisplayIntegerBase(16);
    ui->spinBox_B1->setDisplayIntegerBase(16);
    ui->spinBox_B2->setDisplayIntegerBase(16);
    ui->spinBox_B3->setDisplayIntegerBase(16);
    ui->spinBox_B4->setDisplayIntegerBase(16);
    ui->spinBox_B5->setDisplayIntegerBase(16);
    ui->spinBox_B6->setDisplayIntegerBase(16);
    ui->spinBox_B7->setDisplayIntegerBase(16);
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
    ui->spinBoxODValue1->setMaximum(0xFFFF);
    ui->spinBoxODValue2->setMaximum(0xFFFF);
    ui->spinBoxIndex->setPrefix("0x");
    ui->spinBoxSubIndex->setPrefix("0x");
    ui->spinBoxODValue1->setPrefix("0x");
    ui->spinBoxODValue2->setPrefix("0x");
    ui->spinBoxIndex->setDisplayIntegerBase(16);
    ui->spinBoxSubIndex->setDisplayIntegerBase(16);
    ui->spinBoxODValue1->setDisplayIntegerBase(16);
    ui->spinBoxODValue2->setDisplayIntegerBase(16);
    ui->spinBoxIndex->clear();
    ui->spinBoxSubIndex->clear();
    ui->spinBoxODValue1->clear();
    ui->spinBoxODValue2->clear();


    //ui->progressBar->setValue(90);
}

void MainWindow::CanSendMsg(uint16_t ID, uint8_t *tab)
{
    char msg[24];
    sprintf(msg, "M%03x,%02x%02x%02x%02x%02x%02x%02x%02x;", ID,
            tab[0], tab[1], tab[2], tab[3], tab[4], tab[5], tab[6], tab[7]);
    addToLogs(msg);
    this->sendMessageToDevice(msg);
}

void MainWindow::readFromPort()
{
    const QByteArray data = nowyserial.device.readAll();
    QString line = QString(data);
    QString log, tmp;
    bool ok;

    uint16_t i_ID_rcv;
    uint8_t payload[8];

    ID_rcv = line[1];
    ID_rcv += line[2];
    ID_rcv += line[3];

    log.append("<-ID: ");
    log.append(ID_rcv);
    log.append(" Dane: ");

    int i=0;
    for(i=0; i<8; i++){
        tmp.append(line[5+2*i]);
        tmp.append(line[6+2*i]);
        payload[i] = tmp.toInt(&ok, 16);
        tmp.clear();

        log.append(line[5+2*i]);
        log.append(line[6+2*i]);
        log.append(" ");
    }
    if(Sniffer == false){
        this->addToLogs(log);
    }
    else{
        i_ID_rcv = ID_rcv.toInt(&ok, 10);

        int j=0,f=0;
        for (j=0; j<(int)myvector.size(); j++){             //szukanie czy ID juz jest zapisane
            //qDebug() << "1 for: " << j;
            if(myvector[j].ID == i_ID_rcv){
                f = 1;
                break;
                }
        }
        if (f){
            //jest takie ID w vectorze
            for(int i=0; i<8; i++){
                myvector[j].payload[i] = payload[i];
            }
        }
        else{
            //nie ma takiego ID, wiec dodaj
            myvector.push_back(new_Msg());

            j=(int)myvector.size() - 1;
            myvector[j].ID = i_ID_rcv;
            for(int i=0; i<8; i++){
                myvector[j].payload[i] = payload[i];
            }
        }

        ui->textEditLogi->clear();
        for(int i=0; i<(int)myvector.size(); i++){
            tmp.append("ID: ");
            tmp.append(QString::number(myvector[i].ID, 10));
            tmp.append(" >> ");
            for(int j=0; j<8; j++){
                tmp.append(QString::number(myvector[i].payload[j], 16));
                tmp.append("  ");
            }
            tmp.append("\r");
        }
        ui->textEditLogi->append(tmp);
    }
    if(ID_rcv.toInt(&ok, 16) == SDO_Rx_ID){
        SDO_response(line);
        qDebug() << "sdo response";
    }

}


void MainWindow::on_pushButtonFilterSet_clicked()
{
    //wyslij ramke z wartosciami 4 filtrow
    char msg[24];
    sprintf(msg, "FCHG,%03x%03x%03x%03x;;;;;", ui->spinBox_F0->value(),
            ui->spinBox_F1->value(), ui->spinBox_F2->value(), ui->spinBox_F3->value());

    this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtonFilterOff_clicked()
{
    //wyslij ramke z informacja zeby wylaczyc filtrowanie
    this->sendMessageToDevice("FDEA,123456789123;;;;;");
}

void MainWindow::on_pushButtonSendMsg_clicked()
{
    char msg[24];
    sprintf(msg, "M%03x,%02x%02x%02x%02x%02x%02x%02x%02x;", ui->spinBox_ID->value(),
            ui->spinBox_B0->value(), ui->spinBox_B1->value(), ui->spinBox_B2->value(), ui->spinBox_B3->value(),
            ui->spinBox_B4->value(), ui->spinBox_B5->value(), ui->spinBox_B6->value(), ui->spinBox_B7->value());

    this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtonPasmo_clicked()
{
    QString msg = "PASM,";
    QString pasmo_cb = ui->comboBoxPasmo->currentText();
    pasmo_cb.resize(3);
    msg.append(pasmo_cb);
    msg.append("1234567890123;");

    this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtonSDORead_clicked()
{
    //Read OD Value
    // ID=0x601 B0=0x40 B1=indexLSB B2=indexMSB B3=subindex B4-B7=0
    char msg[24];
    uint16_t index;
    uint8_t indexM, indexL, subindex;
    QString log;

    index = ui->spinBoxIndex->value();
    indexM = index >> 8;
    indexL = index & 0xff;
    subindex = ui->spinBoxSubIndex->value();

    sprintf(msg, "M%03x,40%02x%02x%02x00000000;", SDO_Tx_ID, indexL, indexM, subindex);

    log.append("SDO Read 0x");
    log.append(QString::number(index, 16));
    log.append(" ");
    log.append(QString::number(subindex, 16));
    addToLogs(log);

    this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtonSDOWrite_clicked()
{
    //Write OD Value
    // ID=0x601 B0=0x22 B1=indexLSB B2=indexMSB B3=subindex B4-B7=wartość OD
    char msg[24];
    QString log, value, si_value;
    uint16_t index, ODvalue1, ODvalue2;
    uint8_t indexM, indexL, subindex;
    uint32_t ODvalue;
    char DLC;
    uint8_t valueB0 = 0, valueB1 = 0, valueB2 = 0, valueB3 = 0;
    valueB0 = 0;
    valueB1 = 0;
    valueB2 = 0;
    valueB3 = 0;

    index = ui->spinBoxIndex->value();
    indexM = index >> 8;
    indexL = index & 0xff;
    subindex = ui->spinBoxSubIndex->value();
    ODvalue1 = ui->spinBoxODValue1->value();
    ODvalue2 = ui->spinBoxODValue2->value();
    ODvalue = (ODvalue1 << 16) | ODvalue2;
    value = QString::number(ODvalue, 16);

    if(ODvalue <= 0xff){
        valueB0 = ODvalue;
        DLC = '1';
    }
    if((ODvalue > 0xff) && (ODvalue <= 0xffff)){
        valueB1 = ODvalue >> 8;
        valueB0 = ODvalue & 0xff;
        DLC = '2';
    }
    if(ODvalue > 0xffff && ODvalue <= 0xffffff){
        valueB2 = ODvalue >> 16;
        valueB1 = ODvalue >> 8;
        valueB0 = ODvalue & 0xff;
        DLC = '3';
    }
    if(ODvalue > 0xffffff){
        valueB3 = ODvalue >> 24;
        valueB2 = ODvalue >> 16;
        valueB1 = ODvalue >> 8;
        valueB0 = ODvalue & 0xff;
        DLC = '4';
    }
    //zapi i sevcon działa bez dlc w B0 = 0x22
    sprintf(msg, "M%03x,22%02x%02x%02x%02x%02x%02x%02x;", SDO_Tx_ID, indexL, indexM, subindex,
            valueB0, valueB1, valueB2, valueB3);


    si_value = QString::number(ODvalue, 10);
    //komunikat potwierdzający do logów
    log.append("SDO Write 0x");
    log.append(QString::number(index, 16));
    log.append(" ");
    log.append(QString::number(subindex, 16));
    log.append(" =0x");
    log.append(value);
    log.append("  dec=");
    log.append(si_value);
    addToLogs(log);

    this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtonLogin_clicked()
{
    uint8_t dane[8] = {0x22, 0x10, 0x10, 0x01, 0x4F, 0x50, 0x45, 0x4E};
    CanSendMsg(SDO_Tx_ID, dane);
    //char msg[24];
    //sprintf(msg, "M%03x,221010014F50454E;", SDO_Tx_ID);
    //addToLogs(msg);
    //this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtonSave_clicked()
{
    char msg[24];
    sprintf(msg, "M%03x,2210100173617665;", SDO_Tx_ID);
    addToLogs(msg);
    this->sendMessageToDevice(msg);
}

void MainWindow::SDO_response(QString line)
{
    QString log, s_value;
    QString cmd, index, subindex, value;
    unsigned int icmd, sdo_n, i_value, i_index, i_subindex;
    bool ok;
    ui->lineEdit->clear();

    cmd += line[5];
    cmd += line[6];
    icmd = cmd.toInt(&ok, 16);
    sdo_n = (icmd & 0x0c) >> 2;

    index = line[9];
    index += line[10];
    index += line[7];
    index += line[8];

    subindex += line[11];
    subindex += line[12];

    switch(sdo_n){
    case 0:
       value = line[19];
       value += line[20];

       value += line[17];
       value += line[18];

       value += line[15];
       value += line[16];

       value += line[13];
       value += line[14];
       break;
     case 1:
        value = line[17];
        value += line[18];

        value += line[15];
        value += line[16];

        value += line[13];
        value += line[14];
        break;
    case 2:
       value = line[15];
       value += line[16];

       value += line[13];
       value += line[14];
       break;
    case 3:
       value = line[13];
       value += line[14];
       break;
    }

    i_value = value.toInt(&ok, 16);
    s_value = QString::number(i_value);

    log.append("\tOdp 0x");
    log.append(index);
    log.append(" ");
    log.append(subindex);
    log.append("  =0x");
    log.append(value);
    log.append("  dec=");
    log.append(s_value);

    ui->lineEdit->setText("0x" + value);
    this->addToLogs(log);
    this->addToLogs("");

    i_index = index.toUInt(&ok, 16);
    i_subindex = subindex.toUInt(&ok, 16);
   qDebug() << "index: " << i_index << " subindex: " << i_subindex;

    if(index == "5000"){
        if(value == "04") ui->checkBoxLogin->setChecked(1);
        else ui->checkBoxLogin->setChecked(0);
    }
    else if(index == "5110"){ //SEVCON operational
        if(value == "7f") ui->checkBoxPreop->setChecked(1);
        else ui->checkBoxPreop->setChecked(0);
    }
    else if(i_index >= 0x1A00 && i_index <= 0x1A04){
        secondwindow->addValueToTree(i_index, i_subindex, value);
    }
//    else if(i_index >= 0x1800 && i_index <= 0x1804){
//        secondwindow->addValueToTree(i_index, i_subindex, value);
//    }


}

void MainWindow::on_pushButtonLoginSevcon_clicked()
{
    //login set
    // 0x5000, 3, =0  -> user id=0
    // 0x5000, 2, =0x4bdf  -> password
    //login level check 0x5000, 1, 4=Engineering, 1=User
    char msg[24];

    //user id=0
    sprintf(msg, "M%03x,2200500300000000;", SDO_Tx_ID);
    this->sendMessageToDevice(msg);

    volatile int g=0;
    for(int i=0; i<0xFFFF; i++){
        for(int j=0; j<0xFFF; j++){
        g++;
        }
    }

    //login password
    sprintf(msg, "M%03x,22005002df4b0000;", SDO_Tx_ID);
    this->sendMessageToDevice(msg);

}

void MainWindow::on_pushButtonSaveSevcon_clicked()
{
    char msg[24];
    //OD write save parameters
    sprintf(msg, "M%03x,2210100173617665;", SDO_Tx_ID);
    this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtoOpSevcon_clicked()
{
    //op set 0x2800, 0, =0
    //op check 0x5110, 0, =5
    char msg[24];
    sprintf(msg, "M%03x,2200280000000000;", SDO_Tx_ID);
    this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtonPreOpSevcon_clicked()
{
    //pre-op set 0x2800, 0, =1
    //pre-op check 0x5110, 0, =127 or 0x7f
    char msg[24];
    sprintf(msg, "M%03x,2200280001000000;", SDO_Tx_ID);
    this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtonLogSCheck_clicked()
{
    char msg[24];
    //OD read state login level
    sprintf(msg, "M%03x,4000500100000000;", SDO_Tx_ID);
    this->sendMessageToDevice(msg);
}

void MainWindow::on_pushButtonOpSCheck_clicked()
{
    char msg[24];
    //OD read operational state
    sprintf(msg, "M%03x,4010510000000000;", SDO_Tx_ID);
    this->sendMessageToDevice(msg);
}

void MainWindow::on_comboBoxNodeID_currentIndexChanged(int index)
{
    Node_ID = ui->comboBoxNodeID->currentText().toInt();
    SDO_Tx_ID = 0x600 + index + 1;
    SDO_Rx_ID = 0x580 + index + 1;
}

void MainWindow::on_pushButtonTPDO_clicked()
{
    secondwindow->show();
}

void MainWindow::on_radioButton_Sniffer_toggled(bool checked)
{
    Sniffer = checked;
}

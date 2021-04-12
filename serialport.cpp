#include "serialport.h"

#include <QDebug>
#include <QList>
#include <QSerialPortInfo>

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{

}

QList<QSerialPortInfo> SerialPort::Search()
{
    devices = QSerialPortInfo::availablePorts();
    return devices;
}

int SerialPort::Link()
{
    // OTWÓRZ I SKONFIGURUJ PORT:
    if(!device.isOpen())
    {
        if(device.open(QSerialPort::ReadWrite))
        {
            this->device.setBaudRate(QSerialPort::Baud115200);
            this->device.setDataBits(QSerialPort::Data8);
            this->device.setParity(QSerialPort::NoParity);
            this->device.setStopBits(QSerialPort::OneStop);
            this->device.setFlowControl(QSerialPort::NoFlowControl);
            return 1;
           // this->addToLogs("Otwarto port szeregowy.");
        }
        else
        {
            return 2;
            //this->addToLogs("Otwarcie portu szeregowego się nie powiodło!");
        }
    }
    else
    {
        //this->addToLogs("Port już jest otwarty!");
        return 3;
    }
}

int SerialPort::sendMessageToDevice(QString message)
{
    if(device.isOpen() && device.isWritable()) {
        device.write(message.toStdString().c_str());
        qDebug() << "wyslano: " + message;
        return 1;
    }
    else {
        qDebug() << "nie udalo sie wyslac";
        return 2;
    }

}

//void SerialPort::ready1(){

//    qDebug() << "Wywolano serialpoet ready";
//}

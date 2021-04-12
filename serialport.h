#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>

//#include "canopen.h"

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);


    QSerialPort device;
    QList<QSerialPortInfo> devices;



    QList<QSerialPortInfo> Search();
    int Link();
    int sendMessageToDevice(QString message);

private:
    //CANOpen *canOpenWin;

private slots:


signals:
     void ready1();

};

#endif // SERIALPORT_H

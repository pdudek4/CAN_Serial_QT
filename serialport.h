#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);


    QSerialPort device;
    QList<QSerialPortInfo> devices;


    QList<QSerialPortInfo> Search();
    int Link();

    int SendMessageToDevice(QString message);

private slots:
     void ReadFromPort();
signals:

};

#endif // SERIALPORT_H

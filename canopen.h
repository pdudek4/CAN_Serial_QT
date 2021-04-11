#ifndef CANOPEN_H
#define CANOPEN_H

#include <QDialog>
#include <QDebug>

#include "serialport.h"

namespace Ui {
class CANOpen;
}

class CANOpen : public QDialog
{
    Q_OBJECT

public:
    explicit CANOpen(QWidget *parent = nullptr);
    ~CANOpen();

    SerialPort *serial_second;

private slots:


    void on_pushButtonOp_clicked();

private:
    Ui::CANOpen *ui;

};

#endif // CANOPEN_H

#include "canopen.h"
#include "ui_canopen.h"

CANOpen::CANOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CANOpen)
{
    ui->setupUi(this);

}

CANOpen::~CANOpen()
{
    delete ui;
}


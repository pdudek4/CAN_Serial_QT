#ifndef CANOPEN_H
#define CANOPEN_H

#include <QDialog>
#include <QDebug>
#include <QTreeWidgetItem>

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

    Ui::CANOpen *ui;
    SerialPort* serial_second;
    void setTree();
    void addValueToTree(unsigned int index, unsigned int subindex, QString value);

private slots:

    void on_pushButtonOp_clicked();
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_comboBoxNodeID_currentIndexChanged(int index);

private:
    uint8_t Node_ID;
    uint16_t SDO_Tx_ID;
    uint16_t SDO_Rx_ID;

    QTreeWidgetItem TreeItem[5][9];

    void createTreeItems();

};

#endif // CANOPEN_H

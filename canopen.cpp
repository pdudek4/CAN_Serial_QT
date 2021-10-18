#include "canopen.h"
#include "ui_canopen.h"


CANOpen::CANOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CANOpen)
{
    ui->setupUi(this);

    SDO_Tx_ID = 0x601;
    SDO_Rx_ID = 0x581;

    createTreeItems();
    setTree();
}

CANOpen::~CANOpen()
{
    delete ui;
}


void CANOpen::on_pushButtonOp_clicked()
{
    serial_second->sendMessageToDevice("hello");
}

void CANOpen::setTree()
{
    ui->treeWidget->setColumnCount(2);

    QStringList headerLabels;
    headerLabels.push_back(tr("Index"));
    headerLabels.push_back(tr("Value"));
    ui->treeWidget->setHeaderLabels(headerLabels);
}

void CANOpen::addValueToTree(unsigned int index, unsigned int subindex, QString value)
{
    // index i subindex do tablicy obeiktów qtwItem value jako string
    int parent = index - 0x1A00;
    TreeItem[parent][subindex+1].setText(1, value);
    qDebug() << "add tp tree";
}

void CANOpen::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString index, subindex;
    bool ok;

    if(item->childCount() != 0) return;
    if(column == 1) return;

    index = item->parent()->text(0);
    subindex = item->text(0);
    qDebug() << "Kliknięty item: " + index + " " + subindex;

    char msg[24];
    uint16_t i_index;
    uint8_t i_indexM, i_indexL, i_subindex;

    i_index = index.toInt(&ok, 16);
    i_indexM = i_index >> 8;
    i_indexL = i_index & 0xff;
    //i_subindex = subindex.toInt(&ok, 10);
    i_subindex = item->parent()->indexOfChild(item);

    sprintf(msg, "M%03x,40%02x%02x%02x00000000;", SDO_Tx_ID, i_indexL, i_indexM, i_subindex);

    serial_second->sendMessageToDevice(msg);
    qDebug() << msg;
}

void CANOpen::on_comboBoxNodeID_currentIndexChanged(int index)
{
    Node_ID = ui->comboBoxNodeID->currentText().toInt();
    SDO_Tx_ID = 0x600 + index + 1;
    SDO_Rx_ID = 0x580 + index + 1;
    qDebug() << SDO_Tx_ID;
}

void CANOpen::createTreeItems()
{
    char nrsub[5], nri[18];
//TPDO communication parameters
    for(int i=0; i<5; i++){
        std::sprintf(nri, "180%d", i);
        TreeItem1[i][0].setText(0, nri);
        std::sprintf(nri, "TPDO %d com param", i);
        TreeItem1[i][0].setText(1, nri);
        ui->treeWidget->addTopLevelItem(&TreeItem1[i][0]);

        //std::sprintf(nrsub, "%d", j-1);
        TreeItem1[i][1].setText(0, "Largest subindex");
        TreeItem1[i][2].setText(0, "COB-ID used by PDO");
        TreeItem1[i][3].setText(0, "Transmission type");

        TreeItem1[i][0].addChild(&TreeItem1[i][1]);
        TreeItem1[i][0].addChild(&TreeItem1[i][2]);
        TreeItem1[i][0].addChild(&TreeItem1[i][3]);

    }

//TPDO mapping parameters
    for(int i=0; i<5; i++){
        std::sprintf(nri, "1A0%d", i);
        TreeItem[i][0].setText(0, nri);
        std::sprintf(nri, "TPDO %d map param", i);
        TreeItem[i][0].setText(1, nri);
        ui->treeWidget->addTopLevelItem(&TreeItem[i][0]);

        TreeItem[i][1].setText(0, "Nr of mapped objects");
        TreeItem[i][0].addChild(&TreeItem[i][1]);

        for(int j=2; j<9; j++){
          std::sprintf(nrsub, "Map%d", j-1);
          TreeItem[i][j].setText(0, nrsub);
          TreeItem[i][0].addChild(&TreeItem[i][j]);
        }
    }

}

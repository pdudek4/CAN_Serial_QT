#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canopen.h"
#include "serialport.h"
#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSearch_clicked();
    void on_pushButtonLink_clicked();
    void on_pushButtonClose_clicked();
    void readFromPort();
    void on_pushButtonFilterSet_clicked();
    void on_pushButtonFilterOff_clicked();
    void on_pushButtonSendMsg_clicked();

    void on_pushButtonPasmo_clicked();
    void on_pushButtonSDORead_clicked();
    void on_pushButtonSDOWrite_clicked();
    void on_pushButtonLogin_clicked();
    void on_pushButtonSave_clicked();


    void SDO_response(QString line);
    void on_pushButtoOpSevcon_clicked();
    void on_pushButtonLoginSevcon_clicked();
    void on_pushButtonSaveSevcon_clicked();
    void on_pushButtonPreOpSevcon_clicked();
    void on_pushButtonLogSCheck_clicked();
    void on_pushButtonOpSCheck_clicked();
    void on_comboBoxNodeID_currentIndexChanged(int index);

    void on_pushButtonTPDO_clicked();

    void on_radioButton_Sniffer_toggled(bool checked);


private:
    Ui::MainWindow *ui;
    //QSerialPort *device;
    CANOpen *secondwindow;
    SerialPort nowyserial;

    void addToLogs(QString message);
    void sendMessageToDevice(QString message);
    void setUi(void);
    void CanSendMsg(uint16_t ID, uint8_t *tab);

    QString ID_rcv;
    uint8_t data_rcv[8];
    uint8_t Node_ID;
    uint8_t DLC_in;
    uint8_t DLC_out;
    uint16_t SDO_Tx_ID;
    uint16_t SDO_Rx_ID;
    enum Op_state {preop, op};
    struct new_Msg{
        uint16_t ID;
        uint8_t DLC;
        uint8_t payload[8];
    };
    bool Sniffer;
    std::vector<new_Msg> myvector;


};
#endif // MAINWINDOW_H

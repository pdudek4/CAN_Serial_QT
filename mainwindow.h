#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;
    QSerialPort *device;
    void addToLogs(QString message);
    void sendMessageToDevice(QString message);
    void setUi(void);

    QString ID_rcv;
    uint8_t data_rcv[8];
    //uint8_t DLC_rcv;


};
#endif // MAINWINDOW_H
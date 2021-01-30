#ifndef CANOPEN_H
#define CANOPEN_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class CANOpen;
}

class CANOpen : public QDialog
{
    Q_OBJECT

public:
    explicit CANOpen(QWidget *parent = nullptr);
    ~CANOpen();

private slots:


private:
    Ui::CANOpen *ui;

};

#endif // CANOPEN_H

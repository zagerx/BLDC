#ifndef SERIALWINDOW_H
#define SERIALWINDOW_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class serialwindow;
}


class serialwindow : public QWidget
{
    Q_OBJECT

public:
    explicit serialwindow(QWidget *parent = nullptr);
    ~serialwindow();
    void SerialPortInit(void);
private slots:
    void on_pushButton_clicked();

    void on_enter_bt_clicked();

    void on_mc_startBt_clicked();

    void on_mt_stopBt_clicked();

    void sendKpCommand();

    void sendKiCommand();

    void sendKcCommand();

    void sendIdCommand();

    void sendIqCommand();

    void onReadSerialData();
private:
    Ui::serialwindow *ui;
    QSerialPort *serial;

};

#endif // SERIALWINDOW_H

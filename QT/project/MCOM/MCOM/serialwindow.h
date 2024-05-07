#ifndef SERIALWINDOW_H
#define SERIALWINDOW_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>

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

    void on_mc_pidset_clicked();

    void sendParamEnter();

    void on_mc_speed_Bt_clicked();

    void processdata(QByteArray data);
private:
    Ui::serialwindow *ui;
    QSerialPort *serial;
    // QLabel *lightLabel;
};

#endif // SERIALWINDOW_H

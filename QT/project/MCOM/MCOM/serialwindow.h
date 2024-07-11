#ifndef SERIALWINDOW_H
#define SERIALWINDOW_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>
#include "mcprotocol.h"


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
    McProtocol *pMcProtocl;
private slots:

    void on_mc_startBt_clicked();

    void on_mt_stopBt_clicked();

    void onReadSerialData();

    void processdata(QByteArray data);
    void on_debug_bt_clicked();
    void onDataReceivedFromB(const QString &data); // 接收B界面传递的数据

    void on_enseriBt_clicked();

    void on_normal_bt_clicked();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::serialwindow *ui;
    QSerialPort *serial;
};

#endif // SERIALWINDOW_H

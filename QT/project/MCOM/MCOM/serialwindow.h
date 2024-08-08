#ifndef SERIALWINDOW_H
#define SERIALWINDOW_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>
#include "mc_protocol/mc_protocol.h"
#include "mc_protocol/commands.h"

namespace Ui
{
    class serialwindow;
}

class serialwindow : public QWidget
{
    Q_OBJECT

public:
    explicit serialwindow(QWidget *parent = nullptr);
    ~serialwindow();
    void SerialPortInit(void);
    MCProtocol *pMcProtocl;
    QMap<QString, int> commandMap = {
                                     {"速度", M_SET_SPEED},
                                     {"启动", M_SET_START},
                                     {"停止", M_SET_STOP},
                                     {"板子信息",M_GET_PCBAInfo}
    };
private slots:
    void on_mc_startBt_clicked();
    void on_mt_stopBt_clicked();
    void on_debug_bt_clicked();
    void on_enseriBt_clicked();
    void on_normal_bt_clicked();
    void on_cmd_enBt_clicked();

    void onReadSerialData();
    void processdata(QByteArray data);
    void onDataReceivedFromB(const QString &data); // 接收B界面传递的数据
    void timerTick(void);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::serialwindow *ui;
    QSerialPort *serial;
    QTimer *timer;
};

#endif // SERIALWINDOW_H

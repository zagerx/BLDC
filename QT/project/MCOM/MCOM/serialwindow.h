#ifndef SERIALWINDOW_H
#define SERIALWINDOW_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>
#include "mc_protocol/mc_protocol.h"
#include "mc_protocol/commands.h"
#include <functional>
#include <QtCharts>

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
    void led_blink(std::vector<unsigned char>& input);

    MCProtocol *pMcProtocl;
    QMap<QString, int> commandMap = {
                                     {"速度", M_SET_SPEED},
                                     {"启动", M_SET_START},
                                     {"停止", M_SET_STOP},
                                     {"控制参数信息",M_GET_CtrlParaseInfo},
                                     {"设置PID参数",M_SET_PIDParam},
                                     {"设置PID目标",M_SET_PIDTarge}
    };

    void _forch_cmdmap(unsigned short cmd, std::vector<unsigned char>& input);
private slots:
    void on_mc_startBt_clicked();
    void on_mt_stopBt_clicked();
    void on_debug_bt_clicked();
    void on_enseriBt_clicked();
    void on_normal_bt_clicked();
    void on_cmd_enBt_clicked();

    void onReadSerialData();
    void onDataReceivedFromB(const QString &data); // 接收B界面传递的数据
    void timerTick(void);

    void on_ClearRicevBt_clicked();

    void on_enseriBt_3_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void charts_init(void);
    void SendThread(void);
    void ReciveThread(void);
    void processdata(void);
    QChart *chart_1;
    QValueAxis *axis_x;
    QValueAxis *axis_y;
    QLineSeries *line;
    Ui::serialwindow *ui;
    QSerialPort *serial;
    QTimer *timer;
};


extern serialwindow *pserialwind;

#endif // SERIALWINDOW_H

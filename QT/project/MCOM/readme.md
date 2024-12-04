# 串口调试助手
## C++基础知识

```C++

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
    QMap<QString, int> commandMap = {
                                     {"速度", M_SET_SPEED},
                                     {"位置", M_SET_POS},
    };
    QSerialPort *serial;
    QCustomPlot *customPlot;
private slots:
    void on_mc_startBt_clicked();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::serialwindow *ui;
};

```



## UI界面
- 通信接口(串口/CAN)
## 任务线程
- 数据处理线程
    - 协议解析
        - 数据波形显示
        - 数据textlab显示
- 数据发送线程
    - 协议发送

- UI界面触发
    - 各种按键

- 1ms定时器线程


封装成类:

- 协议类(protocol_class)
    - 数据帧
    - 数据获取
    - 数据发送
- 串口类
    - 串口的打开
    - 串口数据的获取
    - 串口数据的发送

## 数据类型
- byte
## 数据获取
## 数据发送
## 数据粘包
## 接收到数据缓存
## 发送到数据缓存
## 波形图的绘制
## 通信协议



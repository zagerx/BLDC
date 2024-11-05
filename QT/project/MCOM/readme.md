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
## 数据获取
## 数据发送
## 数据粘包
## 接收到数据缓存
## 发送到数据缓存
## 波形图的绘制
## 通信协议



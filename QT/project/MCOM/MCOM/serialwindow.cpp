#include "serialwindow.h"
#include "ui_serialwindow.h"
#include <QtDebug>
#include <QThread>
#include <QTimer>
#include "mc_protocol/mc_protocol.h"
#include "mc_protocol/mc_frame.h"
#include "debugwindow.h"
#include "comment.h"

serialwindow *pserialwind;

/*
 * 界面初始化
 */
serialwindow::serialwindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::serialwindow), timer(new QTimer(this))
{
    ui->setupUi(this);
    SerialPortInit();
    ui->mc_startBt->setDisabled(true);
    ui->mt_stopBt->setDisabled(true);

    QLabel *lightLabel;
    lightLabel = ui->LED_Label;
    lightLabel->setScaledContents(true); // /*根据控件大小缩放图片*/
    lightLabel->setFixedSize(30, 30);    // 设置控件大小为 50x50
    lightLabel->setPixmap(QPixmap("../MCOM/images/GrapLED.png")); // 设置初始状态为灭灯

    /*初始化图表*/
    charts_init();
    // 设置定时器的时间间隔为1ms
    timer->setInterval(1);
    // 将定时器的timeout()信号连接到槽函数timerTick()
    connect(timer, &QTimer::timeout, this, &serialwindow::timerTick);
    // 启动定时器
    timer->start();
}
/*
 * 界面反向初始化
 */
serialwindow::~serialwindow()
{
    delete serial;    // 确保删除 serial 指向的对象
    serial = nullptr; // 防止悬空指针

    delete pMcProtocl;
    pMcProtocl = nullptr;

    delete timer;
    delete ui;
}
/*
* 图表charts的初始化
*/
void serialwindow::charts_init()
{
    chart_1 = new QChart;
    // chart_1->setTitle("data collo");
    ui->graphicsView->setChart(chart_1);
    //设置X轴
    axis_x = new QValueAxis;
    axis_x->setTitleText("时间");
    axis_x->setRange(0,100);
    chart_1->addAxis(axis_x,Qt::AlignBottom);
    //设置Y轴
    axis_y = new QValueAxis;
    axis_y->setTitleText("value");
    axis_y->setRange(-13,13);
    chart_1->addAxis(axis_y,Qt::AlignLeft);
    //增加数据
    line = new QLineSeries;
    line->setName("正弦函数");
    chart_1->addSeries(line);
    line->attachAxis(axis_x);
    line->attachAxis(axis_y);
}

/*
* 窗口关闭回调
*/
void serialwindow::closeEvent(QCloseEvent *event)
{
    // 请求在事件循环的下一个迭代中删除此对象
    serial->close();
    deleteLater();
}

/*
 *   按键回调  on_mc_startBt_clicked/
 *            on_mt_stopBt_clicked/
 *            on_normal_bt_clicked/
 *            on_debug_bt_clicked
 *   后续使用单选框，以简化代码
 */
void serialwindow::on_mc_startBt_clicked()
{
    MC_Frame datafram;
    datafram.CMD = M_SET_START;
    datafram.UnPack();
    pMcProtocl->AddFrameToBuf(datafram);
}
void serialwindow::on_mt_stopBt_clicked()
{
    MC_Frame datafram;
    datafram.CMD = M_SET_STOP;
    datafram.UnPack();
    pMcProtocl->AddFrameToBuf(datafram);
}
void serialwindow::on_normal_bt_clicked()
{
    MC_Frame datafram;
    datafram.CMD = M_SET_NormalM;
    datafram.UnPack();
    pMcProtocl->AddFrameToBuf(datafram);
    ui->mc_startBt->setDisabled(false);
    ui->mt_stopBt->setDisabled(false);
}
void serialwindow::on_debug_bt_clicked()
{
    /*发送命令 使电机进入debug模式*/
    QString command = "motor_debugmode:\r\n";
    serial->write(command.toLatin1());

    motordebug *pmd = new motordebug;
    // /*connect*/(b, &B::dataReady, this, &A::onDataReceivedFromB); // 连接B界面的信号到A界面的槽
    connect(pmd, &motordebug::dataReady, this, &serialwindow::onDataReceivedFromB);
    pmd->show();
}
void serialwindow::on_enseriBt_clicked()
{
    QPushButton *openbutton = ui->enseriBt;
    QComboBox *portBt = ui->portBox;
    QString pstr = portBt->currentText();

    if (openbutton->text() == "关闭串口")
    {
        if (serial && serial->isOpen())
        {
            serial->close();
        }
        openbutton->setText("打开串口");
        return;
    }

    // 查找并尝试打开串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if (pstr == info.portName())
        {
            if (!serial || !serial->isOpen())
            {
                if (!serial)
                {
                    qDebug() << "mallocc";
                    serial = new QSerialPort; // 创建新对象
                }
                serial->setPort(info);
                if (serial->open(QIODevice::ReadWrite))
                {
                    openbutton->setText("关闭串口");
                    serial->setBaudRate(QSerialPort::Baud115200);
                    serial->setParity(QSerialPort::NoParity);
                    serial->setDataBits(QSerialPort::Data8);
                    serial->setStopBits(QSerialPort::OneStop);
                    return;
                }
            }
            // 如果串口已打开但不是当前选中的端口，则关闭它
            else
            {
                serial->close();
            }
        }
    }

    // 如果没有找到匹配的串口或打开失败，重置按钮文本
    openbutton->setText("打开串口");

    // 如果之前创建了新的 serial 对象但没有成功打开串口，则删除它
    if (serial && !serial->isOpen())
    {
        delete serial;
        serial = nullptr;
    }
}
void serialwindow::on_cmd_enBt_clicked()
{
    MC_Frame datafram;

    QString currentText = ui->cmd_portBox->currentText();
    if (commandMap.contains(currentText)) {
        datafram.CMD = commandMap.value(currentText);
    } else {
        qDebug() << "未知命令：" << currentText;
        return;
    }

    currentText = ui->data_lineEdit->text();
    qDebug()<<currentText;
    datafram.data = stringToUCharVectorOptimized(currentText);// 调用函数，将QString中的浮点数转换为unsigned char向量;
    datafram.UnPack();
    datafram.PrintFrame();
    pMcProtocl->AddFrameToBuf(datafram);    
}

/*
 *  定时器回调 
 */
void serialwindow::SendThread()
{
    if (!pMcProtocl->SendbufHasData())
    {
        return;
    }
    std::vector<unsigned char> data = pMcProtocl->RDFromSendBuf();
    QByteArray byteArray;
    byteArray.resize(data.size());
    std::copy(data.begin(), data.end(), byteArray.begin());
    serial->write(byteArray);
}

void serialwindow::ReciveThread()
{
    MC_Frame curframe;
    if(pMcProtocl->RDFrameFromRecvBuf(&curframe) == false)
    {
        return;
    }
    //遍历整个协议地图
    _forch_cmdmap(curframe.CMD,curframe.data);
}

void serialwindow::timerTick()
{
    ReciveThread();
    SendThread();
}

/*
 *  串口初始化
 */
void serialwindow::SerialPortInit()
{
    serial = new (QSerialPort);
    QSerialPort *curPort;
    curPort = new (QSerialPort);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        curPort->setPort(info);
        if (curPort->open(QIODevice::ReadWrite))
        {
            ui->portBox->addItem(info.portName());
            curPort->close();
            qDebug() << "串口打开成功";
        }
        else
        {
            qDebug() << "串口打开失败，请重试";
        }
    }
    delete (curPort);
    size_t sendBufferSize = 512; // 假设的发送缓冲区大小
    pMcProtocl = new MCProtocol(sendBufferSize);
    connect(serial, &QSerialPort::readyRead, this, &serialwindow::onReadSerialData);
}

/*
*   串口接收回调
*/
void serialwindow::onReadSerialData()
{
    // 假设 serial 已经被正确初始化和打开
    if (serial && serial->isOpen())
    {
        // 读取串口上所有可用的数据
        QByteArray data = serial->readAll();

        // 如果读取到了数据
        if (!data.isEmpty())
        {
            std::vector<unsigned char> vecData(data.constBegin(), data.constEnd());
            pMcProtocl->ReceiveData(vecData);
        }
        else
        {
            // 没有读取到数据，可以按需处理
            qDebug() << "No data available.";
        }
    }
    else
    {
        // 串口未打开或未初始化，可以给出警告或错误提示
        qDebug() << "Serial port is not open or not initialized.";
    }
}

void serialwindow::onDataReceivedFromB(const QString &data)
{
    // 在这里处理从B界面接收到的数据
    qDebug() << "Received data from B:" << data;
    // 由于B界面在此时可能已经被销毁，确保不要再访问b的指针
    QString command;
    command += data;
    serial->write(command.toUtf8());
}

void serialwindow::led_blink(std::vector<unsigned char>& input)
{
    static unsigned char _state = 0;
    QLabel *lightLabel;
    lightLabel = ui->LED_Label;    
    if (_state)
    {
        _state = 0;
        lightLabel->setPixmap(QPixmap("../MCOM/images/GreenLED.png")); // 设置初始状态为灭灯
    }else{
        _state = 1;
        lightLabel->setPixmap(QPixmap("../MCOM/images/GrapLED.png")); // 设置初始状态为灭灯
    }
}

void serialwindow::_forch_cmdmap(unsigned short cmd, std::vector<unsigned char>& input)
{
    switch (cmd)
    {
    case S_HeartP:
        led_blink(input);
        break;
    case S_MotorSpeed:
        {
            qreal y;
            static unsigned int x = 0;
            x++;
            axis_x->setRange(0,x+20);
            float value;
            getFloatsFromBytes(input,&value);
            y = value;
            line->append(x,y);            
        }
        break;
    default:
        break;
    }
}

/******************************************************************************
 * @file serialwindow.cpp
 * @author zager
 * @version 0.1
 * @date 2024-09-25
 * 
 * @copyright Copyright (c) 2024
 * 
 * @brief 
  ---------------------------------------------------------------------------*/
#include "serialwindow.h"
#include "ui_serialwindow.h"
#include <QtDebug>
#include <QThread>
#include <QTimer>
#include <QByteArray>
#include <QVector>
#include "mc_protocol/mc_protocol.h"
#include "mc_protocol/mc_frame.h"
#include "debugwindow.h"
#include "comment.h"
serialwindow *pserialwind;

/******************************************************************************
 * @brief Construct a new serialwindow::serialwindow object
 * 界面初始化
 * @param parent 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
serialwindow::serialwindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::serialwindow),
     timer(new QTimer(this)),
     m_ProcessThread(new ProcessThread(this))
{
    ui->setupUi(this);
    this->resize(1200,800);

    ButtonInit();

    SerialPortInit();


    // 设置定时器的时间间隔为1ms
    timer->setInterval(1);
    // 将定时器的timeout()信号连接到槽函数timerTick()
    connect(timer, &QTimer::timeout, this, &serialwindow::timerTick);
    // 启动定时器
    timer->start();


    connect(m_ProcessThread, &ProcessThread::ProcessData, this, &serialwindow::ReciveThread);
    // 启动线程
    m_ProcessThread->start();      
}
/******************************************************************************
 * @brief Destroy the serialwindow::serialwindow object
 * 界面反向初始化
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
serialwindow::~serialwindow()
{
    delete serial;    // 确保删除 serial 指向的对象
    serial = nullptr; // 防止悬空指针

    delete pMcProtocl;
    pMcProtocl = nullptr;

    delete timer;
    delete ui;
}
/******************************************************************************
 * @brief 
 * 波形图初始化
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
void serialwindow::WaveformGraphInit()
{   
    customPlot = ui->CustomPlot;
#if 1// 1：使用子图
    //Generate data
    QVector<double> x,ys,yc,y_cs;
    for(double xi=-2*M_PI;xi<2*M_PI;xi+=0.1)
    {
        x.push_back(xi);
        ys.push_back(sin(xi));
        yc.push_back(cos(xi));
        y_cs.push_back(sin(xi)+cos(xi));
    }

    //Create Rectangle Area and add to QCustomplot
    QCPAxisRect * R00=new QCPAxisRect(customPlot);
    QCPAxisRect * R10=new QCPAxisRect(customPlot);
    customPlot->plotLayout()->clear();
    customPlot->plotLayout()->addElement(0,0,R00);
    customPlot->plotLayout()->addElement(1,0,R10);

    //Draw graphs with data
    pGraph2 = customPlot->addGraph(R00->axis(QCPAxis::atBottom),R00->axis(QCPAxis::atLeft));
    pGraph1 = customPlot->addGraph(R10->axis(QCPAxis::atBottom),R10->axis(QCPAxis::atLeft));
    pGraph1->setPen(QPen(Qt::blue));
    pGraph2->setPen(QPen(Qt::red));
    customPlot->graph(0)->setData(x,ys);
    customPlot->graph(0)->rescaleAxes();
    customPlot->graph(0)->setName("channl0");

    customPlot->graph(1)->setData(x,yc);
    customPlot->graph(1)->rescaleAxes();
    customPlot->graph(1)->setName("channl1");
    //Add Legends
    QCPLegend *arLegend00=new QCPLegend;
    R00->insetLayout()->addElement(arLegend00,Qt::AlignTop|Qt::AlignRight);
    arLegend00->setLayer("legend");
    arLegend00->addItem(new QCPPlottableLegendItem(arLegend00, customPlot->graph(0)));

    QCPLegend *arLegend10=new QCPLegend;
    R10->insetLayout()->addElement(arLegend10,Qt::AlignTop|Qt::AlignRight);
    arLegend10->setLayer("legend");
    arLegend10->addItem(new QCPPlottableLegendItem(arLegend10, customPlot->graph(1)));

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->axisRect()->setRangeZoomFactor(1.2,1);
    return;
#else


    // 设置坐标轴标签名称
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    // 设置坐标轴显示范围，不设置时默认范围为 0~5
    customPlot->xAxis->setRange(0, 1000);
    customPlot->yAxis->setRange(-100, 100);
    pGraph1 = customPlot->addGraph();
    pGraph2 = customPlot->addGraph();

    pGraph1->setPen(QPen(Qt::blue));
    pGraph2->setPen(QPen(Qt::red));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->axisRect()->setRangeZoomFactor(1.2,1);
    return;
#endif

}
/******************************************************************************
 * @brief 数据发送线程
 * 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
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
/******************************************************************************
 * @brief 从接收缓冲区读取协议帧
 * 
 * @param ret 
 * @return uint8_t 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
uint8_t serialwindow::read_fromQByteArray(QByteArray &ret)
{
    unsigned char data;
    enum
    {
        START,
        FIRST,
        SECOND,
        READ,
        IDLE,
    };
    unsigned char chState = START;
    switch (chState)
    {
    case START:
        chState = FIRST;
    case FIRST:
        data = serial_recivbuf.front();
        serial_recivbuf.removeFirst();
        if(data == 0xA5)
        {
            ret.append(data);
            chState = SECOND;
        }else{
            break;
        }
    case SECOND:
        data = serial_recivbuf.front();
        serial_recivbuf.removeFirst();
        if(data == 0xA5)
        {
            ret.append(data);
            chState = READ;
        }else{
            break;
        }
    case READ:
        do{
            data = serial_recivbuf.front();
            ret.append(data);
            serial_recivbuf.removeFirst();
            if(data == 0x5A)
            {
                data = serial_recivbuf.front();
                ret.append(data);
                serial_recivbuf.removeFirst();
                if(data == 0x5A)
                {
                    return 0;//读取成功
                }
            }
            if(serial_recivbuf.size()<=0)
            {
                return 1;//读取失败
            }
        }while(1);
    default:
        break;
    }
    return 1;
}

/******************************************************************************
 * @brief 数据处理线程
 * 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
void serialwindow::ReciveThread()
{
    // qDebug()<<"ReciveThread"<<serial_recivbuf.size();
    QByteArray temp;
    // if(/*serial_recivbuf*/.size()>=32)
    {
        for(unsigned short i = 0;i<32;i++)
        {
            if(read_fromQByteArray(temp))
            {
                continue;
            }
            if(temp.size()<10){
                continue;
            }

            if (static_cast<unsigned char>(temp[0]) != (FRAME_HEAD >> 8) || static_cast<unsigned char>(temp[0]) != (FRAME_HEAD & 0xFF)) {
                continue;
            }
            MC_Frame frame;
            frame.CMD = (temp[2] << 8) | temp[3];
            uint16_t dataLength = (temp[4] << 8) | temp[5];
            if (temp.size() != 10 + dataLength) {
                continue ;
            }
            frame.data.clear(); // 清空frame的data成员
            for (size_t i = 0; i < dataLength; ++i) {
                frame.data.push_back(temp[6 + i]);
            }
            _forch_cmdmap(frame.CMD,frame.data);
        }
    }
    return;
}

/******************************************************************************
 * @brief 1ms定时器回调
 * 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
void serialwindow::timerTick()
{
    ReciveThread();
    SendThread();
    refreshWaveformDisplay();
}

/******************************************************************************
 * @brief 串口接收回调
 * 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
void serialwindow::onReadSerialData()
{
    QMutexLocker locker(&m_dataMutex); 
    // 假设 serial 已经被正确初始化和打开
    if (serial == NULL)
    {
        // 串口未打开或未初始化，可以给出警告或错误提示
        qDebug() << "Serial port is not open or not initialized.";
        return;
    }
    // 读取串口上所有可用的数据
    QByteArray data = serial->readAll();
    // 如果读取到了数据
    if (data.isEmpty())
    {
        return;
    }
    // qDebug()<<"processing"<<serial_recivbuf.size();

    // 直接比较前两个字节是否等于 0xA5A5
    // if (static_cast<unsigned char>(data[0]) == 0xA5 && static_cast<unsigned char>(data[1]) == 0xA5)
    {
        // if(static_cast<unsigned char>(data[3] == 0x04))
        {
            /*添加到缓冲区*/
            // qDebug()<<serial_recivbuf.size();
            if(serial_recivbuf.size()+data.size()>=40960)
            {
                qDebug()<<"recivbuf over";
                return;
            }
            serial_recivbuf.append(data);
        }
        // else{
        //     static unsigned int cout = 0;
        //     std::vector<unsigned char> data;
        //     led_blink(data);
        // }
    }

    // else{
    //     QString stringData = QString::fromUtf8(data.constData(), data.size());
    //     // 将字符串显示到QTextEdit
    //     QTextEdit *edit = ui->textEdit;
    //     edit->append(stringData);
    // }
}
/******************************************************************************
 * @brief 命令处理
 * 
 * @param cmd 
 * @param input 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
void serialwindow::_forch_cmdmap(unsigned short cmd, std::vector<unsigned char>& input)
{
    switch (cmd)
    {
    case S_HeartP:
        led_blink(input);
        break;
    case S_MotorSpeed://TODO
        {
            float value[2];
            getFloatsFromBytes(input, &value[0]);
            WaveGraph_1_Queue.enqueue(value[0]);
            WaveGraph_2_Queue.enqueue(value[1]);
        }
        break;
    default:
        break;
    }
}
/******************************************************************************
 * @brief 刷新波形图
 * 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
void serialwindow::refreshWaveformDisplay()
{
    QVector<double> x1; // 时间轴，这里使用简单的递增索引
    QVector<double> y1; // 波形数据，从floatQueue中读取
    QVector<double> y2; // 波形数据，从floatQueue中读取
    static int count = 0;
  
    // 从队列中读取数据
    int queueSize = WaveGraph_1_Queue.size();
    if((WaveGraph_1_Queue.isEmpty() || WaveGraph_1_Queue.isEmpty()))
    {
        return;
    }
    // 清空x1和y1，准备填充新数据  
    x1.clear();
    y1.clear();
    y2.clear();
    for (int i = 0; i < queueSize; ++i) {  
        float value = WaveGraph_1_Queue.dequeue(); // 出队并获取值
        y1.append(static_cast<double>(value)); // 将float转换为double并添加到y1  
        x1.append(static_cast<double>(count++)); // 使用索引作为时间轴的值
        float value2 = WaveGraph_2_Queue.dequeue();
        y2.append(static_cast<double>(value2));
    }
    // 设置图表数据  
    pGraph1->addData(x1,y1);
    pGraph2->addData(x1,y2);

    customPlot->graph(0)->rescaleAxes();
    customPlot->graph(1)->rescaleAxes(true);

    // 重绘图表  
    customPlot->replot();
}

/******************************************************************************
 * @brief 串口初始化
 * 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
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
            ui->ComxBox->addItem(info.portName());
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


/******************************************************************************
 * @brief 窗口关闭事件处理
 * 
 * @param event 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
void serialwindow::closeEvent(QCloseEvent *event)
{
    // 请求在事件循环的下一个迭代中删除此对象
    serial->close();
    deleteLater();
}

/******************************************************************************
 * @brief 状态灯闪烁
 * 
 * @param input 
 * @version 0.1
 * @author zager
 * @date 2024-09-25
 * @copyright Copyright (c) 2024
  ---------------------------------------------------------------------------*/
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

void serialwindow::onBTSlotFunc(void)
{
    QPushButton *clickedButton;
    clickedButton = qobject_cast<QPushButton*>(sender());
    if(clickedButton == ui->SpeedModeBT)
    {
        MC_Frame datafram;
        datafram.CMD = M_SET_SpeedM;
        datafram.UnPack();
        pMcProtocl->AddFrameToBuf(datafram);
        ui->MotorStartBT->setDisabled(false);
        ui->MotorStopBT->setDisabled(false);
    }else if(clickedButton == ui->MotorStartBT){//电机启动按键
        MC_Frame datafram;
        datafram.CMD = M_SET_START;
        datafram.UnPack();
        pMcProtocl->AddFrameToBuf(datafram);
    }else if(clickedButton == ui->CMDEnterBT){  //命令确认按键
        MC_Frame datafram;
        QString currentText = ui->CMDBox->currentText();
        if (commandMap.contains(currentText)) {
            datafram.CMD = commandMap.value(currentText);
        } else {
            qDebug() << "未知命令：" << currentText;
            return;
        }
        currentText = ui->DataBufEdit->text();
        qDebug()<<currentText;
        datafram.data = stringToUCharVectorOptimized(currentText);// 调用函数，将QString中的浮点数转换为unsigned char向量;
        datafram.UnPack();
        datafram.PrintFrame();
        pMcProtocl->AddFrameToBuf(datafram);
    }else if(clickedButton == ui->MotorStopBT){ //电机停止按键
        MC_Frame datafram;
        datafram.CMD = M_SET_STOP;
        datafram.UnPack();
        pMcProtocl->AddFrameToBuf(datafram);
    }else if(clickedButton == ui->ClearChartBT){//清除图标按键
        customPlot->graph(0)->data().data()->clear();
    }else if(clickedButton == ui->NormalModeBT){//正常模式按键
        MC_Frame datafram;
        datafram.CMD = M_SET_NormalM;
        datafram.UnPack();
        pMcProtocl->AddFrameToBuf(datafram);
        ui->MotorStartBT->setDisabled(false);
        ui->MotorStopBT->setDisabled(false);
    }else if(clickedButton == ui->PositionModeBT){//位置模式按键
        MC_Frame datafram;
        datafram.CMD = M_SET_PosM;
        datafram.UnPack();
        pMcProtocl->AddFrameToBuf(datafram);
        ui->MotorStartBT->setDisabled(false);
        ui->MotorStopBT->setDisabled(false);
    }else if(clickedButton == ui->ClearRicivBufBt){//清除接收区缓存按键
        QTextEdit *edit = ui->RiceveEdit;
        edit->clear();
    }else if(clickedButton == ui->OpenSerilBT){//打开串口按键
        OpenSerial();
    }
}

void serialwindow::ButtonInit(void)
{
    ui->MotorStartBT->setDisabled(true);
    ui->MotorStopBT->setDisabled(true);
    QObject::connect(ui->SpeedModeBT,SIGNAL(clicked()),this,SLOT(onBTSlotFunc()));
    QObject::connect(ui->MotorStartBT,SIGNAL(clicked()),this,SLOT(onBTSlotFunc()));
    QObject::connect(ui->CMDEnterBT,SIGNAL(clicked()),this,SLOT(onBTSlotFunc()));
    QObject::connect(ui->MotorStopBT,SIGNAL(clicked()),this,SLOT(onBTSlotFunc()));
    QObject::connect(ui->ClearChartBT,SIGNAL(clicked()),this,SLOT(onBTSlotFunc()));
    QObject::connect(ui->NormalModeBT,SIGNAL(clicked()),this,SLOT(onBTSlotFunc()));
    QObject::connect(ui->PositionModeBT,SIGNAL(clicked()),this,SLOT(onBTSlotFunc()));
    QObject::connect(ui->ClearRicivBufBt,SIGNAL(clicked()),this,SLOT(onBTSlotFunc()));
    QObject::connect(ui->OpenSerilBT,SIGNAL(clicked()),this,SLOT(onBTSlotFunc()));

    /*心跳灯初始化*/
    QLabel *lightLabel = ui->LED_Label;
    lightLabel->setScaledContents(true); // 根据控件大小缩放图片
    lightLabel->setFixedSize(30, 30);    // 设置控件大小为 30x30（注意这里的修改）
    lightLabel->setPixmap(QPixmap("../MCOM/images/GrapLED.png"));       // 设置初始状态为灭灯（使用加载成功的pixmap）

   /* 图表初始化 */
    WaveformGraphInit();
}
void serialwindow::OpenSerial(void)
{
        QPushButton *openbutton = ui->OpenSerilBT;
        QComboBox *portBt = ui->ComxBox;
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
                        // qDebug() << "mallocc";
                        serial = new QSerialPort; // 创建新对象
                    }
                    serial->setPort(info);
                    if (serial->open(QIODevice::ReadWrite))
                    {
                        openbutton->setText("关闭串口");
                        bool ok;
                        QString baudRateStr = ui->BaoudBox->currentText();
                        int baudRate = baudRateStr.toInt(&ok);

                        // 检查转换是否成功
                        if (ok) {
                            // 转换成功，设置波特率
                            serial->setBaudRate(baudRate);
                            // qDebug() << "Baud rate set to:" << baudRate;
                        }else{
                            serial->setBaudRate(115200);
                        }
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


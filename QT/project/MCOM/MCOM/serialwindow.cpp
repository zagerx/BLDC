#include "serialwindow.h"
#include "ui_serialwindow.h"
#include <QtDebug>
#include <QThread>
#include <QTimer>

serialwindow::serialwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::serialwindow)
{
    ui->setupUi(this);
    SerialPortInit();
    ui->mc_startBt->setDisabled(true);
    ui->mt_stopBt->setDisabled(true);
}

serialwindow::~serialwindow()
{
    /*点击退出图标  无法进入这个函数*/
    qDebug()<< "Eite SerialWindow";
    delete serial; // 确保删除 serial 指向的对象
    serial = nullptr; // 防止悬空指针
    delete ui;
}



/*
 * 串口初始化
*/
void serialwindow::SerialPortInit()
{
    serial = new(QSerialPort);
    qDebug() <<"串口初始化...";
    QSerialPort *curPort;
    curPort = new(QSerialPort);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        curPort ->setPort(info);
        if(curPort ->open(QIODevice::ReadWrite))
        {
            ui->portBox->addItem(info.portName());
            curPort ->close();
            qDebug() << "串口打开成功";
        }else{
            qDebug() << "串口打开失败，请重试";
        }
    }
    delete(curPort);

    connect(serial, &QSerialPort::readyRead, this, &serialwindow::onReadSerialData);
}
void serialwindow::closeEvent(QCloseEvent *event) {

    // 请求在事件循环的下一个迭代中删除此对象

    serial->close();
    deleteLater();
}

/*
**   数据处理
*/
void serialwindow::processdata(QByteArray data)
{
    QTextEdit *line_edit;
    line_edit = ui->textEdit;
    // 将数据转换为 QString，并将 "\r\n" 转换为 ""
    QString textData = QString::fromUtf8(data).replace("\r\n", "");

    line_edit->append(textData);

    // ui->Kc_lineEdit->setEnabled(false);

    QLabel *lightLabel;
    lightLabel = ui->LED_Label;
    lightLabel->setPixmap(QPixmap("../MCOM/images/GreenLED.png")); // 设置初始状态为灭灯

    lightLabel->setScaledContents(true); // /*根据控件大小缩放图片*/
    lightLabel->setFixedSize(50, 50); // 设置控件大小为 50x50
    /* 设置小灯 */

}
/*
 * 串口接收槽函数
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
            processdata(data);
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




void serialwindow::on_mc_startBt_clicked()
{
    QString command = "motor_start:\r\n";
    serial->write(command.toLatin1());
}

void serialwindow::on_mt_stopBt_clicked()
{
    QString command = "motor_stop:\r\n";
    serial->write(command.toLatin1());
}

#include "motordebug.h"
void serialwindow::onDataReceivedFromB(const QString &data) {
    // 在这里处理从B界面接收到的数据
    qDebug() << "Received data from B:" << data;
    // 由于B界面在此时可能已经被销毁，确保不要再访问b的指针

    QString command;// = "pid_param:"
    command += data;
    serial->write(command.toUtf8());
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


void serialwindow::on_normal_bt_clicked()
{
    /**/
    ui->mc_startBt->setDisabled(false);
    ui->mt_stopBt->setDisabled(false);

    QString command = "motor_normalmode:\r\n";
    serial->write(command.toLatin1());
}
void serialwindow::on_debug_bt_clicked()
{
    /*发送命令 使电机进入debug模式*/
    QString command = "motor_debugmode:\r\n";
    serial->write(command.toLatin1());

    motordebug *pmd = new motordebug;
    // /*connect*/(b, &B::dataReady, this, &A::onDataReceivedFromB); // 连接B界面的信号到A界面的槽
    connect(pmd,&motordebug::dataReady,this,&serialwindow::onDataReceivedFromB);

    pmd->show();
}

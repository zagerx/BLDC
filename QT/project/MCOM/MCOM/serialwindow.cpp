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

/*
**   数据处理
*/
void serialwindow::processdata(QByteArray data)
{
    QTextEdit *line_edit;
    line_edit = ui->textEdit;
    // 将数据转换为 QString，并将 "\r\n" 转换为 ""
    QString textData = QString::fromUtf8(data).replace("\r\n", "");

    /*获取Kp文本框*/
    line_edit->append(textData);

    ui->Kc_lineEdit->setEnabled(false);

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


/*
**  按键响应
*/
void serialwindow::on_pushButton_clicked()
{
    QPushButton *openbutton = ui->pushButton;
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

/*LineEdit*/
void serialwindow::on_enter_bt_clicked()
{
    // sendKpCommand();
    sendIdCommand();
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

void serialwindow::on_mc_pidset_clicked()
{
    sendKpCommand();
}

void serialwindow::on_mc_speed_Bt_clicked()
{
    QLineEdit *line_edit;
    QString pstr;
    /*获取iq文本框*/
    line_edit = ui->Speed_lineEdit;
    pstr = line_edit->text();
    if(!pstr.isEmpty())
    {
        qDebug() << "send speed";
        QString command = "mc_tar_speed:";
        command += pstr;
        serial->write(command.toLatin1());
    }
}



/*
** 普通公共函数
*/
void serialwindow::sendKpCommand()
{
    QLineEdit *line_edit;
    QString pstr;

    /*获取Kp文本框*/
    line_edit = ui->Kp_lineEdit;
    pstr = line_edit->text();
    if(!pstr.isEmpty())
    {
        qDebug() << "send kp";

        QString command = "mc_setd_kp:";
        command += pstr;
        serial->write(command.toUtf8());
    }
    QTimer::singleShot(10, this, SLOT(sendKiCommand()));
}

void serialwindow::sendKiCommand()
{
    QLineEdit *line_edit;
    QString pstr;
    /*获取Ki文本框*/
    line_edit = ui->Ki_lineEdit;
    pstr = line_edit->text();
    if(!pstr.isEmpty())
    {
        qDebug() << "send ki";
        QString command = "mc_setd_ki:";
        command += pstr;
        serial->write(command.toLatin1());
    }
    QTimer::singleShot(10, this, SLOT(sendKcCommand()));
}

void serialwindow::sendKcCommand()
{
    QLineEdit *line_edit;
    QString pstr;
    /*获取Kc文本框*/
    line_edit = ui->Kc_lineEdit;
    pstr = line_edit->text();
    if(!pstr.isEmpty())
    {
        QString command = "mc_setd_ki:";
        command += pstr;
        serial->write(command.toLatin1());
    }

    QTimer::singleShot(10, this, SLOT(sendParamEnter()));
}

void serialwindow::sendParamEnter()
{
    QString command = "mc_pid_paraset:";
    serial->write(command.toLatin1());
}

void serialwindow::sendIdCommand()
{
    QLineEdit *line_edit;
    QString pstr;
    /*获取id文本框*/
    line_edit = ui->id_lineEdit;
    pstr = line_edit->text();
    if(!pstr.isEmpty())
    {
        qDebug() << "send id";
        QString command = "mc_tar_id:";
        command += pstr;
        serial->write(command.toLatin1());
    }
    QTimer::singleShot(10, this, SLOT(sendIqCommand()));
}

void serialwindow::sendIqCommand()
{
    QLineEdit *line_edit;
    QString pstr;
    /*获取iq文本框*/
    line_edit = ui->iq_lineEdit;
    pstr = line_edit->text();
    if(!pstr.isEmpty())
    {
        qDebug() << "send iq";
        QString command = "mc_tar_iq:";
        command += pstr;
        serial->write(command.toLatin1());
    }
}



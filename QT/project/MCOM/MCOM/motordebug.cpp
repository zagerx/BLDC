#include "motordebug.h"
#include "ui_motordebug.h"
#include <QtDebug>

motordebug::motordebug(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::motordebug)
{
    qDebug() << "motordebug window open\r\n";

    ui->setupUi(this);

    QPushButton *pStartBt = ui->start_bt;
    QPushButton *pStopBt = ui->stop_bt;
    pStartBt->setEnabled(false);
    pStopBt->setEnabled(false);

    QLineEdit *pKpLE = ui->Kp_lineEdit;
    QLineEdit *pKiLE = ui->Ki_lineEdit;
    QLineEdit *pKcLE = ui->Kc_lineEdit;
    QLineEdit *ptar =  ui->tarlineEdit;
    pKpLE->setText("0.0");
    pKiLE->setText("0.0");
    pKcLE->setText("0.0");
    ptar->setText("0.0");
}

motordebug::~motordebug()
{
    qDebug() << "motordebug window close\r\n";
    delete ui;
}

void motordebug::closeEvent(QCloseEvent *event) {

    // 请求在事件循环的下一个迭代中删除此对象
    deleteLater();
}

void motordebug::on_paramenterBT_clicked()
{
    QLineEdit *pKpLE = ui->Kp_lineEdit;
    QLineEdit *pKiLE = ui->Ki_lineEdit;
    QLineEdit *pKcLE = ui->Kc_lineEdit;
    /*获取文本框数据*/
    QString strdata;

    if(pKpLE->text().isEmpty())
    {
        pKpLE->setText("0.0");
    }

    QString temdata = pKpLE->text() ;/*pKiLE->text();*/
    temdata += ",";
    if(pKiLE->text().isEmpty())
    {
        pKiLE->setText("0.0");
    }
    temdata += pKiLE->text();
    temdata += ",";
    if(pKcLE->text().isEmpty())
    {
        pKcLE->setText("0.0");
    }
    temdata += pKcLE->text();

    strdata = temdata;
    qDebug() << strdata;

    /*全部发送*/
    emit dataReady(strdata); // 发射信号，传递数据

    QPushButton *pStartBt = ui->start_bt;
    QPushButton *pStopBt = ui->stop_bt;
    pStartBt->setEnabled(true);
    pStopBt->setEnabled(true);
}









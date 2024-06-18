#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_serialButton_clicked()
{
    /*创建一个串口窗口*/
    serialwindow *pserialwind = new serialwindow;
    pserialwind->show();
}




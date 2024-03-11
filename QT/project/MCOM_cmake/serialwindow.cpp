#include "serialwindow.h"
#include "ui_serialwindow.h"
#include <QSerialPort>

SerialWindow::SerialWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SerialWindow)
{
    ui->setupUi(this);
}

SerialWindow::~SerialWindow()
{
    delete ui;
}

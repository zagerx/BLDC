#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "serialwindow.h""

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

void MainWindow::on_SerialButton_clicked()
{
    SerialWindow *pserialwind = new SerialWindow;
    pserialwind->show();
}


#include "pidwindow.h"
#include "ui_pidwindow.h"

pidwindow::pidwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pidwindow)
{
    ui->setupUi(this);
}

pidwindow::~pidwindow()
{
    delete ui;
}

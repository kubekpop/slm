#include "status_window.h"
#include "ui_status_window.h"

status_window::status_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::status_window)
{
    ui->setupUi(this);
}

status_window::~status_window()
{
    delete ui;
}
/*
 Target functionality:
 monitor free ram
 monitor cpu usage
 disk usage
 raid health check




  */

#include "dhcp_window.h"
#include "ui_dhcp_window.h"

dhcp_window::dhcp_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dhcp_window)
{
    ui->setupUi(this);
}

dhcp_window::~dhcp_window()
{
    delete ui;
}


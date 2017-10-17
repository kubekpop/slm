#include "firewall_window.h"
#include "ui_firewall_window.h"

firewall_window::firewall_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::firewall_window)
{
    ui->setupUi(this);
}

firewall_window::~firewall_window()
{
    delete ui;
}

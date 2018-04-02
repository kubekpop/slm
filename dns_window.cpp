#include "dns_window.h"
#include "ui_dns_window.h"

dns_window::dns_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dns_window)
{
    ui->setupUi(this);
}

dns_window::~dns_window()
{
    delete ui;
}


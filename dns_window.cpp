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

void dns_window::on_next_clicked()
{
    int current_index = ui->stacked->currentIndex();
    if(current_index < 2)
    {
        current_index++;
        ui->stacked->setCurrentIndex(current_index);
    }
}

void dns_window::on_back_clicked()
{
    int current_index = ui->stacked->currentIndex();
    if(current_index > 0)
    {
        current_index--;
        ui->stacked->setCurrentIndex(current_index);
    }
}

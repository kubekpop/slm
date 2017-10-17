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

void dhcp_window::on_next_clicked()
{
    int current_index = ui->content->currentIndex();
    if(current_index < 2)
    {
        current_index++;
        ui->content->setCurrentIndex(current_index);
    }
}

void dhcp_window::on_back_clicked()
{
    int current_index = ui->content->currentIndex();
    if(current_index > 0)
    {
        current_index--;
        ui->content->setCurrentIndex(current_index);
    }
}

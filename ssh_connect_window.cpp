#include "ssh_connect_window.h"
#include "ui_ssh_connect_window.h"

ssh_connect_window::ssh_connect_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ssh_connect_window)
{
    ui->setupUi(this);
}

ssh_connect_window::~ssh_connect_window()
{
    delete ui;
}

void ssh_connect_window::bash_output_interpreter(QString output)
{
    if(output.startsWith("["))
    {
        QString tag = output.left(7);
        tag.replace("[","");
        tag.replace("]","");
        int tag_number = tag.toInt();
        output.remove(0, 7);
        switch(tag_number)
        {
        default:
            break;
        }
    }
}

void ssh_connect_window::on_connect_clicked()
{
    this->close();
    QString IP = "127.0.0.1";
    QString port = "22";
    if(ui->server_ip_radio->isChecked())
    {
    IP = ui->IP_1->text()+"."+ui->IP_2->text()+"."+ui->IP_3->text()+"."+ui->IP_4->text();
    port = ui->port->text();
    }
    if(ui->hostname_radio->isChecked())
    {
        IP = ui->hostname->text();
        port = ui->hostame_port->text();
    }
    QString password = ui->password->text();
    emit connect_ssh(IP, port, password);
}

void ssh_connect_window::closeEvent(QCloseEvent *event)
{
    emit closed();
    event->accept();
}

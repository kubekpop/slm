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

void ssh_connect_window::on_connect_clicked()
{
    this->close();
    QString IP = ui->IP_1->text()+"."+ui->IP_2->text()+"."+ui->IP_3->text()+"."+ui->IP_4->text();
    QString password = ui->password->text();
    emit connect_ssh(IP, password);
}

void ssh_connect_window::closeEvent(QCloseEvent *event)
{
    emit closed();
    event->accept();
}

#include "firewall_window.h"
#include "ui_firewall_window.h"
//ls -A1 /sys/class/net | sed ':a;N;$!ba;s/\n/=/g'


firewall_window::firewall_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::firewall_window)
{
    ui->setupUi(this);


}
void firewall_window::firewall_prepare_window(QString ifaces)
{
    ui->source_interface->clear();
    ui->destination_interface->clear();
    ui->local_interface->clear();
    ui->internet_interface->clear();
    ui->allowed_interface_services->clear();
    ui->custom_port_interface->clear();

    QStringList interfaces = ifaces.split("-separate-");
    interfaces.prepend("");
    foreach(QString iface, interfaces)
    {
        iface.replace("\n","");
        iface.replace(" ","");
        ui->source_interface->addItem(iface);
        ui->destination_interface->addItem(iface);
        ui->local_interface->addItem(iface);
        ui->internet_interface->addItem(iface);
        ui->allowed_interface_services->addItem(iface);
        ui->custom_port_interface->addItem(iface);
    }
}
/*
firewall_window::dnat()
{
    bash_root->write();
    emit data_to_log("komenda");
}
*/
firewall_window::~firewall_window()
{
    delete ui;
}



void firewall_window::on_dnat_apply_clicked()
{
   emit data_to_log("komenda");
}

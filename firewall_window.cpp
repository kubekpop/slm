#include "firewall_window.h"
#include "ui_firewall_window.h"
//ls -A1 /sys/class/net | sed ':a;N;$!ba;s/\n/-/g'


firewall_window::firewall_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::firewall_window)
{
    ui->setupUi(this);

    QString ifaces = "any=eth0=eth2=eth3";
    QStringList interfaces = ifaces.split("=");
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
void firewall_window::firewall_prepare_window()
{


}

firewall_window::~firewall_window()
{
    delete ui;
}



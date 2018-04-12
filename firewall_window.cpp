#include "firewall_window.h"
#include "ui_firewall_window.h"
//ls -A1 /sys/class/net | sed ':a;N;$!ba;s/\n/=/g'


firewall_window::firewall_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::firewall_window)
{
    ui->setupUi(this);


}

void firewall_window::bash_output_interpreter(QString output)
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
        case 1:
            this->firewall_prepare_window(output);//gets interfaces and opens firewall
            this->show();
            break;
        case 5:
            emit data_to_log(output);  //do ostylowania
        default:
            break;
        }
    }
}

void firewall_window::firewall_prepare_window(QString ifaces)
{
    //ui->stackedWidget->setCurrentIndex(0);
    ui->source_interface->clear();
    ui->destination_interface->clear();
    ui->local_interface->clear();
    ui->internet_interface->clear();
    ui->allowed_interface_services->clear();
    ui->custom_port_interface->clear();

    QStringList interfaces = ifaces.split("-separate-");
    interfaces.prepend("");
    ui->source_interface->addItem("*");
    ui->destination_interface->addItem("*");
    ui->custom_port_interface->addItem("*");
    ui->allowed_interface_services->addItem("*");
    foreach(QString iface, interfaces)
    {
        iface.replace("\n","");
        iface.replace(" ","");
        if(!iface.isEmpty())
        {
            ui->source_interface->addItem(iface);
            ui->destination_interface->addItem(iface);
            ui->local_interface->addItem(iface);
            ui->internet_interface->addItem(iface);
            ui->allowed_interface_services->addItem(iface);
            ui->custom_port_interface->addItem(iface);
        }
    }
}

firewall_window::~firewall_window()
{
    delete ui;
}



void firewall_window::on_dnat_apply_clicked()
{
    //czydziala?
            QString source_ip = ui->source_ip->text();//Public Interface IP
            QString source_port = ui->source_port->text();//Ports To Forward
            QString source_interface = ui->source_interface->currentText();//Public Interface
            QString destination_ip = ui->destination_ip->text();//Local Server IP (destination)
            QString destination_port = ui->destination_port->text();//Local Port (destination)
            QString destination_interface = ui->destination_interface->currentText();//Local Interface (connecting to LAN)
            //QString command_dnat = "iptables -t nat -A PREROUTING -i "+source_interface+" -p tcp --source_port "+source_port+" -d "+source_ip+" --destination_port "+destination_port+" -j DNAT --to-destination "+destination_ip;
            QString command_dnat = "sysctl -w net.ipv4.ip_forward=1; ";
            //command_dnat += ";<br />";
            command_dnat += "iptables -t nat -A PREROUTING ";
            if(source_ip != "")
            {
                command_dnat += "-d "+source_ip+" ";
            }
            if(source_interface != "*")
            {
                command_dnat += "-i "+source_interface+" ";
            }
            if(source_port != "")
            {
                command_dnat += "-p tcp --dport "+source_port+" ";
            }

            command_dnat += "-j DNAT --to-destination "+destination_ip+":"+destination_port+"; ";
           /*
            if(source_interface != "")
            {
                command_dnat += "iptables -t nat -A PREROUTING -i "+source_interface+" -p tcp --dport "+source_port+" -j DNAT --to-destination "+destination_ip+":"+destination_port;//main DNAT rule
            }
            else
            {
                command_dnat += "iptables -t nat -A PREROUTING -p tcp --dport "+source_port+" -j DNAT --to-destination "+destination_ip+":"+destination_port;//main DNAT rule
            }
            */
            command_dnat += "iptables -A FORWARD ";
            if(source_interface != "*")
            {
                command_dnat += "-i "+source_interface+" ";
            }
            if(destination_interface != "*")
            {
                command_dnat += "-o "+destination_interface+" ";
            }
            command_dnat += "-p tcp --sport "+source_port+" -d "+destination_ip+" --dport "+destination_port+" -m state --state NEW -j ACCEPT; ";
            command_dnat += "iptables -A FORWARD ";
            if(destination_interface != "*")
            {
                command_dnat += "-i "+destination_interface+" ";
            }
            if(source_interface != "*")
            {
                command_dnat += "-o "+source_interface+" ";
            }
            command_dnat += " -m state --state ESTABLISHED,RELATED -j ACCEPT; ";

            command_dnat += "iptables -A FORWARD ";
            if(destination_interface != "*")
            {
                command_dnat += "-o "+destination_interface+" ";
            }
            if(source_interface != "*")
            {
                command_dnat += "-i "+source_interface+" ";
            }
            command_dnat += " -m state --state ESTABLISHED,RELATED -j ACCEPT; ";

            ask(command_dnat, "fire","00002");
}

void firewall_window::on_routing_apply_clicked()
{

    //localinterface, internet -> interfejsy

            QString command_routing = "iptables -t nat -A POSTROUTING -o "+ui->internet_interface->currentText()+" -j MASQUERADE; sysctl -w net.ipv4.ip_forward=1; ";
            command_routing += "iptables -A FORWARD -o "+ui->local_interface->currentText()+" -m state --state ESTABLISHED,RELATED -j ACCEPT; ";

            command_routing += "iptables -A FORWARD -o "+ui->internet_interface->currentText()+" -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT; ";

    if(ui->public_ip_checkbox->isChecked())
            {
                command_routing += "iptables -A INPUT -i "+ui->internet_interface->currentText()+" -m state --state NEW -j ACCEPT; ";//jesli ma być dostęp z zewnątrz, ale określić trzeba by interfejs i co to w sumie ma do maskarady, bez sensu zupełnie
                //dodać interfejs wejściowy
            }
            command_routing += "iptables -A FORWARD -i "+ui->internet_interface->currentText()+" -o "+ui->internet_interface->currentText()+" -m state --state NEW -j REJECT; ";

            ask(command_routing, "fire","00003");
}

void firewall_window::on_custom_port_apply_clicked()
{
    QString custom_port_command = "iptables -A INPUT";
    if(ui->custom_port_interface->currentText() == "*")
    {

    }
    else
    {
        custom_port_command += " -i " + ui->custom_port_interface->currentText();
    }

     if(ui->protocol_combo_box->currentText() == "TCP/UDP")
     {
         custom_port_command += " -p all";
     }
     else
     {
         custom_port_command += " -p " + ui->protocol_combo_box->currentText().toLower();
     }

   custom_port_command += " --dport " + ui->custom_port_value->text() + " -j ACCEPT";
   ask(custom_port_command, "fire","00004");
}


void firewall_window::ask(QString command, QString tag, QString tag_number)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this,tr("Are you sure?"), command ,QMessageBox::No | QMessageBox::Yes);
    if(reply == QMessageBox::Yes)
    {
        command.prepend("echo '["+ tag +"]["+ tag_number +"]'`");
        command.append("`'[XXXXX]' \n");
        //bash_root->write(command.toStdString().c_str());
    }
}

void firewall_window::on_list_rules_ok_clicked()
{
    QString list_command = "echo '[fire][00005]'`iptables -L " + ui->list_rules_chain->currentText() + "`'[XXXXX]' \n";
    bash_root->write(list_command.toStdString().c_str());
}

void firewall_window::on_allowed_services_ok_clicked()
{
    QString services_command_body;
    int counter = 0;
    if(ui->sql_check->isChecked())
    {
        counter++;
    }
    if(ui->http_check->isChecked())
    {
        counter++;
    }
    if(ui->https_check->isChecked())
    {
        counter++;
    }
    if(ui->ftp_check->isChecked())
    {
        counter++;
    }
    if(counter == 0)
    {

    }
    else if(counter == 1)
    {
        services_command_body += " -p tcp -m tcp --dport ";
        if(ui->sql_check->isChecked())
        {
            services_command_body += "3389";
        }
        if(ui->http_check->isChecked())
        {
            services_command_body += "80";
        }
        if(ui->https_check->isChecked())
        {
            services_command_body += "443";
        }
        if(ui->ftp_check->isChecked())
        {
            services_command_body += "21";
        }
    }
    else
    {
        services_command_body += " -p tcp -m multiport --dports ";
        QString ports;
        if(ui->sql_check->isChecked())
        {
            ports += "3389";
        }
        if(ui->http_check->isChecked())
        {
            ports+= ",80";
        }
        if(ui->https_check->isChecked())
        {
            ports += ",443";
        }
        if(ui->ftp_check->isChecked())
        {
            ports += ",21";
        }
        if(ports.startsWith(","))
        {
            ports.remove(0,1);
        }
        if(ports.endsWith(","))
        {
            ports.chop(1);
        }
        services_command_body += ports;
    }
    services_command_body += " -j ACCEPT";
    QString services_command_full = "iptables -A INPUT" + services_command_body;
    if(ui->add_output_rule->isChecked())
    {
        services_command_full += "; iptables -A OUTPUT" + services_command_body;
    }
    ask(services_command_full, "fire","00006");
}

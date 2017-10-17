#include "main_widget.h"
#include "ui_main_widget.h"
#include "QMessageBox"
#include "QTime"
main_widget::main_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::main_widget)
{
    ui->setupUi(this);
}

main_widget::~main_widget()
{
    delete ui;
}

void main_widget::closeEvent(QCloseEvent *event)
{
    event->accept();
    /*
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Sure?", "Exit XUMPP Lite?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
    */
    bash_root->close();
}

void main_widget::startup()
{
    // setup bash for authorisation
    bash_root = new QProcess(this);
    bash_root->start("/bin/bash");
    connect(bash_root, SIGNAL(readyRead()), this, SLOT(bash_root_reader()));
    // todo: read chosen distribution from settings
    QString distribution = "fedora";

    ui->log->append("SLM dev.");
    ui->log->append("Distribution: "+distribution);
    ui->log->append("Current kernel: 4.13.4-200.fc26.x86_64");
    ui->log->append(QTime::currentTime().toString()+" Loading modules...");




    if(distribution == "suse")
    {
        service_names.append("apache2");
        pids_names.append("apache2");

        service_names.append("mysql");
        pids_names.append("mysql");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");

        service_names.append("smbd");
        pids_names.append("smbd");

        service_names.append("dhcpd");
        pids_names.append("dhcpd");

        service_names.append("nfs-server");
        pids_names.append("nfsd");
    }
    else if(distribution == "fedora")
    {
        service_names.append("httpd");
        pids_names.append("httpd");

        service_names.append("mysqld");
        pids_names.append("mysql");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");

        service_names.append("smbd");
        pids_names.append("smbd");

        service_names.append("dhcpd");
        pids_names.append("dhcpd");

        service_names.append("nfs-server");
        pids_names.append("nfsd");




    }
    else if(distribution == "ubuntu")
    {
        service_names.append("apache2");
        pids_names.append("apache2");

        service_names.append("mysqld");
        pids_names.append("mysql");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");

        service_names.append("smbd");
        pids_names.append("smbd");

        service_names.append("isc-dhcp-server");
        pids_names.append("dhcpd");

        service_names.append("nfs-server");
        pids_names.append("nfsd");
    }
    // update information about services

    for(int i = 0; i < service_names.count(); i++)
    {
        pid_check(pids_names[i],i);
        status_check(service_names[i],i);
        ui->log->append(QTime::currentTime().toString()+" Done checks for module: "+service_names[i]);

    }
    ui->log->append(QTime::currentTime().toString()+" Done module checking");
    //ui->log->append(QTime::currentTime().toString());
    is_authorised = false;
    lock_interface(true);
}

void main_widget::root_setup()
{
    for(int i = 0; i < service_names.count(); i++)
    {
        pid_check(pids_names[i],i);
        port_check(service_names[i],i);
        status_check(service_names[i],i);
    }
}


void main_widget::on_authorize_clicked()
{

    bash_root->write("pkexec su \n");
    bash_root->waitForBytesWritten();
    bash_root->write("echo '[00000]'`whoami`'[XXXXX]' \n");
}

void main_widget::authorisation(QString user)
{
    if(user == "root")
    {
        qInfo()<< "Autoryzacja zakonczona..."<<user;
        ui->log->append(QTime::currentTime().toString()+" Authorization successful");

        is_authorised = true;
        lock_interface(false);
        root_setup();
        ui->authorize->setEnabled(false);
    }
    else
    {
        qInfo()<< "Autoryzacja nieudana..."<<user;
        ui->log->append(QTime::currentTime().toString()+" Authorization failed");

        lock_interface(true);
        //interfejs zablokowany
    }
}

void main_widget::lock_interface(bool interface_locked)
{
        //lock intreface
        ui->apache_config->setDisabled(interface_locked);
        ui->mysql_config->setDisabled(interface_locked);
        ui->samba_config->setDisabled(interface_locked);
        ui->nfs_config->setDisabled(interface_locked);
        ui->dhcp_config->setDisabled(interface_locked);
        ui->ftp_config->setDisabled(interface_locked);

        ui->apache_start->setDisabled(interface_locked);
        ui->mysql_start->setDisabled(interface_locked);
        ui->samba_start->setDisabled(interface_locked);
        ui->nfs_start->setDisabled(interface_locked);
        ui->dhcp_start->setDisabled(interface_locked);
        ui->ftp_start->setDisabled(interface_locked);

        ui->apache_stop->setDisabled(interface_locked);
        ui->mysql_stop->setDisabled(interface_locked);
        ui->samba_stop->setDisabled(interface_locked);
        ui->nfs_stop->setDisabled(interface_locked);
        ui->dhcp_stop->setDisabled(interface_locked);
        ui->ftp_stop->setDisabled(interface_locked);

        ui->apache_restart->setDisabled(interface_locked);
        ui->mysql_restart->setDisabled(interface_locked);
        ui->samba_restart->setDisabled(interface_locked);
        ui->nfs_restart->setDisabled(interface_locked);
        ui->dhcp_restart->setDisabled(interface_locked);
        ui->ftp_restart->setDisabled(interface_locked);

        ui->iptables->setDisabled(interface_locked);
}

void main_widget::status_check(QString service_name, int module_number)
{
    QString check_command;
    switch(module_number)
    {
    case 0:
        check_command = "echo '[00001]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 1:
        check_command = "echo '[00002]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 2:
        check_command = "echo '[00003]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 3:
        check_command = "echo '[00004]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 4:
        check_command = "echo '[00005]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 5:
        check_command = "echo '[00011]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') || $(systemctl status $service | egrep '^.*active.*(exited).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;
    }
    bash_root->write(check_command.toStdString().c_str());
    // jump to function bash_root_reader();
}

void main_widget::pid_check(QString service_name, int module_number)
{
    QString check_command;
    switch(module_number)
    {
    case 0:
        check_command = "echo '[00006]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 1:
        check_command = "echo '[00007]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 2:
        check_command = "echo '[00008]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 3:
        check_command = "echo '[00009]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 4:
        check_command = "echo '[00010]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 5:
        check_command = "echo '[00012]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;
    }
    bash_root->write(check_command.toStdString().c_str());
    // jump to function bash_root_reader();
}

void main_widget::port_check(QString service_name, int module_number)
{
    QString check_command;
    switch(module_number)
    {
    case 0:
        check_command = "echo '[00013]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;

    case 1:
        check_command = "echo '[00014]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;

    case 2:
        check_command = "echo '[00015]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;

    case 3:
        check_command = "echo '[00016]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;

    case 4:
        check_command = "echo '[00017]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;
    case 5:
        check_command = "echo '[00018]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;
    }
    bash_root->write(check_command.toStdString().c_str());
    // jump to function bash_root_reader();
}


void main_widget::bash_root_reader()
{
    QString output_from_bash = bash_root->readAllStandardOutput();
    //output_from_bash.chop(1);
    output_from_bash.replace("\n","");
    bash_output_processor(output_from_bash);
    // todo: output processing in another function
}

void main_widget::bash_output_processor(QString output_from_bash)
{
    QStringList output_separated = output_from_bash.split("[XXXXX]");
    foreach (QString output, output_separated)
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
        case 0:
            authorisation(output);
            break;
        case 1:
            set_status(output, 0);
            break;
        case 2:
            set_status(output, 1);
            break;
        case 3:
            set_status(output, 2);
            break;
        case 4:
            set_status(output, 3);
            break;
        case 5:
            set_status(output, 4);
            break;
        case 6:
            set_pids(output, 0);
            break;
        case 7:
            set_pids(output, 1);
            break;
        case 8:
            set_pids(output, 2);
            break;
        case 9:
            set_pids(output, 3);
            break;
        case 10:
            set_pids(output, 4);
            break;
        case 11:
            set_status(output, 5);
            break;
        case 12:
            set_pids(output, 5);
            break;
        case 13:
            set_port(output,0);
            break;
        case 14:
            set_port(output,1);
            break;
        case 15:
            set_port(output,2);
            break;
        case 16:
            set_port(output,3);
            break;
        case 17:
            set_port(output,4);
            break;
        case 18:
            set_port(output,5);
            break;
        case 19:
            pid_check(pids_names[0],0);
            port_check(service_names[0],0);
            status_check(service_names[0],0);
            break;
        case 20:
            pid_check(pids_names[0],0);
            port_check(service_names[0],0);
            status_check(service_names[0],0);
            break;
        case 21:
            pid_check(pids_names[0],0);
            port_check(service_names[0],0);
            status_check(service_names[0],0);
            break;
        case 22:
            pid_check(pids_names[5],5);
            port_check(service_names[5],5);
            status_check(service_names[5],5);
            break;
        case 23:
            pid_check(pids_names[5],5);
            port_check(service_names[5],5);
            status_check(service_names[5],5);
            break;
        case 24:
            pid_check(pids_names[5],5);
            port_check(service_names[5],5);
            status_check(service_names[5],5);
            break;
        case 25:
            pid_check(pids_names[2],2);
            port_check(service_names[2],2);
            status_check(service_names[2],2);
            break;
        case 26:
            pid_check(pids_names[2],2);
            port_check(service_names[2],2);
            status_check(service_names[2],2);
            break;
        case 27:
            pid_check(pids_names[2],2);
            port_check(service_names[2],2);
            status_check(service_names[2],2);
            break;
        case 28:
            ui->log->setText(output);
            break;
        default:
            // todo: error
            break;
        }
        }
    }
}


void main_widget::set_status(QString status, int module_number)
{
    QString status_style_sheet = "background-color: rgb(0, 170, 255); border: 1px solid rgb(0, 170, 255); border-radius: 5px;";
    if(status == "Running")
    {
        status_style_sheet = "background-color: rgb(85, 255, 127); border: 1px solid rgb(85, 255, 127); border-radius: 5px;";
    }
    else if(status == "Not running")
    {
        status_style_sheet = "background-color: rgb(176, 176, 176); border: 1px solid rgb(176, 176, 176);; border-radius: 5px;";
    }
    else if(status == "Not installed")
    {
        status_style_sheet = "background-color: rgb(0, 170, 255); border: 1px solid rgb(0, 170, 255); border-radius: 5px;";
    }
    else if(status == "Failed")
    {
        status_style_sheet = "background-color: rgb(255,0,0); border: 1px solid rgb(255,0,0); border-radius: 5px;";
    }
    switch(module_number)
    {
    case 0:
        ui->apache_status->setText(status);
        ui->apache_status->setStyleSheet(status_style_sheet);
        if(status == "Not installed")
        {
            ui->apache_start->setEnabled(false);
            ui->apache_stop->setEnabled(false);
            ui->apache_restart->setEnabled(false);
            ui->apache_config->setEnabled(false);
            ui->apache_port->setText("N/A");
            ui->apache_pids->setText("N/A");
        }
        if(is_authorised == true)
        {
            if(status == "Running")
            {
                ui->apache_start->setEnabled(false);
                ui->apache_stop->setEnabled(true);
                ui->apache_restart->setEnabled(true);
                ui->apache_config->setEnabled(true);
            }
            if(status == "Not running")
            {
                ui->apache_start->setEnabled(true);
                ui->apache_stop->setEnabled(false);
                ui->apache_restart->setEnabled(false);
                ui->apache_config->setEnabled(true);
            }
            if(status == "Failed")
            {
                ui->apache_start->setEnabled(true);
                ui->apache_stop->setEnabled(false);
                ui->apache_restart->setEnabled(false);
                ui->apache_config->setEnabled(true);
            }
        }
        break;
    case 1:
        ui->mysql_status->setText(status);
        ui->mysql_status->setStyleSheet(status_style_sheet);
        if(status == "Not installed")
        {
            ui->mysql_start->setEnabled(false);
            ui->mysql_stop->setEnabled(false);
            ui->mysql_restart->setEnabled(false);
            ui->mysql_config->setEnabled(false);
            ui->mysql_port->setText("N/A");
            ui->mysql_pids->setText("N/A");
        }
        if(is_authorised == true)
        {
            if(status == "Running")
            {
                ui->mysql_start->setEnabled(false);
                ui->mysql_stop->setEnabled(true);
                ui->mysql_restart->setEnabled(true);
                ui->mysql_config->setEnabled(true);
            }
            if(status == "Not running")
            {
                ui->mysql_start->setEnabled(true);
                ui->mysql_stop->setEnabled(false);
                ui->mysql_restart->setEnabled(false);
                ui->mysql_config->setEnabled(true);
            }
            if(status == "Failed")
            {
                ui->mysql_start->setEnabled(true);
                ui->mysql_stop->setEnabled(false);
                ui->mysql_restart->setEnabled(false);
                ui->mysql_config->setEnabled(true);
            }
        }
        break;
    case 2:
        ui->ftp_status->setText(status);
        ui->ftp_status->setStyleSheet(status_style_sheet);
        if(status == "Not installed")
        {
            ui->ftp_start->setEnabled(false);
            ui->ftp_stop->setEnabled(false);
            ui->ftp_restart->setEnabled(false);
            ui->ftp_config->setEnabled(false);
            ui->ftp_port->setText("N/A");
            ui->ftp_pids->setText("N/A");
        }
        if(is_authorised == true)
        {
            if(status == "Running")
            {
                ui->ftp_start->setEnabled(false);
                ui->ftp_stop->setEnabled(true);
                ui->ftp_restart->setEnabled(true);
                ui->ftp_config->setEnabled(true);
            }
            if(status == "Not running")
            {
                ui->ftp_start->setEnabled(true);
                ui->ftp_stop->setEnabled(false);
                ui->ftp_restart->setEnabled(false);
                ui->ftp_config->setEnabled(true);
            }
            if(status == "Failed")
            {
                ui->ftp_start->setEnabled(true);
                ui->ftp_stop->setEnabled(false);
                ui->ftp_restart->setEnabled(false);
                ui->ftp_config->setEnabled(true);
            }
        }
        break;
    case 3:
        ui->samba_status->setText(status);
        ui->samba_status->setStyleSheet(status_style_sheet);
        if(status == "Not installed")
        {
            ui->samba_start->setEnabled(false);
            ui->samba_stop->setEnabled(false);
            ui->samba_restart->setEnabled(false);
            ui->samba_config->setEnabled(false);
            ui->samba_port->setText("N/A");
            ui->samba_pids->setText("N/A");
        }
        if(is_authorised == true)
        {
            if(status == "Running")
            {
                ui->samba_start->setEnabled(false);
                ui->samba_stop->setEnabled(true);
                ui->samba_restart->setEnabled(true);
                ui->samba_config->setEnabled(true);
            }
            if(status == "Not running")
            {
                ui->samba_start->setEnabled(true);
                ui->samba_stop->setEnabled(false);
                ui->samba_restart->setEnabled(false);
                ui->samba_config->setEnabled(true);
            }
            if(status == "Failed")
            {
                ui->samba_start->setEnabled(true);
                ui->samba_stop->setEnabled(false);
                ui->samba_restart->setEnabled(false);
                ui->samba_config->setEnabled(true);
            }
        }
        break;
    case 4:
        ui->dhcp_status->setText(status);
        ui->dhcp_status->setStyleSheet(status_style_sheet);
        if(status == "Not installed")
        {
            ui->dhcp_start->setEnabled(false);
            ui->dhcp_stop->setEnabled(false);
            ui->dhcp_restart->setEnabled(false);
            ui->dhcp_config->setEnabled(false);
            ui->dhcp_port->setText("N/A");
            ui->dhcp_pids->setText("N/A");
        }
        if(is_authorised == true)
        {
            if(status == "Running")
            {
                ui->dhcp_start->setEnabled(false);
                ui->dhcp_stop->setEnabled(true);
                ui->dhcp_restart->setEnabled(true);
                ui->dhcp_config->setEnabled(true);
            }
            if(status == "Not running")
            {
                ui->dhcp_start->setEnabled(true);
                ui->dhcp_stop->setEnabled(false);
                ui->dhcp_restart->setEnabled(false);
                ui->dhcp_config->setEnabled(true);
            }
            if(status == "Failed")
            {
                ui->dhcp_start->setEnabled(true);
                ui->dhcp_stop->setEnabled(false);
                ui->dhcp_restart->setEnabled(false);
                ui->dhcp_config->setEnabled(true);
            }
        }
        break;
    case 5:
        ui->nfs_status->setText(status);
        ui->nfs_status->setStyleSheet(status_style_sheet);
        if(status == "Not installed")
        {
            ui->nfs_start->setEnabled(false);
            ui->nfs_stop->setEnabled(false);
            ui->nfs_restart->setEnabled(false);
            ui->nfs_config->setEnabled(false);
            ui->nfs_port->setText("N/A");
            ui->nfs_pids->setText("N/A");
        }
        if(is_authorised == true)
        {
            if(status == "Running")
            {
                ui->nfs_start->setEnabled(false);
                ui->nfs_stop->setEnabled(true);
                ui->nfs_restart->setEnabled(true);
                ui->nfs_config->setEnabled(true);
            }
            if(status == "Not running")
            {
                ui->nfs_start->setEnabled(true);
                ui->nfs_stop->setEnabled(false);
                ui->nfs_restart->setEnabled(false);
                ui->nfs_config->setEnabled(true);
            }
            if(status == "Failed")
            {
                ui->nfs_start->setEnabled(true);
                ui->nfs_stop->setEnabled(false);
                ui->nfs_restart->setEnabled(false);
                ui->nfs_config->setEnabled(true);
            }
        }
        break;
    default:
        break;

    }
}

void main_widget::set_pids(QString pids, int module_number)
{
    if(pids == "")
    {
        pids = "N/A";
    }
    switch(module_number)
    {
    case 0:
        ui->apache_pids->setText(pids);
        break;
    case 1:
        ui->mysql_pids->setText(pids);
        break;
    case 2:
        ui->ftp_pids->setText(pids);
        break;
    case 3:
        ui->samba_pids->setText(pids);
        break;
    case 4:
        ui->dhcp_pids->setText(pids);
        break;
    case 5:
        ui->nfs_pids->setText(pids);
        break;
    default:
        break;

    }
}

void main_widget::set_port(QString port, int module_number)
{
    if(port == "")
    {
        port = "N/A";
    }
    switch(module_number)
    {
    case 0:
        ui->apache_port->setText(port);
        break;
    case 1:
        ui->mysql_port->setText(port);
        break;
    case 2:
        ui->ftp_port->setText(port);
        break;
    case 3:
        ui->samba_port->setText(port);
        break;
    case 4:
        ui->dhcp_port->setText(port);
        break;
    case 5:
        ui->nfs_port->setText(port);
        break;
    default:
        break;

    }
}


void main_widget::on_iptables_clicked()
{

}

void main_widget::on_shell_clicked()
{

}

void main_widget::on_config_clicked()
{
    settings_window *settings = new settings_window();
    settings->bash_root = bash_root;
	setttings->is_authorised = is_authorised;
	settings->settings_prepare_window();
    settings->show();
}

void main_widget::on_info_clicked()
{
    QMessageBox::information(this,tr("Information"),tr("Simple Linux Manager is 100% vegan <br/>No Tux has ever suffered while we got it built.<br/>More info, downloads and source code at <a href=\"https://slm.tuxdev.com\">slm.tuxdev.com</a> <br/><br/>Created by Kubix & kubekpop<br/>Support SLM project by donating <br/><a href=\"http://bit.do/xumppdonate\">Donate</a><br/><br/>Licence GNU LPG :P (to be exact LGPL v3)<br/>"));

}

void main_widget::on_apache_start_clicked()
{
    QString start_comand = "echo '[00019]'`systemctl start "+service_names[0]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->apache_start->setEnabled(false);
    ui->apache_stop->setEnabled(false);
    ui->apache_restart->setEnabled(false);
}

void main_widget::on_apache_stop_clicked()
{
    QString stop_comand = "echo '[00020]'`systemctl stop "+service_names[0]+"`'[XXXXX]' \n";
    bash_root->write(stop_comand.toStdString().c_str());
    ui->apache_start->setEnabled(false);
    ui->apache_stop->setEnabled(false);
    ui->apache_restart->setEnabled(false);
}

void main_widget::on_apache_restart_clicked()
{
    QString restart_comand = "echo '[00021]'`systemctl restart "+service_names[0]+"`'[XXXXX]' \n";
    bash_root->write(restart_comand.toStdString().c_str());
    ui->apache_start->setEnabled(false);
    ui->apache_stop->setEnabled(false);
    ui->apache_restart->setEnabled(false);
}

void main_widget::on_apache_config_clicked()
{
    apache *apache_window = new apache();
    apache_window->show();
}

void main_widget::on_nfs_start_clicked()
{
    QString start_comand = "echo '[00022]'`systemctl start "+service_names[5]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->nfs_start->setEnabled(false);
    ui->nfs_stop->setEnabled(false);
    ui->nfs_restart->setEnabled(false);
}

void main_widget::on_nfs_stop_clicked()
{
    QString stop_comand = "echo '[00023]'`systemctl stop "+service_names[5]+"`'[XXXXX]' \n";
    bash_root->write(stop_comand.toStdString().c_str());
    ui->nfs_start->setEnabled(false);
    ui->nfs_stop->setEnabled(false);
    ui->nfs_restart->setEnabled(false);
}

void main_widget::on_nfs_restart_clicked()
{
    QString restart_comand = "echo '[00024]'`systemctl restart "+service_names[5]+"`'[XXXXX]' \n";
    bash_root->write(restart_comand.toStdString().c_str());
    ui->nfs_start->setEnabled(false);
    ui->nfs_stop->setEnabled(false);
    ui->nfs_restart->setEnabled(false);
}

void main_widget::on_nfs_config_clicked()
{
    nfs *nfs_window = new nfs();
    nfs_window->show();
}

void main_widget::on_samba_config_clicked()
{

}

void main_widget::on_dhcp_config_clicked()
{
    dhcp_window *dhcp_win = new dhcp_window();
    dhcp_win->show();
}


void main_widget::on_ftp_start_clicked()
{
    QString start_comand = "echo '[00025]'`systemctl start "+service_names[2]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->ftp_start->setEnabled(false);
    ui->ftp_stop->setEnabled(false);
    ui->ftp_restart->setEnabled(false);
}

void main_widget::on_ftp_stop_clicked()
{
    QString start_comand = "echo '[00026]'`systemctl stop "+service_names[2]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->ftp_start->setEnabled(false);
    ui->ftp_stop->setEnabled(false);
    ui->ftp_restart->setEnabled(false);
}

void main_widget::on_ftp_restart_clicked()
{
    QString start_comand = "echo '[00027]'`systemctl restart "+service_names[2]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->ftp_start->setEnabled(false);
    ui->ftp_stop->setEnabled(false);
    ui->ftp_restart->setEnabled(false);
}

void main_widget::on_ftp_config_clicked()
{
    ftp_window *ftp_win = new ftp_window();
    ftp_win->bash_root = bash_root;
    ftp_win->show();
    ftp_win->ftpOptionCheck();
    ftp_win->ftpOptionCheckText();
}

void main_widget::on_pushButton_clicked()
{
    ui->apache_config->setEnabled(true);
    ui->mysql_config->setEnabled(true);
    ui->ftp_config->setEnabled(true);
    ui->samba_config->setEnabled(true);
    ui->dhcp_config->setEnabled(true);
    ui->nfs_config->setEnabled(true);
}
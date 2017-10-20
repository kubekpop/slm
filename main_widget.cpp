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
    apache_win->close();
    dhcp_win->close();
    firewall_win->close();
    ftp_win->close();
    nfs_win->close();
    settings_win->close();
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

    // get scrollbar
    log_scrollbar = ui->log->verticalScrollBar();

    // print version
    update_log("SLM dev. [version]");
    bash_root->write("echo '[00036]'`uname -r`'[XXXXX]' \n");
    //setup interface before authorisation
    is_authorised = false;
    lock_interface(true);

    // set app info
    QCoreApplication::setOrganizationName("Tuxdev");
    QCoreApplication::setOrganizationDomain("slm.tuxdev.com");
    QCoreApplication::setApplicationName("SLM");

    // get distribution from settings
    change_distribution();

    // print startup info


    //windows

    apache_win = new apache_window();

    dhcp_win = new dhcp_window();

    firewall_win = new firewall_window();
    firewall_win->bash_root = bash_root;
    connect(firewall_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    ftp_win = new ftp_window();
    ftp_win->bash_root = bash_root;

    nfs_win = new nfs_window();

    settings_win = new settings_window();
    connect(settings_win, SIGNAL(distro_changed()),this, SLOT(change_distribution()));
    settings_win->bash_root = bash_root;
}

void main_widget::update_module_info()
{
    update_log("Loading modules...");
    if(is_authorised == true)
    {
        for(int i = 0; i < service_names.count(); i++)
        {
            pid_check(pids_names[i],i);
            status_check(service_names[i],i);
            port_check(service_names[i],i);
            update_log("Done checks for module: "+service_names[i]);

        }
    }
    else
    {
        for(int i = 0; i < service_names.count(); i++)
        {
            pid_check(pids_names[i],i);
            status_check(service_names[i],i);
            update_log("Done checks for module: "+service_names[i]);

        }
    }
    update_log("Done module checking");
}

void main_widget::change_distribution()
{
    service_names.clear();
    pids_names.clear();
    installCommands.clear();
    QSettings settings;

    QString distribution = settings.value("distro").toString();
    update_log("Distribution: "+distribution);
    if(distribution == "")
    {
        distribution = "fedora";
        settings.setValue("distro","fedora");
    }

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

        installCommands.append("dnf install -y httpd php php-common");      //install Apache
        installCommands.append("dnf install -y dhcp-server");               //install dhcp-server
        installCommands.append("dnf install -y nfs-utils");                 //install nfs server
        installCommands.append("dnf install -y exo");                       //install exo
        installCommands.append("dnf install -y mysql-server");              //install mysql server
        installCommands.append("dnf install -y vsftpd");                    //install ftp-server
        installCommands.append("dnf install -y samba");                     //install samba
        installCommands.append("dnf install -y phpmyadmin");                //install phpmyadmin
    }
    else if(distribution == "fedora")
    {
        service_names.append("httpd");
        pids_names.append("httpd");

        service_names.append("mariadb");
        pids_names.append("mysqld");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");

        service_names.append("smb");
        pids_names.append("smbd");

        service_names.append("dhcpd");
        pids_names.append("dhcpd");

        service_names.append("nfs-server");
        pids_names.append("nfsd");

        installCommands.append("dnf install -y httpd php php-common");      //install Apache
        installCommands.append("dnf install -y dhcp-server");               //install dhcp-server
        installCommands.append("dnf install -y nfs-utils");                 //install nfs server
        installCommands.append("dnf install -y exo");                       //install exo
        installCommands.append("dnf install -y mysql-server");              //install mysql server
        installCommands.append("dnf install -y vsftpd");                    //install ftp-server
        installCommands.append("dnf install -y samba");                     //install samba
        installCommands.append("dnf install -y phpmyadmin");                //install phpmyadmin


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

        installCommands.append("dnf install -y httpd php php-common");      //install Apache
        installCommands.append("dnf install -y dhcp-server");               //install dhcp-server
        installCommands.append("dnf install -y nfs-utils");                 //install nfs server
        installCommands.append("dnf install -y exo");                       //install exo
        installCommands.append("dnf install -y mysql-server");              //install mysql server
        installCommands.append("dnf install -y vsftpd");                    //install ftp-server
        installCommands.append("dnf install -y samba");                     //install samba
        installCommands.append("dnf install -y phpmyadmin");                //install phpmyadmin
    }

    // update information about services
    update_module_info();
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
        update_log("Authorization successful");

        is_authorised = true;
        lock_interface(false);
        root_setup();
        ui->authorize->setEnabled(false);
    }
    else
    {
        update_log("Authorization failed");

        lock_interface(true);
        //interfejs zablokowany
    }
}

void main_widget::update_log(QString new_content)
{
    if(new_content != "")
    {
    log_string += "<font color=\"darkgreen\">" + QTime::currentTime().toString() + "</font> " + new_content + "<br/>";
    ui->log->setHtml(log_string);
    log_scrollbar->setValue(log_scrollbar->maximum());
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
            set_status(output, 0);//status check for Apache
            break;
        case 2:
            set_status(output, 1);//status check for mysql
            break;
        case 3:
            set_status(output, 2);//status check for ftp
            break;
        case 4:
            set_status(output, 3);//status check for samba
            break;
        case 5:
            set_status(output, 4);//status check for dhcp
            break;
        case 6:
            set_pids(output, 0);//pid check for apache
            break;
        case 7:
            set_pids(output, 1);//pid check for mysql
            break;
        case 8:
            set_pids(output, 2);//pid check for ftp
            break;
        case 9:
            set_pids(output, 3);//pid check for samba
            break;
        case 10:
            set_pids(output, 4);//pid check for dhcp
            break;
        case 11:
            set_status(output, 5);//status check for nfs
            break;
        case 12:
            set_pids(output, 5);//pid check for nfs
            break;
        case 13:
            set_port(output,0);//port check for apache
            break;
        case 14:
            set_port(output,1);//port check for mysql
            break;
        case 15:
            set_port(output,2);//port check for ftp
            break;
        case 16:
            set_port(output,3);//port check for samba
            break;
        case 17:
            set_port(output,4);//port check for dhcp
            break;
        case 18:
            set_port(output,5);//port check for nfs
            break;
        case 19:
            pid_check(pids_names[0],0);//start apache
            port_check(service_names[0],0);
            status_check(service_names[0],0);
            break;
        case 20:
            pid_check(pids_names[0],0);//stop apache
            port_check(service_names[0],0);
            status_check(service_names[0],0);
            break;
        case 21:
            pid_check(pids_names[0],0);//restart apache
            port_check(service_names[0],0);
            status_check(service_names[0],0);
            break;
        case 22:
            pid_check(pids_names[5],5);//start nfs
            port_check(service_names[5],5);
            status_check(service_names[5],5);
            break;
        case 23:
            pid_check(pids_names[5],5);//stop nfs
            port_check(service_names[5],5);
            status_check(service_names[5],5);
            break;
        case 24:
            pid_check(pids_names[5],5);//restart nfs
            port_check(service_names[5],5);
            status_check(service_names[5],5);
            break;
        case 25:
            pid_check(pids_names[2],2);//start ftp
            port_check(service_names[2],2);
            status_check(service_names[2],2);
            break;
        case 26:
            pid_check(pids_names[2],2);//stop ftp
            port_check(service_names[2],2);
            status_check(service_names[2],2);
            break;
        case 27:
            pid_check(pids_names[2],2);//restart ftp
            port_check(service_names[2],2);
            status_check(service_names[2],2);
            break;
        case 28:
            update_log(output);//apache install
            update_module_info();
            break;
        case 29:
            update_log(output);//phpmyadmin install
            update_module_info();
            break;
        case 30:
            update_log(output);//ftp install
            update_module_info();
            break;
        case 31:
            update_log(output);//samba install
            update_module_info();
            break;
        case 32:
            update_log(output);//dhcp install
            update_module_info();
            break;
        case 33:
            update_log(output);//nfs install
            update_module_info();
            break;
        case 34:
            update_log(output);//mysql install
            update_module_info();
            break;
        case 35:
            update_log(output);//exo install
            update_module_info();
            break;
        case 36:
            update_log("Current kernel: "+output);//kernel info
            break;
        case 37:
            firewall_win->firewall_prepare_window(output);//gets interfaces and opens firewall
            firewall_win->show();
            break;
        case 38:
            update_log("Dnat command applied: "+output);
            break;
        case 39:
            pid_check(pids_names[1],1);//start mysql
            port_check(service_names[1],1);
            status_check(service_names[1],1);
            break;
        case 40:
            pid_check(pids_names[1],1);//stop mysql
            port_check(service_names[1],1);
            status_check(service_names[1],1);
            break;
        case 41:
            pid_check(pids_names[1],1);//restart mysql
            port_check(service_names[1],1);
            status_check(service_names[1],1);
            break;
        case 42:
            pid_check(pids_names[3],3);//start samba
            port_check(service_names[3],3);
            status_check(service_names[3],3);
            break;
        case 43:
            pid_check(pids_names[3],3);//stop samba
            port_check(service_names[3],3);
            status_check(service_names[3],3);
            break;
        case 44:
            pid_check(pids_names[3],3);//restart samba
            port_check(service_names[3],3);
            status_check(service_names[3],3);
            break;
        case 45:
            pid_check(pids_names[4],4);//start dhcp
            port_check(service_names[4],4);
            status_check(service_names[4],4);
            break;
        case 46:
            pid_check(pids_names[4],4);//stop dhcp
            port_check(service_names[4],4);
            status_check(service_names[4],4);
            break;
        case 47:
            pid_check(pids_names[4],4);//restart dhcp
            port_check(service_names[4],4);
            status_check(service_names[4],4);
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
    bash_root->write("echo '[00037]'`ls -A1 /sys/class/net | sed ':a;N;$!ba;s/\\n/-separate-/g'`'[XXXXX]' \n");
}

void main_widget::on_shell_clicked()
{

}

void main_widget::on_config_clicked()
{
    settings_win->is_authorised = is_authorised;
    settings_win->installCommands = installCommands;
    settings_win->settings_prepare_window();
    settings_win->show();
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
    apache_win->show();
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
    nfs_win->show();
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
    is_authorised = true;
    ui->iptables->setEnabled(true);
}

void main_widget::on_mysql_start_clicked()
{
    QString start_comand = "echo '[00039]'`systemctl start "+service_names[1]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->mysql_start->setEnabled(false);
    ui->mysql_stop->setEnabled(false);
    ui->mysql_restart->setEnabled(false);
}

void main_widget::on_mysql_stop_clicked()
{
    QString start_comand = "echo '[00040]'`systemctl stop "+service_names[1]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->mysql_start->setEnabled(false);
    ui->mysql_stop->setEnabled(false);
    ui->mysql_restart->setEnabled(false);
}

void main_widget::on_mysql_restart_clicked()
{
    QString start_comand = "echo '[00041]'`systemctl restart "+service_names[1]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->mysql_start->setEnabled(false);
    ui->mysql_stop->setEnabled(false);
    ui->mysql_restart->setEnabled(false);
}

void main_widget::on_mysql_config_clicked()
{

}

void main_widget::on_samba_start_clicked()
{
    QString start_comand = "echo '[00042]'`systemctl start "+service_names[3]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->samba_start->setEnabled(false);
    ui->samba_stop->setEnabled(false);
    ui->samba_restart->setEnabled(false);
}

void main_widget::on_samba_stop_clicked()
{
    QString start_comand = "echo '[00043]'`systemctl stop "+service_names[3]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->samba_start->setEnabled(false);
    ui->samba_stop->setEnabled(false);
    ui->samba_restart->setEnabled(false);
}

void main_widget::on_samba_restart_clicked()
{
    QString start_comand = "echo '[00044]'`systemctl restart "+service_names[3]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->samba_start->setEnabled(false);
    ui->samba_stop->setEnabled(false);
    ui->samba_restart->setEnabled(false);
}

void main_widget::on_samba_config_clicked()
{

}

void main_widget::on_dhcp_start_clicked()
{
    QString start_comand = "echo '[00045]'`systemctl start "+service_names[4]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->dhcp_start->setEnabled(false);
    ui->dhcp_stop->setEnabled(false);
    ui->dhcp_restart->setEnabled(false);
}

void main_widget::on_dhcp_stop_clicked()
{
    QString start_comand = "echo '[00046]'`systemctl stop "+service_names[4]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->dhcp_start->setEnabled(false);
    ui->dhcp_stop->setEnabled(false);
    ui->dhcp_restart->setEnabled(false);
}

void main_widget::on_dhcp_restart_clicked()
{
    QString start_comand = "echo '[00047]'`systemctl restart "+service_names[4]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->dhcp_start->setEnabled(false);
    ui->dhcp_stop->setEnabled(false);
    ui->dhcp_restart->setEnabled(false);
}

void main_widget::on_dhcp_config_clicked()
{
    dhcp_win->show();
}

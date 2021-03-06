#include "main_widget.h"
#include "ui_main_widget.h"
#include "QMessageBox"
#include "QTime"
#define VER "1.0 DEV"
QString version = VER;
bool backup_manager = false;
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
    emit close_all_windows();
    apache_win->close();
    dhcp_win->close();
    firewall_win->close();
    ftp_win->close();
    nfs_win->close();
    settings_win->close();
    backup_win->close();
    status_win->close();
    dns_win->close();
    raid_win->close();
    samba_win->close();
    qemu_win->close();
    docker_win->close();
    /*
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Sure?", "Exit Simple Linux Manager?", QMessageBox::Yes | QMessageBox::No);
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
    event->accept();
}

void main_widget::startup()
{
    // setup bash for authorisation
    bash_root = new QProcess(this);
    bash_root->start("/bin/bash");
    connect(bash_root, SIGNAL(readyRead()), this, SLOT(bash_root_reader()));
    get_local_bash_pid(); // to tylko jesli platform = linux

    // get scrollbar
    log_scrollbar = ui->log->verticalScrollBar();

    ui->spacer_top_label->setText(VER);
    // print version
    update_log("SLM "+version);
    bash_root->write("echo '[main][00028]'`uname -r`'[XXXXX]' \n");
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
    connect(apache_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    dhcp_win = new dhcp_window();
    connect(dhcp_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));


    firewall_win = new firewall_window();
    firewall_win->bash_root = bash_root;
    connect(firewall_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    ftp_win = new ftp_window();
    ftp_win->bash_root = bash_root;
    connect(ftp_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    nfs_win = new nfs_window();
    nfs_win->bash_root = bash_root;
    connect(nfs_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    settings_win = new settings_window();
    settings_win->bash_root = bash_root;
    connect(settings_win, SIGNAL(distro_changed()),this, SLOT(change_distribution()));
    connect(settings_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));
    connect(settings_win, SIGNAL(update_module_info()), this, SLOT(update_module_info()));

    status_win = new status_window();
    status_win->bash_root = bash_root;
    connect(status_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    backup_win = new backup_window();
    backup_win->bash_root = bash_root;
    connect(backup_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    raid_win = new raid_window();
    connect(raid_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    samba_win = new samba_window();
    connect(samba_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    dns_win = new dns_window();
    connect(dns_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    qemu_win = new qemu_window();
    qemu_win->bash_root = bash_root;
    connect(qemu_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));

    docker_win = new docker_window();
    docker_win->bash_root = bash_root;
    connect(docker_win, SIGNAL(data_to_log(QString)), this, SLOT(update_log(QString)));


    QTimer *bash_unlocker = new QTimer(this);
    connect(bash_unlocker, &QTimer::timeout, [=] () {bash_root->write("echo '[main][99999][XXXXX]' \n");});
    bash_unlocker->start(2000);
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
            port_check(ports_names[i],i);
            enable_check(service_names[i],i);
            update_log("Done checks for module: "+service_names[i]);

        }
    }
    else
    {
        for(int i = 0; i < service_names.count(); i++)
        {
            pid_check(pids_names[i],i);
            status_check(service_names[i],i);
            enable_check(service_names[i],i);
            update_log("Done checks for module: "+service_names[i]);

        }
    }
    update_log("Done module checking");


}

void main_widget::change_distribution()
{
    service_names.clear();
    pids_names.clear();
    ports_names.clear();
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
        ports_names.append("httpd");

        service_names.append("mysql");
        pids_names.append("mysql");
        ports_names.append("mysql");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");
        ports_names.append("vsftpd");

        service_names.append("smb");
        pids_names.append("smbd");
        ports_names.append("smb");

        service_names.append("dhcpd");
        pids_names.append("dhcpd");
        ports_names.append("dhcpd");

        service_names.append("nfsserver");
        pids_names.append("nfsd");
        ports_names.append("nfs");
        //rpcinfo -p | grep nfs

        service_names.append("named");
        pids_names.append("named");
        ports_names.append("named");

        service_names.append("libvirtd");
        pids_names.append("libvirtd");
        ports_names.append("libvirtd");

        service_names.append("docker");
        pids_names.append("docker");
        ports_names.append("docker");

        installCommands.append("zypper install -y apache2 php7 php7-pear apache2-mod_php7");      //install Apache
        installCommands.append("zypper install -y dhcp-server");                                  //install dhcp-server
        installCommands.append("zypper install -y nfs-kernel-server");                            //install nfs server
        installCommands.append("zypper install -y exo-tools");                                    //install exo
        installCommands.append("zypper install -y mysql-community-server");                       //install mysql server
        installCommands.append("zypper install -y vsftpd");                                       //install ftp-server
        installCommands.append("zypper install -y samba");                                        //install samba
        installCommands.append("zypper install -y phpMyAdmin");                                   //install phpmyadmin
        installCommands.append("zypper install -y bind");                                   //install bind
        installCommands.append("dnf group install --with-optional -y virtualization");  //install virtualization
        installCommands.append("zypper install -y backup-manager");                        //install backup-manager
        installCommands.append("zypper install -y docker");                        //install backup-manager
    }
    else if(distribution == "fedora")
    {
        service_names.append("httpd");
        pids_names.append("httpd");
        ports_names.append("httpd");

        service_names.append("mariadb");
        pids_names.append("mysqld");
        ports_names.append("mysqld");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");
        ports_names.append("vsftpd");

        service_names.append("smb");
        pids_names.append("smbd");
        ports_names.append("smbd");

        service_names.append("dhcpd");
        pids_names.append("dhcpd");
        ports_names.append("dhcpd");

        service_names.append("nfs-server");
        pids_names.append("nfsd");
        ports_names.append("nfs");

        service_names.append("named");
        pids_names.append("named");
        ports_names.append("named");

        service_names.append("libvirtd");
        pids_names.append("libvirtd");
        ports_names.append("libvirtd");

        installCommands.append("dnf install -y httpd php php-common");                  //install Apache
        installCommands.append("dnf install -y dhcp-server");                           //install dhcp-server
        installCommands.append("dnf install -y nfs-utils");                             //install nfs server
        installCommands.append("dnf install -y exo");                                   //install exo
        installCommands.append("dnf install -y mysql-server");                          //install mysql server
        installCommands.append("dnf install -y vsftpd");                                //install ftp-server
        installCommands.append("dnf install -y samba");                                 //install samba
        installCommands.append("dnf install -y phpmyadmin");                            //install phpmyadmin
        installCommands.append("dnf install -y bind");                                  //install dns
        installCommands.append("dnf group install --with-optional -y virtualization");  //install virtualization
        installCommands.append("dnf install -y backup-manager");                        //install backup-manager


    }
    else if(distribution == "centos")
    {
        service_names.append("httpd");
        pids_names.append("httpd");
        ports_names.append("httpd");

        service_names.append("mariadb-server");
        pids_names.append("mysqld");
        ports_names.append("mysqld");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");
        ports_names.append("vsftpd");

        service_names.append("smb");
        pids_names.append("smbd");
        ports_names.append("smbd");

        service_names.append("dhcp");
        pids_names.append("dhcpd");
        ports_names.append("dhcpd");

        service_names.append("nfs-server");
        pids_names.append("nfsd");
        ports_names.append("nfs");

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
        ports_names.append("apache2");

        service_names.append("mysqld");
        pids_names.append("mysql");
        ports_names.append("mysql");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");
        ports_names.append("vsftpd");

        service_names.append("smbd");
        pids_names.append("smbd");
        ports_names.append("smbd");

        service_names.append("isc-dhcp-server");
        pids_names.append("dhcpd");
        ports_names.append("dhcpd");

        service_names.append("nfs-server");
        pids_names.append("nfsd");
        ports_names.append("nfs");

        installCommands.append("dnf install -y httpd php php-common");      //install Apache
        installCommands.append("dnf install -y dhcp-server");               //install dhcp-server
        installCommands.append("dnf install -y nfs-utils");                 //install nfs server
        installCommands.append("dnf install -y exo");                       //install exo
        installCommands.append("dnf install -y mysql-server");              //install mysql server
        installCommands.append("dnf install -y vsftpd");                    //install ftp-server
        installCommands.append("apt -y install samba");                     //install samba
        installCommands.append("dnf install -y phpmyadmin");                //install phpmyadmin
    }
    else if(distribution == "arch")
    {

        service_names.append("httpd");
        pids_names.append("httpd");
        ports_names.append("httpd");

        service_names.append("mysqld");
        pids_names.append("mysql");
        ports_names.append("mysql");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");
        ports_names.append("vsftpd");

        service_names.append("smbd");
        pids_names.append("smbd");
        ports_names.append("smbd");

        service_names.append("dhcpd4");
        pids_names.append("dhcpd");
        ports_names.append("dhcpd");

        service_names.append("nfs-server");
        pids_names.append("nfsd");
        ports_names.append("nfs");

        service_names.append("named");
        pids_names.append("named");
        ports_names.append("named");

        service_names.append("libvirtd");
        pids_names.append("libvirtd");
        ports_names.append("libvirtd");

        service_names.append("docker");
        pids_names.append("docker");
        ports_names.append("docker");

        installCommands.append("pacman -S apache php-apache --noconfirm");  //install Apache
        installCommands.append("pacman -S dhcp --noconfirm");               //install dhcp-server
        installCommands.append("pacman -S nfs-utils --noconfirm");          //install nfs server
        installCommands.append("pacman -S exo --noconfirm");                //install exo
        installCommands.append("pacman -S mysql --noconfirm");              //install mysql server
        installCommands.append("pacman -S vsftpd --noconfirm");             //install ftp-server
        installCommands.append("pacman -S samba --noconfirm");              //install samba
        installCommands.append("pacman -S phpmyadmin --noconfirm");         //install phpmyadmin
        installCommands.append("pacman -S bind --noconfirm");               //install phpmyadmin
        installCommands.append("pacman -S libvirt qemu --noconfirm");       //install libvirt
        installCommands.append("pacman -S backup-manager --noconfirm");     //install backup-manager
        installCommands.append("pacman -S docker --noconfirm");             //install docker

        //QString updateCommand = "pacman -Qu 2>/dev/null | wc -l";
        /*
         * updateCommand = "echo n | pacman -Syu";
         * pacman -Qu
         *
         *
         *
         */
    }
    else if(distribution == "debian")
    {
        service_names.append("apache2");
        pids_names.append("apache2");
        ports_names.append("apache2");

        service_names.append("mysql");
        pids_names.append("mysql");
        ports_names.append("mysql");

        service_names.append("vsftpd");
        pids_names.append("vsftpd");
        ports_names.append("vsftpd");

        service_names.append("smbd");
        pids_names.append("smbd");
        ports_names.append("smbd");

        service_names.append("isc-dhcp-server");
        pids_names.append("dhcpd");
        ports_names.append("dhcpd");

        service_names.append("nfs-server");
        pids_names.append("nfsd");
        ports_names.append("nfs");

        installCommands.append("apt -y install apache2 php7.0 libapache2-mod-php7.0");                    //install Apache
        installCommands.append("apt -y install isc-dhcp-server");                                       //install dhcp-server
        installCommands.append("apt -y install nfs-kernel-server");                                     //install nfs server
        installCommands.append("apt -y install exo");                                                   //install exo
        installCommands.append("export DEBIAN_FRONTEND=noninteractive; apt -y install mysql-server");   //install mysql server
        installCommands.append("apt -y install vsftpd");                                                //install ftp-server
        installCommands.append("apt -y install samba");                                                 //install samba
        installCommands.append("apt -y install phpmyadmin");                                            //install phpmyadmin
    }
    update_module_info();// update information about services
}

void main_widget::root_setup()
{
    for(int i = 0; i < service_names.count(); i++)
    {
        pid_check(pids_names[i],i);
        port_check(ports_names[i],i);
        status_check(service_names[i],i);
        enable_check(service_names[i],i);

    }
    QString command="echo '[main][00066]'`ls -1 /etc/backup-manager.conf`'[XXXXX]'";
    bash_root->write(command.toStdString().c_str());
}

void main_widget::on_authorize_clicked()
{

    bash_root->write("pkexec su \n");
    bash_root->waitForBytesWritten();
    bash_root->write("echo '[main][00000]'`whoami`'[XXXXX]' \n");
}

void main_widget::authorisation(QString user)
{
    if(user == "root")
    {
        update_log("Authorization successful");
        ui->ssh_connect->setEnabled(false);
        ui->authorize->setEnabled(false);
        is_authorised = true;
        lock_interface(false);
        root_setup();

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
        ui->dns_config->setDisabled(interface_locked);
        ui->qemu_config->setDisabled(interface_locked);
        ui->docker_config->setDisabled(interface_locked);

        ui->apache_start->setDisabled(interface_locked);
        ui->mysql_start->setDisabled(interface_locked);
        ui->samba_start->setDisabled(interface_locked);
        ui->nfs_start->setDisabled(interface_locked);
        ui->dhcp_start->setDisabled(interface_locked);
        ui->ftp_start->setDisabled(interface_locked);
        ui->dns_start->setDisabled(interface_locked);
        ui->qemu_start->setDisabled(interface_locked);
        ui->docker_start->setDisabled(interface_locked);

        ui->apache_stop->setDisabled(interface_locked);
        ui->mysql_stop->setDisabled(interface_locked);
        ui->samba_stop->setDisabled(interface_locked);
        ui->nfs_stop->setDisabled(interface_locked);
        ui->dhcp_stop->setDisabled(interface_locked);
        ui->ftp_stop->setDisabled(interface_locked);
        ui->dns_stop->setDisabled(interface_locked);
        ui->qemu_stop->setDisabled(interface_locked);
        ui->docker_stop->setDisabled(interface_locked);

        ui->apache_restart->setDisabled(interface_locked);
        ui->mysql_restart->setDisabled(interface_locked);
        ui->samba_restart->setDisabled(interface_locked);
        ui->nfs_restart->setDisabled(interface_locked);
        ui->dhcp_restart->setDisabled(interface_locked);
        ui->ftp_restart->setDisabled(interface_locked);
        ui->dns_restart->setDisabled(interface_locked);
        ui->qemu_restart->setDisabled(interface_locked);
        ui->docker_restart->setDisabled(interface_locked);

        ui->iptables->setDisabled(interface_locked);
        if (backup_manager == true)
        {
            ui->backup->setDisabled(interface_locked);
        }
        else
        {
            ui->backup->setDisabled(true);
        }
        ui->apache_enable->setDisabled(interface_locked);
        ui->mysql_enable->setDisabled(interface_locked);
        ui->ftp_enable->setDisabled(interface_locked);
        ui->dhcp_enable->setDisabled(interface_locked);
        ui->samba_enable->setDisabled(interface_locked);
        ui->nfs_enable->setDisabled(interface_locked);
        ui->dns_enable->setDisabled(interface_locked);
        ui->qemu_enable->setDisabled(interface_locked);
        ui->docker_enable->setDisabled(interface_locked);
}

void main_widget::enable_check(QString service_name, int module_number)
{
    QString check_command;
    switch(module_number)
    {
    case 0:
        check_command = "echo '[main][00052]'`(systemctl status "+service_name+" | grep 'Loaded.*enabled.*vendor.*$') > /dev/null && echo enabled || echo disabled`'[XXXXX]' \n";
        break;

    case 1:
        check_command = "echo '[main][00053]'`(systemctl status "+service_name+" | grep 'Loaded.*enabled.*vendor.*$') > /dev/null && echo enabled || echo disabled`'[XXXXX]' \n";
        break;

    case 2:
        check_command = "echo '[main][00054]'`(systemctl status "+service_name+" | grep 'Loaded.*enabled.*vendor.*$') > /dev/null && echo enabled || echo disabled`'[XXXXX]' \n";
        break;

    case 3:
        check_command = "echo '[main][00055]'`(systemctl status "+service_name+" | grep 'Loaded.*enabled.*vendor.*$') > /dev/null && echo enabled || echo disabled`'[XXXXX]' \n";
        break;

    case 4:
        check_command = "echo '[main][00056]'`(systemctl status "+service_name+" | grep 'Loaded.*enabled.*vendor.*$') > /dev/null && echo enabled || echo disabled`'[XXXXX]' \n";
        break;

    case 5:
        check_command = "echo '[main][00057]'`(systemctl status "+service_name+" | grep 'Loaded.*enabled.*vendor.*$') > /dev/null && echo enabled || echo disabled`'[XXXXX]' \n";
        //update_log(check_command);
        break;

    case 6:
        check_command = "echo '[main][00058]'`(systemctl status "+service_name+" | grep 'Loaded.*enabled.*vendor.*$') > /dev/null && echo enabled || echo disabled`'[XXXXX]' \n";
        break;

    case 7:
        check_command = "echo '[main][00059]'`(systemctl status "+service_name+" | grep 'Loaded.*enabled.*vendor.*$') > /dev/null && echo enabled || echo disabled`'[XXXXX]' \n";
        //update_log(check_command);
        break;

    case 8:
        check_command = "echo '[main][00060]'`(systemctl status "+service_name+" | grep 'Loaded.*enabled.*vendor.*$') > /dev/null && echo enabled || echo disabled`'[XXXXX]' \n";
        //update_log(check_command);
        break;
    }
    bash_root->write(check_command.toStdString().c_str());
    // jump to function bash_root_reader();
}

void main_widget::status_check(QString service_name, int module_number)
{
    QString check_command;
    switch(module_number)
    {
    case 0:
        check_command = "echo '[main][00001]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 1:
        check_command = "echo '[main][00002]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 2:
        check_command = "echo '[main][00003]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 3:
        check_command = "echo '[main][00004]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 4:
        check_command = "echo '[main][00005]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 5:
        check_command = "echo '[main][00011]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') || $(systemctl status $service | egrep '^.*active.*(exited).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;

    case 6:
        check_command = "echo '[main][00041]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') || $(systemctl status $service | egrep '^.*active.*(exited).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;
    case 7:
        check_command = "echo '[main][00050]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') || $(systemctl status $service | egrep '^.*active.*(exited).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
        break;
    case 8:
        check_command = "echo '[main][00062]'`service='"+service_name+"'; if [[ $(systemctl status $service) == '' ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Loaded:.*not-found.*$') ]];then echo 'Not installed'; elif [[ $(systemctl status $service | egrep '^.*Active:.*failed.*$') ]];then echo 'Failed'; elif [[ $(systemctl status $service | egrep '^.*active.*(running).*$') || $(systemctl status $service | egrep '^.*active.*(exited).*$') ]];then echo 'Running'; else echo 'Not running'; fi`'[XXXXX]' \n";
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
        check_command = "echo '[main][00006]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 1:
        check_command = "echo '[main][00007]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 2:
        check_command = "echo '[main][00008]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 3:
        check_command = "echo '[main][00009]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 4:
        check_command = "echo '[main][00010]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 5:
        check_command = "echo '[main][00012]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        break;

    case 6:
        check_command = "echo '[main][00046]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        //update_log(check_command);
        break;

    case 7:
        check_command = "echo '[main][00051]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        //update_log(check_command);
        break;
    case 8:
        check_command = "echo '[main][00063]'`IN=$(ps aux | grep "+service_name+" | grep -v grep | echo $(xargs -L1 echo|awk '{ print $2}'));echo $IN | sed 's/ /,/g'`'[XXXXX]' \n";
        //update_log(check_command);
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
        check_command = "echo '[main][00013]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;

    case 1:
        check_command = "echo '[main][00014]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;

    case 2:
        check_command = "echo '[main][00015]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;

    case 3:
        check_command = "echo '[main][00016]'`netstat -lnp | grep "+service_name+" | grep tcp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;

    case 4:
        check_command = "echo '[main][00017]'`netstat -lnp | grep "+service_name+" | grep udp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        break;
    case 5:

        check_command = "echo '[main][00018]'`rpcinfo -p | grep "+service_name+" | awk '{ print $4 }' | sed -n 1p`'[XXXXX]' \n";
        break;
    case 6:
        check_command = "echo '[main][00042]'`netstat -lnp | grep "+service_name+" | grep udp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        //update_log(check_command);
        break;

    case 7:
        check_command = "echo '[main][00064]'`netstat -lnp | grep "+service_name+" | grep udp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        //update_log(check_command);
        break;

    case 8:
        check_command = "echo '[main][00042]'`netstat -lnp | grep "+service_name+" | grep udp | awk '{ print $4 }' | awk -F':' ' { print $NF } '`'[XXXXX]' \n";
        //update_log(check_command);
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
            QString class_tag = output.left(6);
            class_tag.replace("[","");
            class_tag.replace("]","");
            output.remove(0, 6);
            if(class_tag == "main")
            {
                bash_output_interpreter(output);
            }
            else if(class_tag ==  "apac")
            {
                apache_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "back")
            {
                backup_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "dhcp")
            {
                dhcp_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "name")
            {
                dns_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "dckr")
            {
                docker_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "fire")
            {
                firewall_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "ftpd")
            {
                ftp_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "nfsd")
            {
               nfs_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "qemu")
            {
                qemu_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "raid")
            {
                raid_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "smbd")
            {
                samba_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "sett")
            {
                settings_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "stat")
            {
                status_win->bash_output_interpreter(output);
            }
            else if(class_tag ==  "sshc")
            {
                // todo: co to wgle jest i jak to dziaua
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
    case 6:
        ui->dns_status->setText(status);
        ui->dns_status->setStyleSheet(status_style_sheet);
        if(status == "Not installed")
        {
            ui->dns_start->setEnabled(false);
            ui->dns_stop->setEnabled(false);
            ui->dns_restart->setEnabled(false);
            ui->dns_config->setEnabled(false);
            ui->dns_ports->setText("N/A");
            ui->dns_pids->setText("N/A");
        }
        if(is_authorised == true)
        {
            if(status == "Running")
            {
                ui->dns_start->setEnabled(false);
                ui->dns_stop->setEnabled(true);
                ui->dns_restart->setEnabled(true);
                ui->dns_config->setEnabled(true);
            }
            if(status == "Not running")
            {
                ui->dns_start->setEnabled(true);
                ui->dns_stop->setEnabled(false);
                ui->dns_restart->setEnabled(false);
                ui->dns_config->setEnabled(true);
            }
            if(status == "Failed")
            {
                ui->dns_start->setEnabled(true);
                ui->dns_stop->setEnabled(false);
                ui->dns_restart->setEnabled(false);
                ui->dns_config->setEnabled(true);
            }
        }
        break;
    case 7:
        ui->qemu_status->setText(status);
        ui->qemu_status->setStyleSheet(status_style_sheet);
        if(status == "Not installed")
        {
            ui->qemu_start->setEnabled(false);
            ui->qemu_stop->setEnabled(false);
            ui->qemu_restart->setEnabled(false);
            ui->qemu_config->setEnabled(false);
            ui->qemu_ports->setText("N/A");
            ui->qemu_pids->setText("N/A");
        }
        if(is_authorised == true)
        {
            if(status == "Running")
            {
                ui->qemu_start->setEnabled(false);
                ui->qemu_stop->setEnabled(true);
                ui->qemu_restart->setEnabled(true);
                ui->qemu_config->setEnabled(true);
            }
            if(status == "Not running")
            {
                ui->qemu_start->setEnabled(true);
                ui->qemu_stop->setEnabled(false);
                ui->qemu_restart->setEnabled(false);
                ui->qemu_config->setEnabled(true);
            }
            if(status == "Failed")
            {
                ui->qemu_start->setEnabled(true);
                ui->qemu_stop->setEnabled(false);
                ui->qemu_restart->setEnabled(false);
                ui->qemu_config->setEnabled(true);
            }
        }
        break;

    case 8:
        ui->docker_status->setText(status);
               ui->docker_status->setStyleSheet(status_style_sheet);
               if(status == "Not installed")
               {
                   ui->docker_start->setEnabled(false);
                   ui->docker_stop->setEnabled(false);
                   ui->docker_restart->setEnabled(false);
                   ui->docker_config->setEnabled(false);
                   ui->docker_ports->setText("N/A");
                   ui->docker_pids->setText("N/A");
               }
               if(is_authorised == true)
               {
                   if(status == "Running")
                   {
                       ui->docker_start->setEnabled(false);
                       ui->docker_stop->setEnabled(true);
                       ui->docker_restart->setEnabled(true);
                       ui->docker_config->setEnabled(true);
                   }
                   if(status == "Not running")
                   {
                       ui->docker_start->setEnabled(true);
                       ui->docker_stop->setEnabled(false);
                       ui->docker_restart->setEnabled(false);
                       ui->docker_config->setEnabled(true);
                   }
                   if(status == "Failed")
                   {
                       ui->docker_start->setEnabled(true);
                       ui->docker_stop->setEnabled(false);
                       ui->docker_restart->setEnabled(false);
                       ui->docker_config->setEnabled(true);
                   }
               }
        break;
    default:
        break;

    }
}

void main_widget::set_enabled(bool enabled, int module_number)
{

    switch(module_number)
    {
    case 0:
        ui->apache_enable->setChecked(enabled);
        break;
    case 1:
        ui->mysql_enable->setChecked(enabled);
        break;
    case 2:
        ui->ftp_enable->setChecked(enabled);
        break;
    case 3:
        ui->samba_enable->setChecked(enabled);
        break;
    case 4:
        ui->dhcp_enable->setChecked(enabled);
        break;
    case 5:
        ui->nfs_enable->setChecked(enabled);
        break;
    case 6:
        ui->dns_enable->setChecked(enabled);
        break;
    case 7:
        ui->qemu_enable->setChecked(enabled);
        break;
    case 8:
        ui->docker_enable->setChecked(enabled);
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
    case 6:
        ui->dns_pids->setText(pids);
        break;
    case 7:
        ui->qemu_pids->setText(pids);
        break;
    case 8:
        ui->docker_pids->setText(pids);
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
    case 6:
        ui->dns_ports->setText(port);
        break;
    default:
        break;
    }
}


void main_widget::on_iptables_clicked()
{
    bash_root->write("echo '[fire][00001]'`ls -A1 /sys/class/net | sed ':a;N;$!ba;s/\\n/-separate-/g'`'[XXXXX]' \n");
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
    QString start_comand = "echo '[main][00019]'`systemctl start "+service_names[0]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->apache_start->setEnabled(false);
    ui->apache_stop->setEnabled(false);
    ui->apache_restart->setEnabled(false);
    update_log("Starting apache ["+service_names[0]+"]");
}

void main_widget::on_apache_stop_clicked()
{
    QString stop_comand = "echo '[main][00020]'`systemctl stop "+service_names[0]+"`'[XXXXX]' \n";
    bash_root->write(stop_comand.toStdString().c_str());
    ui->apache_start->setEnabled(false);
    ui->apache_stop->setEnabled(false);
    ui->apache_restart->setEnabled(false);
    update_log("Stopping apache ["+service_names[0]+"]");
}

void main_widget::on_apache_restart_clicked()
{
    QString restart_comand = "echo '[main][00021]'`systemctl restart "+service_names[0]+"`'[XXXXX]' \n";
    bash_root->write(restart_comand.toStdString().c_str());
    ui->apache_start->setEnabled(false);
    ui->apache_stop->setEnabled(false);
    ui->apache_restart->setEnabled(false);
    update_log("Restarting apache ["+service_names[0]+"]");
}

void main_widget::on_apache_config_clicked()
{
    apache_win->show();
}

void main_widget::on_nfs_start_clicked()
{
    QString start_comand = "echo '[main][00022]'`systemctl start "+service_names[5]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->nfs_start->setEnabled(false);
    ui->nfs_stop->setEnabled(false);
    ui->nfs_restart->setEnabled(false);
    update_log("Starting nfs server");
}

void main_widget::on_nfs_stop_clicked()
{
    QString stop_comand = "echo '[main][00023]'`systemctl stop "+service_names[5]+"`'[XXXXX]' \n";
    bash_root->write(stop_comand.toStdString().c_str());
    ui->nfs_start->setEnabled(false);
    ui->nfs_stop->setEnabled(false);
    ui->nfs_restart->setEnabled(false);
    update_log("Stopping nfs server");
}

void main_widget::on_nfs_restart_clicked()
{
    QString restart_comand = "echo '[main][00024]'`systemctl restart "+service_names[5]+"`'[XXXXX]' \n";
    bash_root->write(restart_comand.toStdString().c_str());
    ui->nfs_start->setEnabled(false);
    ui->nfs_stop->setEnabled(false);
    ui->nfs_restart->setEnabled(false);
    update_log("Restarting nfs server");
}

void main_widget::on_nfs_config_clicked()
{
    QString command_paths = "echo '[nfsd][00004]'`sed -i '/^\\s*$/d' /etc/exports >/dev/null; grep -n -v '^\\#' /etc/exports | cut -d'(' -f1 | sed ':a;N;$!ba;s%\\n%libQt5Xml5%g'`'[XXXXX]' \n";
    bash_root->write(command_paths.toStdString().c_str());
}

void main_widget::on_ftp_start_clicked()
{
    QString start_comand = "echo '[main][00025]'`systemctl start "+service_names[2]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->ftp_start->setEnabled(false);
    ui->ftp_stop->setEnabled(false);
    ui->ftp_restart->setEnabled(false);
    update_log("Starting vsftpd");
}


void main_widget::on_ftp_stop_clicked()
{
    QString start_comand = "echo '[main][00026]'`systemctl stop "+service_names[2]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->ftp_start->setEnabled(false);
    ui->ftp_stop->setEnabled(false);
    ui->ftp_restart->setEnabled(false);
    update_log("Stopping vsftpd");
}

void main_widget::on_ftp_restart_clicked()
{
    QString start_comand = "echo '[main][00027]'`systemctl restart "+service_names[2]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->ftp_start->setEnabled(false);
    ui->ftp_stop->setEnabled(false);
    ui->ftp_restart->setEnabled(false);
    update_log("Restarting vsftpd");
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
    ui->dns_config->setEnabled(true);
    ui->docker_config->setEnabled(true);
}

void main_widget::on_mysql_start_clicked()
{
    QString start_comand = "echo '[main][00029]'`systemctl start "+service_names[1]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->mysql_start->setEnabled(false);
    ui->mysql_stop->setEnabled(false);
    ui->mysql_restart->setEnabled(false);
    update_log("Starting mysql");
}

void main_widget::on_mysql_stop_clicked()
{
    QString start_comand = "echo '[main][00030]'`systemctl stop "+service_names[1]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->mysql_start->setEnabled(false);
    ui->mysql_stop->setEnabled(false);
    ui->mysql_restart->setEnabled(false);
    update_log("Stopping mysql");
}

void main_widget::on_mysql_restart_clicked()
{
    QString start_comand = "echo '[main][00031]'`systemctl restart "+service_names[1]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->mysql_start->setEnabled(false);
    ui->mysql_stop->setEnabled(false);
    ui->mysql_restart->setEnabled(false);
    update_log("Restarting mysql");
}

void main_widget::on_mysql_config_clicked()
{
    //exo-open --launch webbrowser
    QProcess phpmyadmin;
    phpmyadmin.startDetached("exo-open --launch webbrowser localhost/phpmyadmin");
}

void main_widget::on_samba_start_clicked()
{
    QString start_comand = "echo '[main][00032]'`systemctl start "+service_names[3]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->samba_start->setEnabled(false);
    ui->samba_stop->setEnabled(false);
    ui->samba_restart->setEnabled(false);
    update_log("Starting samba");
}

void main_widget::on_samba_stop_clicked()
{
    QString start_comand = "echo '[main][00033]'`systemctl stop "+service_names[3]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->samba_start->setEnabled(false);
    ui->samba_stop->setEnabled(false);
    ui->samba_restart->setEnabled(false);
    update_log("Stopping samba");
}

void main_widget::on_samba_restart_clicked()
{
    QString start_comand = "echo '[main][00034]'`systemctl restart "+service_names[3]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->samba_start->setEnabled(false);
    ui->samba_stop->setEnabled(false);
    ui->samba_restart->setEnabled(false);
    update_log("Restarting samba");
}

void main_widget::on_samba_config_clicked()
{
    samba_win->show();
}

void main_widget::on_dhcp_start_clicked()
{
    QString start_comand = "echo '[main][00035]'`systemctl start "+service_names[4]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->dhcp_start->setEnabled(false);
    ui->dhcp_stop->setEnabled(false);
    ui->dhcp_restart->setEnabled(false);
    update_log("Starting DHCP");
}

void main_widget::on_dhcp_stop_clicked()
{
    QString start_comand = "echo '[main][00036]'`systemctl stop "+service_names[4]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->dhcp_start->setEnabled(false);
    ui->dhcp_stop->setEnabled(false);
    ui->dhcp_restart->setEnabled(false);
    update_log("Stopping DHCP");
}

void main_widget::on_dhcp_restart_clicked()
{
    QString start_comand = "echo '[main][00037]'`systemctl restart "+service_names[4]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->dhcp_start->setEnabled(false);
    ui->dhcp_stop->setEnabled(false);
    ui->dhcp_restart->setEnabled(false);
    update_log("Restarting DHCP");
}

void main_widget::on_dhcp_config_clicked()
{
    dhcp_win->show();
}

void main_widget::on_status_button_clicked()
{
    status_win->show();
    status_win->status_prepare_window();
}

void main_widget::on_backup_clicked()
{
    QString check_bm_tarball = "echo '[back][00001]'`grep '^[[:space:]]*export[[:space:]]*BM_TARBALL_DIRECTORIES.*$' /etc/backup-manager.conf >> /dev/null && echo true`'[XXXXX]' \n";
    bash_root->write(check_bm_tarball.toStdString().c_str());
}

void main_widget::connect_ssh(QString IP, QString port, QString password)
{
    //  block interface
    this->setEnabled(false);
    status_win->setEnabled(false);
    settings_win->setEnabled(false);
    update_log("SSH connection attempt");
    if(port.isEmpty())
    {
        port = "22";
    }
    QString connect_ssh_command = "(sshpass -p'"+password+"' ssh -o UserKnownHostsFile=/dev/null -o ConnectTimeout=2 -o StrictHostKeyChecking=no -p "+port+" root@"+IP+") || echo 'false' \n";
    bash_root->write(connect_ssh_command.toStdString().c_str());
    QTimer::singleShot(2000, this, SLOT(check_ssh_connection()));
}

void main_widget::on_ssh_connect_clicked()
{
    //  block interface
    this->setEnabled(false);
    status_win->setEnabled(false);
    settings_win->setEnabled(false);

    ssh_connect_window *ssh_connect_win = new ssh_connect_window();
    connect(ssh_connect_win, SIGNAL(connect_ssh(QString,QString,QString)), this, SLOT(connect_ssh(QString,QString,QString)));
    connect(ssh_connect_win, SIGNAL(connect_ssh(QString,QString,QString)), ssh_connect_win, SLOT(deleteLater()));
    connect(ssh_connect_win, SIGNAL(closed()), this, SLOT(ssh_connect_window_closed()));
    connect(this, SIGNAL(close_all_windows()), ssh_connect_win, SLOT(close()));
    ssh_connect_win->show();
}

void main_widget::ssh_connect_window_closed()
{
    //activate interface
    this->setEnabled(true);
    status_win->setEnabled(true);
    settings_win->setEnabled(true);
}

void main_widget::check_ssh_connection()
{
    bash_root->write("echo '[main][00000]'`whoami`'[XXXXX]' \n");
    QTimer::singleShot(500, this, SLOT(get_ssh_bash_pid()));
}


void main_widget::get_ssh_bash_pid()
{
    bash_root->write("echo '[main][00038]'`echo $$`'[XXXXX]' \n");
}

void main_widget::get_local_bash_pid()
{
    bash_root->write("echo '[main][00039]'`echo $$`'[XXXXX]' \n");
}

void main_widget::verify_bash_pid()
{
    bash_root->write("echo '[main][00040]'`echo $$`'[XXXXX]' \n");
}


void main_widget::on_raid_manager_clicked()
{
    raid_win->show();
}

void main_widget::on_dns_config_clicked()
{
    dns_win->show();
}

void main_widget::on_dns_start_clicked()
{
    QString start_comand = "echo '[main][00043]'`systemctl start "+service_names[6]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->dns_start->setEnabled(false);
    ui->dns_stop->setEnabled(true);
    ui->dns_restart->setEnabled(false);
    update_log("Starting DNS Server");
}


void main_widget::on_dns_stop_clicked()
{
    QString start_comand = "echo '[main][00044]'`systemctl stop "+service_names[6]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->dns_start->setEnabled(true);
    ui->dns_stop->setEnabled(false);
    ui->dns_restart->setEnabled(false);
    update_log("Stopping DNS Server");
}

void main_widget::on_dns_restart_clicked()
{
    QString start_comand = "echo '[main][00045]'`systemctl restart "+service_names[6]+"`'[XXXXX]' \n";
    bash_root->write(start_comand.toStdString().c_str());
    ui->dns_start->setEnabled(false);
    ui->dns_stop->setEnabled(true);
    ui->dns_restart->setEnabled(true);
    update_log("Restarting DNS Server");
}

void main_widget::on_qemu_config_clicked()
{
    QString command = "echo [qemu][00001]`virsh --connect qemu:///system list --all | awk '{ print $2 }' | sed -n 3,999p | sed ':a;N;$!ba;s/\\n/=/g' | rev | cut -d= -f2-999 | rev`[XXXXX]";
    bash_root->write(command.toStdString().c_str());
}

void main_widget::on_qemu_start_clicked()
{
    QString command = "echo [main][00047]`systemctl start "+service_names[7]+"`[XXXXX]";
    bash_root->write(command.toStdString().c_str());
    ui->qemu_start->setEnabled(false);
    ui->qemu_stop->setEnabled(true);
    ui->qemu_restart->setEnabled(true);
    update_log("Starting "+service_names[7]);
}


void main_widget::on_qemu_stop_clicked()
{
    QString command = "echo [main][00048]`systemctl stop "+service_names[7]+"`[XXXXX]";
    bash_root->write(command.toStdString().c_str());
    ui->qemu_start->setEnabled(true);
    ui->qemu_stop->setEnabled(false);
    ui->qemu_restart->setEnabled(false);
    update_log("Stopping "+service_names[7]);
}

void main_widget::on_qemu_restart_clicked()
{
    QString command = "echo [main][00049]`systemctl restart "+service_names[7]+"`[XXXXX]";
    bash_root->write(command.toStdString().c_str());
    ui->qemu_start->setEnabled(false);
    ui->qemu_stop->setEnabled(true);
    ui->qemu_restart->setEnabled(true);
    update_log("Restarting "+service_names[7]);
}



void main_widget::enableService(bool state, int service)
{
    QString command = "";
    if(state == true)
    {
        command = "echo '[main][00061]'`systemctl enable "+service_names[service]+"`'[XXXXX]'";
        update_log("Enabling "+service_names[service]);
    }
    else
    {
        command = "echo '[main][00061]'`systemctl disable "+service_names[service]+"`'[XXXXX]'";
        update_log("Disabling "+service_names[service]);
    }
    bash_root->write(command.toStdString().c_str());
}

void main_widget::on_apache_enable_clicked(bool checked)
{
     enableService(checked, 0);
}

void main_widget::on_mysql_enable_clicked(bool checked)
{
    enableService(checked, 1);
}

void main_widget::on_ftp_enable_clicked(bool checked)
{
    enableService(checked, 2);
}

void main_widget::on_samba_enable_clicked(bool checked)
{
    enableService(checked, 3);
}

void main_widget::on_dhcp_enable_clicked(bool checked)
{
    enableService(checked, 4);
}

void main_widget::on_nfs_enable_clicked(bool checked)
{
    enableService(checked, 5);
}

void main_widget::on_dns_enable_clicked(bool checked)
{
    enableService(checked, 6);
}

void main_widget::on_qemu_enable_clicked(bool checked)
{
    enableService(checked, 7);
}
void main_widget::on_docker_enable_clicked(bool checked)
{
    enableService(checked, 8);
}


void main_widget::on_docker_config_clicked()
{
    QString command = "echo [dckr][00001]`docker ps -a --format '{{.Names}}' | sed ':a;N;$!ba;s/\\n/=/g'`[XXXXX]";
    bash_root->write(command.toStdString().c_str());
}

void main_widget::on_docker_start_clicked()
{
    QString command = "echo [main][00067]`systemctl start "+service_names[8]+"`[XXXXX]";
    bash_root->write(command.toStdString().c_str());
    ui->docker_start->setEnabled(false);
    ui->docker_stop->setEnabled(true);
    ui->docker_restart->setEnabled(true);
    update_log("Starting "+service_names[8]);
}

void main_widget::on_docker_stop_clicked()
{
    QString command = "echo [main][00068]`systemctl stop "+service_names[8]+"`[XXXXX]";
    bash_root->write(command.toStdString().c_str());
    ui->docker_start->setEnabled(true);
    ui->docker_stop->setEnabled(false);
    ui->docker_restart->setEnabled(false);
    update_log("Stopping "+service_names[8]);
}

void main_widget::on_docker_restart_clicked()
{
    QString command = "echo [main][00069]`systemctl restart "+service_names[8]+"`[XXXXX]";
    bash_root->write(command.toStdString().c_str());
    ui->docker_start->setEnabled(false);
    ui->docker_stop->setEnabled(true);
    ui->docker_restart->setEnabled(true);
    update_log("Restarting "+service_names[8]);
}


void main_widget::bash_output_interpreter(QString output)
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
        case -1:
            //log appender
            break;
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
            port_check(ports_names[0],0);
            status_check(service_names[0],0);
            break;
        case 20:
            pid_check(pids_names[0],0);//stop apache
            port_check(ports_names[0],0);
            status_check(service_names[0],0);
            break;
        case 21:
            pid_check(pids_names[0],0);//restart apache
            port_check(ports_names[0],0);
            status_check(service_names[0],0);
            break;
        case 22:
            pid_check(pids_names[5],5);//start nfs
            port_check(ports_names[5],5);
            status_check(service_names[5],5);
            break;
        case 23:
            pid_check(pids_names[5],5);//stop nfs
            port_check(ports_names[5],5);
            status_check(service_names[5],5);
            break;
        case 24:
            pid_check(pids_names[5],5);//restart nfs
            port_check(ports_names[5],5);
            status_check(service_names[5],5);
            break;
        case 25:
            pid_check(pids_names[2],2);//start ftp
            port_check(ports_names[2],2);
            status_check(service_names[2],2);
            break;
        case 26:
            pid_check(pids_names[2],2);//stop ftp
            port_check(ports_names[2],2);
            status_check(service_names[2],2);
            break;
        case 27:
            pid_check(pids_names[2],2);//restart ftp
            port_check(ports_names[2],2);
            status_check(service_names[2],2);
            break;
        case 28:
            update_log("Current kernel: "+output);//kernel info
            break;
        // RIP case 38
        case 29:
            pid_check(pids_names[1],1);//start mysql
            port_check(ports_names[1],1);
            status_check(service_names[1],1);
            break;
        case 30:
            pid_check(pids_names[1],1);//stop mysql
            port_check(ports_names[1],1);
            status_check(service_names[1],1);
            break;
        case 31:
            pid_check(pids_names[1],1);//restart mysql
            port_check(ports_names[1],1);
            status_check(service_names[1],1);
            break;
        case 32:
            pid_check(pids_names[3],3);//start samba
            port_check(ports_names[3],3);
            status_check(service_names[3],3);
            break;
        case 33:
            pid_check(pids_names[3],3);//stop samba
            port_check(ports_names[3],3);
            status_check(service_names[3],3);
            break;
        case 34:
            pid_check(pids_names[3],3);//restart samba
            port_check(ports_names[3],3);
            status_check(service_names[3],3);
            break;
        case 35:
            pid_check(pids_names[4],4);//start dhcp
            port_check(ports_names[4],4);
            status_check(service_names[4],4);
            break;
        case 36:
            pid_check(pids_names[4],4);//stop dhcp
            port_check(ports_names[4],4);
            status_check(service_names[4],4);
            break;
        case 37:
            pid_check(pids_names[4],4);//restart dhcp
            port_check(ports_names[4],4);
            status_check(service_names[4],4);
            break;
        case 38:
            // verify connection and get ssh bash pid
            output.replace(" ","");
            if(!output.isEmpty() && output.toInt() != this->local_bash_pid && is_authorised == true)
            {
                this->ssh_bash_pid = output.toInt();
                update_log("Connected via ssh");
                QTimer *timer = new QTimer(this);
                connect(timer, SIGNAL(timeout()), this, SLOT(verify_bash_pid()));
                timer->start(5000);
            }
            // activate interface
            this->setEnabled(true);
            status_win->setEnabled(true);
            settings_win->setEnabled(true);
            break;
        case 39:
            //get local bash pid
            output.replace(" ","");
            if(!output.isEmpty())
            {
                this->local_bash_pid = output.toInt();
                //update_log("local bash pid = "+QString::number(this->local_bash_pid));
            }
            break;
        case 40:
            // check if ssh session stil active
            output.replace(" ","");
            if(output.toInt() != this->ssh_bash_pid)
            {
                update_log("Critical ssh connection error wrong bash pid: "+output+" expected: "+QString::number(this->ssh_bash_pid));
                this->setEnabled(false);
                status_win->setEnabled(false);
                settings_win->setEnabled(false);
            }
            //update_log("got bash pid: "+output+" expected: "+QString::number(this->ssh_bash_pid));
            break;
        case 41:
            set_status(output, 6);//status check for dns
            break;
        case 42:
            set_port(output,6);//port check for dns
            update_log(output);
            break;
        case 43:
            pid_check(pids_names[6],6);//start dns
            port_check(ports_names[6],6);
            status_check(service_names[6],6);
            break;
        case 44:
            pid_check(pids_names[6],6);//stop dns
            port_check(ports_names[6],6);
            status_check(service_names[6],6);
            break;
        case 45:
            pid_check(pids_names[6],6);//restart dns
            port_check(ports_names[6],6);
            status_check(service_names[6],6);
            break;
        case 46:
            set_pids(output, 6);//pid check for dns
            break;
        case 47:
            pid_check(pids_names[7],7);//start libvirt
            port_check(ports_names[7],7);
            status_check(service_names[7],7);
            break;
        case 48:
            pid_check(pids_names[7],7);//stop libvirt
            port_check(ports_names[7],7);
            status_check(service_names[7],7);
            break;
        case 49:
            pid_check(pids_names[7],7);//restart libvirt
            port_check(ports_names[7],7);
            status_check(service_names[7],7);
            break;
        case 50:
            set_status(output,7);//status check for libvirt
            break;
        case 51:
            set_pids(output,7);//pid check for libvirt
            break;
        case 52://apache enable check
            if(output == "enabled")
            {
                set_enabled(true, 0);
            }
            else
            {
                set_enabled(false, 0);
             }
            break;
        case 53://mysql enable check
            if(output == "enabled")
            {
                set_enabled(true, 1);
            }
            else
            {
                set_enabled(false, 1);
            }
            break;
        case 54://ftp enable check
            if(output == "enabled")
            {
                set_enabled(true, 2);
            }
            else
            {
                set_enabled(false, 2);
            }
            break;
        case 55://samba enable check
            if(output == "enabled")
            {
                set_enabled(true, 3);
            }
            else
            {
                set_enabled(false, 3);
            }
            break;
        case 56://dhcp enable check
            if(output == "enabled")
            {
                set_enabled(true, 4);
            }
            else
            {
                set_enabled(false, 4);
            }
            break;
        case 57://nfs enable check
            if(output == "enabled")
            {
                set_enabled(true, 5);
            }
            else
            {
                set_enabled(false, 5);
            }
            break;
        case 58://dns enable check
            if(output == "enabled")
            {
                set_enabled(true, 6);
            }
            else
            {
                set_enabled(false, 6);
            }
            break;
        case 59://libvirt enable check
            if(output == "enabled")
            {
                set_enabled(true, 7);
            }
            else
            {
                set_enabled(false, 7);
            }
            break;
        case 60://docker enable check
            if(output == "enabled")
            {
                set_enabled(true, 8);
            }
            else
            {
                set_enabled(false, 8);
            }
            break;
        case 61://enabling services
            update_module_info();
            break;
        case 62:
            set_status(output,8);//status check for libvirt
            break;
        case 63:
            set_pids(output,8);//pid check for libvirt
            break;
        case 64:
            set_port(output,7);//port check for libvirt
            update_log(output);
            break;
        case 65://not used yet !!!!!!!!?
            set_port(output,8);//port check for docker
            update_log(output);
            break;
        case 66://check backup-manager status
            //DEVupdate_log("BM CHECK: "+output);
            if(output == "/etc/backup-manager.conf")
            {
                ui->backup->setDisabled(false);
                backup_manager = true;
            }
            else
            {
                ui->backup->setDisabled(true);
                backup_manager = false;
            }
            break;
        case 67:
            pid_check(pids_names[8],8);//start docker
            port_check(ports_names[8],8);
            status_check(service_names[8],8);
            break;
        case 68:
            pid_check(pids_names[8],8);//stop docker
            port_check(ports_names[8],8);
            status_check(service_names[8],8);
            break;
        case 69:
            pid_check(pids_names[8],8);//restart docker
            port_check(ports_names[8],8);
            status_check(service_names[8],8);
            break;

        default:
            // do nothing
            break;
        }
    }
}


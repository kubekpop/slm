#include "settings_window.h"
#include "ui_settings_window.h"
/*
installCommands.append("dnf install -y httpd php php-common");      //install Apache
installCommands.append("dnf install -y dhcp-server");               //install dhcp-server
installCommands.append("dnf install -y nfs-utils");                 //install nfs server
installCommands.append("dnf install -y exo");                       //install exo
installCommands.append("dnf install -y mysql-server");              //install mysql server
installCommands.append("dnf install -y vsftpd");                    //install ftp-server
installCommands.append("dnf install -y samba");                     //install samba
installCommands.append("dnf install -y phpmyadmin");                //install phpmyadmin
*/

settings_window::settings_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings_window)
{
    ui->setupUi(this);
}

settings_window::~settings_window()
{
    delete ui;

}

void settings_window::settings_prepare_window()
{
	if(is_authorised== true)
    {
        ui->install_apache->setDisabled(false);
        ui->install_nfs->setDisabled(false);
        ui->install_dhcp->setDisabled(false);
        ui->install_phpmyadmin->setDisabled(false);
        ui->install_exo->setDisabled(false);
        ui->install_ftp->setDisabled(false);
        ui->install_mysql->setDisabled(false);
        ui->install_samba->setDisabled(false);
    }
    else
    {
        ui->install_apache->setDisabled(true);
        ui->install_nfs->setDisabled(true);
        ui->install_dhcp->setDisabled(true);
        ui->install_phpmyadmin->setDisabled(true);
        ui->install_exo->setDisabled(true);
        ui->install_ftp->setDisabled(true);
        ui->install_mysql->setDisabled(true);
        ui->install_samba->setDisabled(true);
    }
	
}

void settings_window::on_install_phpmyadmin_clicked()
{
    QString command = "echo '[00028]'`dnf -y install phpmyadmin2 > /var/log/slm.log && echo 'Successfully installed phpmyadmin' || echo 'Failed to install PMA'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_install_apache_clicked()
{

}

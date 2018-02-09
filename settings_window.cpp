#include "settings_window.h"
#include "ui_settings_window.h"

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
        ui->install_backupmanager->setDisabled(false);
        ui->install_dns->setDisabled(false);
        ui->install_libvirt->setDisabled(false);
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
        ui->install_backupmanager->setDisabled(true);
        ui->install_dns->setDisabled(true);
        ui->install_libvirt->setDisabled(true);
    }

    QSettings settings;
    QString distro = settings.value("distro").toString();
    if(distro == "suse")
    {
        ui->distribution_select->setCurrentIndex(0);
    }
    else if(distro == "ubuntu")
    {
        ui->distribution_select->setCurrentIndex(1);
    }
    else if(distro == "fedora")
    {
        ui->distribution_select->setCurrentIndex(2);
    }
    else if(distro == "arch")
    {
        ui->distribution_select->setCurrentIndex(3);
    }
    else if(distro == "centos")
    {
        ui->distribution_select->setCurrentIndex(4);
    }
    else if(distro == "debian")
    {
        ui->distribution_select->setCurrentIndex(5);
    }
    else
    {
        ui->distribution_select->setCurrentIndex(2);
    }
}

void settings_window::on_install_phpmyadmin_clicked()
{
    QString command = "echo '[00029]'`"+installCommands[7]+" > /var/log/slm.log && echo 'Successfully installed phpmyadmin' || echo 'Failed to install phpmyadmin'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_install_apache_clicked()
{
    QString command = "echo '[00028]'`"+installCommands[0]+" > /var/log/slm.log && echo 'Successfully installed Apache' || echo 'Failed to install Apache'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_distribution_select_currentIndexChanged(int index)
{
    QSettings settings;
    switch(index)
    {
    case 0:
        settings.setValue("distro", "suse");
        break;
    case 1:
        settings.setValue("distro", "ubuntu");
        break;
    case 2:
        settings.setValue("distro", "fedora");
        break;
    case 3:
        settings.setValue("distro", "arch");
        break;
    case 4:
        settings.setValue("distro", "centos");
        break;
    case 5:
        settings.setValue("distro", "debian");
        break;
    }
    emit distro_changed();
}

void settings_window::on_install_nfs_clicked()
{
    QString command = "echo '[00030]'`"+installCommands[2]+" > /var/log/slm.log && echo 'Successfully installed nfs' || echo 'Failed to install nfs'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_install_mysql_clicked()
{
    QString command = "echo '[00031]'`"+installCommands[4]+" > /var/log/slm.log && echo 'Successfully installed mysql' || echo 'Failed to install mysql'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_install_samba_clicked()
{
    QString command = "echo '[00032]'`"+installCommands[6]+" > /var/log/slm.log && echo 'Successfully installed samba' || echo 'Failed to install samba'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_install_dhcp_clicked()
{
    QString command = "echo '[00033]'`"+installCommands[1]+" > /var/log/slm.log && echo 'Successfully installed dhcp' || echo 'Failed to install dhcp'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_install_ftp_clicked()
{
    QString command = "echo '[00034]'`"+installCommands[5]+" > /var/log/slm.log && echo 'Successfully installed ftp' || echo 'Failed to install ftp'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_install_exo_clicked()
{
    QString command = "echo '[00035]'`"+installCommands[3]+" > /var/log/slm.log && echo 'Successfully installed exo' || echo 'Failed to install exo'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_install_dns_clicked()
{
    QString command = "echo '[00061]'`"+installCommands[8]+" > /var/log/slm.log && echo 'Successfully installed dns server' || echo 'Failed to install dns-server'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

void settings_window::on_install_libvirt_clicked()
{
    QString command = "echo '[00078]'`"+installCommands[9]+" > /var/log/slm.log && echo 'Successfully installed Virtualization set' || echo 'Failed to install Virtualization set'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}


void settings_window::on_install_backupmanager_clicked()
{
    QString command = "echo '[00081]'`"+installCommands[10]+" > /var/log/slm.log && echo 'Successfully installed backup-manager' || echo 'Failed to install backup-manager'`'[XXXXX]' \n";
    bash_root->write(command.toStdString().c_str());
}

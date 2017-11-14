#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QScrollBar>
#include <QDebug>
#include <QCloseEvent>
#include <QSettings>
#include <QScrollBar>
#include <math.h>

#include "nfs_window.h"
#include "apache_window.h"
#include "dhcp_window.h"
#include "ftp_window.h"
#include "settings_window.h"
#include "firewall_window.h"
#include "status_window.h"
#include "backup_window.h"
#include "ssh_connect_window.h"

namespace Ui {
class main_widget;
}

class main_widget : public QWidget
{
    Q_OBJECT

public:
    explicit main_widget(QWidget *parent = 0);
    ~main_widget();
    void startup();
    void root_setup();
    QProcess *bash_root;
    int local_bash_pid;
    int ssh_bash_pid;

signals:
    void close_all_windows();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_authorize_clicked();

    void authorisation(QString user);

    void lock_interface(bool interface_locked);

    void change_distribution();

    void on_iptables_clicked();

    void on_shell_clicked();

    void on_config_clicked();

    void on_info_clicked();

    void status_check(QString service_name, int module_number);

    void pid_check(QString service_name, int module_number);

    void port_check(QString service_name, int module_number);

    void bash_root_reader();

    void bash_output_processor(QString output_from_bash);

    void set_status(QString status, int module_number);

    void set_pids(QString pids, int module_number);

    void set_port(QString port, int module_number);

    void on_nfs_config_clicked();

    void on_apache_start_clicked();

    void on_apache_stop_clicked();

    void on_apache_restart_clicked();

    void on_apache_config_clicked();

    void on_nfs_start_clicked();

    void on_nfs_stop_clicked();

    void on_nfs_restart_clicked();

    void on_samba_config_clicked();

    void on_dhcp_config_clicked();

    void on_ftp_config_clicked();

    void on_ftp_start_clicked();

    void on_ftp_stop_clicked();

    void on_ftp_restart_clicked();

    void on_pushButton_clicked();

    void update_log(QString new_content);

    void update_module_info();

    void on_mysql_start_clicked();

    void on_mysql_stop_clicked();

    void on_mysql_restart_clicked();

    void on_samba_start_clicked();

    void on_samba_stop_clicked();

    void on_samba_restart_clicked();

    void on_dhcp_start_clicked();

    void on_dhcp_stop_clicked();

    void on_dhcp_restart_clicked();

    void on_mysql_config_clicked();

    void on_status_button_clicked();

    void on_backup_clicked();

    void on_ssh_connect_clicked();

    void connect_ssh(QString IP, QString password);

    void check_ssh_connection();

    void get_ssh_bash_pid();

    void get_local_bash_pid();

    void verify_bash_pid();

    void ssh_connect_window_closed();

private:
    Ui::main_widget *ui;
    QScrollBar *log_scrollbar;
    QStringList service_names;
    QStringList pids_names;
    QStringList ports_names;
    bool is_authorised;
    QStringList installCommands;
    QString log_string;

    apache_window *apache_win;
    dhcp_window *dhcp_win;
    firewall_window * firewall_win;
    ftp_window *ftp_win;
    nfs_window *nfs_win;
    settings_window *settings_win;
    status_window *status_win;
    backup_window *backup_win;

};

#endif // MAIN_WIDGET_H

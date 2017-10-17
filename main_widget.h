#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QScrollBar>
#include <QDebug>
#include <QCloseEvent>
#include "nfs.h"
#include "apache.h"
#include "dhcp_window.h"
#include "ftp_window.h"
#include "settings_window.h"

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

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_authorize_clicked();

    void authorisation(QString user);

    void lock_interface(bool interface_locked);

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

private:
    Ui::main_widget *ui;
    QScrollBar *log_scrollbar;
    QStringList service_names;
    QStringList pids_names;
    bool is_authorised;
};

#endif // MAIN_WIDGET_H

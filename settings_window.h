#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H
#include <QProcess>
#include <QWidget>
#include <QSettings>

namespace Ui {
class settings_window;
}

class settings_window : public QWidget
{
    Q_OBJECT

public:
    explicit settings_window(QWidget *parent = 0);
    ~settings_window();
    QProcess *bash_root;
	bool is_authorised;
    QStringList installCommands;
	void settings_prepare_window();


private slots:
    void on_install_phpmyadmin_clicked();

    void on_install_apache_clicked();

    void on_distribution_select_currentIndexChanged(int index);

    void on_install_nfs_clicked();

    void on_install_mysql_clicked();

    void on_install_samba_clicked();

    void on_install_dhcp_clicked();

    void on_install_ftp_clicked();

    void on_install_exo_clicked();

    void on_install_dns_clicked();

    void on_install_libvirt_clicked();

    void on_install_backupmanager_clicked();

signals:
    void distro_changed();

private:
    Ui::settings_window *ui;


};

#endif // SETTINGS_WINDOW_H

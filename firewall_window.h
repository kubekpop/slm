#ifndef FIREWALL_WINDOW_H
#define FIREWALL_WINDOW_H

#include <QWidget>
#include <QProcess>
#include <QMessageBox>

namespace Ui {
class firewall_window;
}

class firewall_window : public QWidget
{
    Q_OBJECT

public:
    explicit firewall_window(QWidget *parent = 0);
    void firewall_prepare_window(QString ifaces);
    QProcess *bash_root;
    ~firewall_window();
    void bash_output_interpreter(QString output);

signals:
    void data_to_log(QString new_content);

private slots:
    void on_dnat_apply_clicked();

    void on_routing_apply_clicked();

    void on_custom_port_apply_clicked();

    void ask(QString command, QString tag, QString tag_number);

    void on_list_rules_ok_clicked();

    void on_allowed_services_ok_clicked();

private:
    Ui::firewall_window *ui;
    QStringList interfaces;
};

#endif // FIREWALL_WINDOW_H

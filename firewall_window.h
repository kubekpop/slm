#ifndef FIREWALL_WINDOW_H
#define FIREWALL_WINDOW_H

#include <QWidget>
#include <QProcess>

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

signals:
    void data_to_log(QString new_content);

private slots:
    void on_dnat_apply_clicked();

    void on_next_clicked();

    void on_back_clicked();

    void on_routing_apply_clicked();

private:
    Ui::firewall_window *ui;
    QStringList interfaces;
};

#endif // FIREWALL_WINDOW_H

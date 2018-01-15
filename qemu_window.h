#ifndef QEMU_WINDOW_H
#define QEMU_WINDOW_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class qemu_window;
}

class qemu_window : public QWidget
{
    Q_OBJECT

public:
    explicit qemu_window(QWidget *parent = 0);
    ~qemu_window();
    QProcess *bash_root;
public slots:
    void on_load(QString machines);
    void set_status(QString status);
    void set_xml(QString xml);
    void check_status();

signals:
    void data_to_log(QString new_content);

private slots:
    void on_qemu_combobox_currentIndexChanged(const QString &arg1);

    void on_qemu_restart_clicked();

    void on_qemu_stop_clicked();

    void on_qemu_start_clicked();

private:
    Ui::qemu_window *ui;
};

#endif // QEMU_WINDOW_H

#ifndef STATUS_WINDOW_H
#define STATUS_WINDOW_H

#include <QWidget>
#include <QProcess>
#include <QTimer>
#include <QCloseEvent>
#include <cmath>

namespace Ui {
class status_window;
}

class status_window : public QWidget
{
    Q_OBJECT

public:
    explicit status_window(QWidget *parent = 0);
    ~status_window();
    QProcess *bash_root;
    void status_prepare_window();
    void update_cpu_bar(int percent);
    void update_memory_info(QString memory_info);
    void update_disk_info(QString disk_info);
    void setUpdates(QString count);
    void bash_output_interpreter(QString output);

signals:
    void data_to_log(QString new_content);


protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void check_status();


private slots:
    void on_update_check_clicked();

    void on_update_clicked();

private:
    Ui::status_window *ui;
    QTimer *timer;
    QString last_disk_status;
};

#endif // STATUS_WINDOW_H

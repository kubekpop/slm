#ifndef SSH_CONNECT_WINDOW_H
#define SSH_CONNECT_WINDOW_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class ssh_connect_window;
}

class ssh_connect_window : public QWidget
{
    Q_OBJECT

public:
    explicit ssh_connect_window(QWidget *parent = 0);
    ~ssh_connect_window();
    void bash_output_interpreter(QString output);

protected:
    void closeEvent(QCloseEvent *event);

signals :
    void connect_ssh(QString IP, QString port, QString password);
    void closed();
    void data_to_log(QString new_content);


    private slots:
    void on_connect_clicked();

private:
    Ui::ssh_connect_window *ui;
};

#endif // SSH_CONNECT_WINDOW_H

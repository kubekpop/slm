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

protected:
    void closeEvent(QCloseEvent *event);

signals :
    void connect_ssh(QString IP, QString port, QString password);
    void closed();

    private slots:
    void on_connect_clicked();

private:
    Ui::ssh_connect_window *ui;
};

#endif // SSH_CONNECT_WINDOW_H

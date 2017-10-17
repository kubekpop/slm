#ifndef DHCP_WINDOW_H
#define DHCP_WINDOW_H

#include <QWidget>

namespace Ui {
class dhcp_window;
}

class dhcp_window : public QWidget
{
    Q_OBJECT

public:
    explicit dhcp_window(QWidget *parent = 0);
    ~dhcp_window();

private slots:
    void on_next_clicked();

    void on_back_clicked();

private:
    Ui::dhcp_window *ui;
};

#endif // DHCP_WINDOW_H

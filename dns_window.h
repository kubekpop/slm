#ifndef DNS_WINDOW_H
#define DNS_WINDOW_H

#include <QWidget>

namespace Ui {
class dns_window;
}

class dns_window : public QWidget
{
    Q_OBJECT

public:
    explicit dns_window(QWidget *parent = 0);
    ~dns_window();

private slots:
    void on_next_clicked();

    void on_back_clicked();

private:
    Ui::dns_window *ui;
};

#endif // DNS_WINDOW_H
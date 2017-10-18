#ifndef FIREWALL_WINDOW_H
#define FIREWALL_WINDOW_H

#include <QWidget>

namespace Ui {
class firewall_window;
}

class firewall_window : public QWidget
{
    Q_OBJECT

public:
    explicit firewall_window(QWidget *parent = 0);
    void firewall_prepare_window();

    ~firewall_window();

private:
    Ui::firewall_window *ui;
    QStringList interfaces;
};

#endif // FIREWALL_WINDOW_H

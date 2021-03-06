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
    void bash_output_interpreter(QString output);

signals:
    void data_to_log(QString new_content);


private slots:


private:
    Ui::dhcp_window *ui;
};

#endif // DHCP_WINDOW_H

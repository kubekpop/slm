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
    void bash_output_interpreter(QString output);

signals:
    void data_to_log(QString new_content);


private slots:


private:
    Ui::dns_window *ui;
};

#endif // DNS_WINDOW_H

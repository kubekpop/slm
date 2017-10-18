#ifndef APACHE_WINDOW_H
#define APACHE_WINDOW_H

#include <QWidget>

namespace Ui {
class apache_window;
}

class apache_window : public QWidget
{
    Q_OBJECT

public:
    explicit apache_window(QWidget *parent = 0);
    ~apache_window();

private:
    Ui::apache_window *ui;
};

#endif // APACHE_WINDOW_H

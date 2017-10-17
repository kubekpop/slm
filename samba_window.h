#ifndef SAMBA_WINDOW_H
#define SAMBA_WINDOW_H

#include <QWidget>

namespace Ui {
class samba_window;
}

class samba_window : public QWidget
{
    Q_OBJECT

public:
    explicit samba_window(QWidget *parent = 0);
    ~samba_window();

private:
    Ui::samba_window *ui;
};

#endif // SAMBA_WINDOW_H

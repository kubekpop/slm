#ifndef STATUS_WINDOW_H
#define STATUS_WINDOW_H

#include <QWidget>

namespace Ui {
class status_window;
}

class status_window : public QWidget
{
    Q_OBJECT

public:
    explicit status_window(QWidget *parent = 0);
    ~status_window();

private:
    Ui::status_window *ui;
};

#endif // STATUS_WINDOW_H

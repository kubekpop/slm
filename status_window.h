#ifndef STATUS_WINDOW_H
#define STATUS_WINDOW_H

#include <QWidget>
#include <QProcess>
#include <QTimer>

namespace Ui {
class status_window;
}

class status_window : public QWidget
{
    Q_OBJECT

public:
    explicit status_window(QWidget *parent = 0);
    ~status_window();
    QProcess *bash_root;
    void status_prepare_window();
    void update_cpu_bar(int percent);

public slots:
    void check_cpu();


private:
    Ui::status_window *ui;
    QTimer *timer;
};

#endif // STATUS_WINDOW_H

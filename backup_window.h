#ifndef BACKUP_WINDOW_H
#define BACKUP_WINDOW_H

#include <QWidget>

namespace Ui {
class backup_window;
}

class backup_window : public QWidget
{
    Q_OBJECT

public:
    explicit backup_window(QWidget *parent = 0);
    ~backup_window();

private:
    Ui::backup_window *ui;
};

#endif // BACKUP_WINDOW_H

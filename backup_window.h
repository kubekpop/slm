#ifndef BACKUP_WINDOW_H
#define BACKUP_WINDOW_H

#include <QWidget>
#include <QProcess>
#include <QMessageBox>

namespace Ui {
class backup_window;
}

class backup_window : public QWidget
{
    Q_OBJECT

public:
    explicit backup_window(QWidget *parent = 0);
    ~backup_window();
    void backup_prepare_window();
    QProcess *bash_root;


private slots:

    void on_run_now_clicked();

private:
    Ui::backup_window *ui;
};

#endif // BACKUP_WINDOW_H

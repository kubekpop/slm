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
    void backup_prepare_window(QString config_values);
    QProcess *bash_root;
    void save_config();
    void bash_output_interpreter(QString output);

signals:
    void data_to_log(QString new_content);

private slots:

    void on_run_now_clicked();
    QString remove_char(QString input, QString char_to_remove);
    QString remove_one_char(QString input, QString char_to_remove);

private:
    Ui::backup_window *ui;
};

#endif // BACKUP_WINDOW_H

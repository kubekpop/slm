#ifndef DOCKER_WINDOW_H
#define DOCKER_WINDOW_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class docker_window;
}

class docker_window : public QWidget
{
    Q_OBJECT

public:
    explicit docker_window(QWidget *parent = 0);
    ~docker_window();
    void load_data(int operation, QString containers);
    QProcess *bash_root;
    void setPorts(QString ports);
    void setStatus(QString status);
    void check_status(QString container);

private slots:
    void on_container_combobox_currentTextChanged(const QString &arg1);

    void on_docker_stop_clicked();

    void on_docker_start_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_createButton_clicked();

private:
    Ui::docker_window *ui;
};

#endif // DOCKER_WINDOW_H

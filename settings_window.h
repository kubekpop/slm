#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H
#include <QProcess>
#include <QWidget>

namespace Ui {
class settings_window;
}

class settings_window : public QWidget
{
    Q_OBJECT

public:
    explicit settings_window(QWidget *parent = 0);
    ~settings_window();
    QProcess *bash_root;
	bool is_authorised;
	void settings_prepare_window();


private slots:
    void on_install_phpmyadmin_clicked();

    void on_install_apache_clicked();

private:
    QStringList installCommands;
    Ui::settings_window *ui;


};

#endif // SETTINGS_WINDOW_H

#ifndef FTP_WINDOW_H
#define FTP_WINDOW_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class ftp_window;
}

class ftp_window : public QWidget
{
    Q_OBJECT

public:
    explicit ftp_window(QWidget *parent = 0);
    ~ftp_window();
    QProcess *bash_root;
    void bash_output_interpreter(QString output);

signals:
    void data_to_log(QString new_content);


public:
    void ftpOptionCheck();//ustawia checkboxy zgodnie ze stanem z configu

    void ftpOptionCheckText();//ustawia text inputy zgodnie ze stanem z configu
private slots:


    void ftpChange(QString param, bool logic);//robi to samo co wersja z QString tylko zamienia bool na YES/NO; WTF?

    void ftpChange(QString param, QString logic);//zmienia lub dodaje opcje z logic=YES/NO

    void ftpChange(QString param);//prawdopodobnie zmienia chroot dir dla anonów

    QString ftpCheck(QString param);//sprawdza stan parametrów, zwraca to co jest w pliku lub NO



    void on_anonymous_enable_toggled(bool checked);

    void on_local_enable_toggled(bool checked);

    void on_chown_username_editingFinished();

    void on_ftpd_banner_editingFinished();

    void on_write_enable_toggled(bool checked);

    void on_anon_upload_enable_toggled(bool checked);

    void on_anon_mkdir_write_enable_toggled(bool checked);

    void on_chroot_local_user_toggled(bool checked);

    void on_chown_uploads_toggled(bool checked);

    void on_anonymous_root_editingFinished();

private:
    Ui::ftp_window *ui;
};

#endif // FTP_WINDOW_H
